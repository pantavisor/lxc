/* SPDX-License-Identifier: LGPL-2.1+ */

/*
 * Pantacor realpath_x - resolve paths relative to a virtual root
 *
 * Based on FreeBSD realpath implementation:
 * Copyright (c) 2003 Constantin S. Svintsoff <kostik@iclub.nsu.ru>
 *
 * Pantavisor modifications:
 * Copyright (c) 2020 Pantacor Ltd
 * Author: Alexander Sack <asac@pantacor.com>
 */

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include "log.h"

lxc_log_define(realpath_x, lxc);

static size_t pv_strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';
		while (*src++)
			;
	}

	return (src - osrc - 1);
}

static size_t pv_strlcat(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return (dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return (dlen + (s - src));
}

/*
 * Find the real name of path relative to root, by removing all ".", ".."
 * and symlink components. Unlike standard realpath, this does not require
 * the final target to exist, and all symlink resolution stays within root.
 */
char *realpath_x(const char *root, const char *path, char *resolved)
{
	struct stat sb;
	char *p, *q, *s;
	size_t left_len, resolved_len;
	unsigned symlinks;
	int m, slen;
	char left[PATH_MAX], next_token[PATH_MAX], symlink[PATH_MAX];
	char root_path[PATH_MAX];
	size_t root_len;

	TRACE("realpath_x: root=%s path=%s", root ? root : "(null)", path ? path : "(null)");

	if (path == NULL) {
		errno = EINVAL;
		return NULL;
	}
	if (path[0] == '\0') {
		errno = ENOENT;
		return NULL;
	}
	if (resolved == NULL) {
		resolved = malloc(PATH_MAX);
		if (resolved == NULL)
			return NULL;
		m = 1;
	} else {
		m = 0;
	}
	if (root == NULL) {
		errno = EINVAL;
		if (m)
			free(resolved);
		return NULL;
	}

	pv_strlcpy(root_path, root, sizeof(root_path));
	root_len = strlen(root_path);
	if (root_path[root_len - 1] != '/') {
		root_path[root_len] = '/';
		root_len++;
		root_path[root_len] = '\0';
	}

	symlinks = 0;
	if (path[0] == '/') {
		pv_strlcpy(resolved, root_path, PATH_MAX);
		if (path[1] == '\0')
			return resolved;
		resolved_len = root_len;
		left_len = pv_strlcpy(left, path + 1, sizeof(left));
	} else {
		pv_strlcpy(resolved, root_path, PATH_MAX);
		resolved_len = strlen(resolved);
		left_len = pv_strlcpy(left, path, sizeof(left));
	}

	if (left_len >= sizeof(left) || resolved_len >= PATH_MAX) {
		if (m)
			free(resolved);
		errno = ENAMETOOLONG;
		return NULL;
	}

	while (left_len != 0) {
		p = strchr(left, '/');
		s = p ? p : left + left_len;
		if ((size_t)(s - left) >= sizeof(next_token)) {
			if (m)
				free(resolved);
			errno = ENAMETOOLONG;
			return NULL;
		}
		memcpy(next_token, left, s - left);
		next_token[s - left] = '\0';
		left_len -= s - left;
		if (p != NULL)
			memmove(left, s + 1, left_len + 1);

		if (resolved[resolved_len - 1] != '/') {
			if (resolved_len + 1 >= PATH_MAX) {
				if (m)
					free(resolved);
				errno = ENAMETOOLONG;
				return NULL;
			}
			resolved[resolved_len++] = '/';
			resolved[resolved_len] = '\0';
		}

		if (next_token[0] == '\0') {
			if (lstat(resolved, &sb) == 0 && !S_ISDIR(sb.st_mode)) {
				if (m)
					free(resolved);
				errno = ENOTDIR;
				return NULL;
			}
			continue;
		} else if (strcmp(next_token, ".") == 0) {
			continue;
		} else if (strcmp(next_token, "..") == 0) {
			if (resolved_len > root_len) {
				resolved[resolved_len - 1] = '\0';
				q = strrchr(resolved, '/') + 1;
				*q = '\0';
				resolved_len = q - resolved;
			}
			continue;
		}

		resolved_len = pv_strlcat(resolved, next_token, PATH_MAX);
		if (resolved_len >= PATH_MAX) {
			if (m)
				free(resolved);
			errno = ENAMETOOLONG;
			return NULL;
		}

		if (lstat(resolved, &sb) != 0) {
			/* Target doesn't exist yet - that's OK for create=dir/file */
			continue;
		}

		if (S_ISLNK(sb.st_mode)) {
			if (symlinks++ > MAXSYMLINKS) {
				if (m)
					free(resolved);
				errno = ELOOP;
				return NULL;
			}
			slen = readlink(resolved, symlink, sizeof(symlink) - 1);
			if (slen < 0) {
				if (m)
					free(resolved);
				return NULL;
			}
			symlink[slen] = '\0';

			if (symlink[0] == '/') {
				/* Absolute symlink - reset to root */
				resolved[root_len] = '\0';
				resolved_len = root_len;
			} else if (resolved_len > root_len) {
				/* Relative symlink - strip last component */
				resolved[resolved_len - 1] = '\0';
				q = strrchr(resolved, '/') + 1;
				*q = '\0';
				resolved_len = q - resolved;
			}

			if (p != NULL) {
				if (symlink[slen - 1] != '/') {
					if ((size_t)(slen + 1) >= sizeof(symlink)) {
						if (m)
							free(resolved);
						errno = ENAMETOOLONG;
						return NULL;
					}
					symlink[slen] = '/';
					symlink[slen + 1] = '\0';
				}
				left_len = pv_strlcat(symlink, left, sizeof(symlink));
				if (left_len >= sizeof(left)) {
					if (m)
						free(resolved);
					errno = ENAMETOOLONG;
					return NULL;
				}
			}
			left_len = pv_strlcpy(left, symlink, sizeof(left));
		}
	}

	if (resolved_len > 1 && resolved[resolved_len - 1] == '/')
		resolved[resolved_len - 1] = '\0';

	TRACE("realpath_x: resolved=%s", resolved);
	return resolved;
}
