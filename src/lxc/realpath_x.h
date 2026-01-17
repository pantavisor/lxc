/* SPDX-License-Identifier: LGPL-2.1+ */

/*
 * Pantacor realpath_x - resolve paths relative to a virtual root
 *
 * Copyright (c) 2020 Pantacor Ltd
 * Author: Alexander Sack <asac@pantacor.com>
 */

#ifndef __LXC_REALPATH_X_H
#define __LXC_REALPATH_X_H

/*
 * Resolve path relative to root, handling symlinks within the virtual root.
 * Unlike standard realpath(), this does not require the final target to exist.
 */
char *realpath_x(const char *root, const char *path, char *resolved);

#endif /* __LXC_REALPATH_X_H */
