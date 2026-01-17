/* SPDX-License-Identifier: LGPL-2.1+ */

#ifndef __LXC_PV_EXPORT_H
#define __LXC_PV_EXPORT_H

/*
 * Pantavisor export header - exposes LXC namespace constants
 * for external consumers like pv_lxc.c
 */

typedef enum lxc_namespace_t {
	LXC_NS_USER	= 0,
	LXC_NS_MNT	= 1,
	LXC_NS_PID	= 2,
	LXC_NS_UTS	= 3,
	LXC_NS_IPC	= 4,
	LXC_NS_NET	= 5,
	LXC_NS_CGROUP	= 6,
	LXC_NS_TIME	= 7,
	LXC_NS_MAX	= 8
} lxc_namespace_t;

#endif /* __LXC_PV_EXPORT_H */
