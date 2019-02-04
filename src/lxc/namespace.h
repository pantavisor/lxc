/* SPDX-License-Identifier: LGPL-2.1+ */

#ifndef __LXC_NAMESPACE_H
#define __LXC_NAMESPACE_H

#include "config.h"

#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "compiler.h"

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

#ifndef CLONE_CHILD_SETTID
#define CLONE_CHILD_SETTID 0x01000000
#endif

#ifndef CLONE_VFORK
#define CLONE_VFORK 0x00004000
#endif

#ifndef CLONE_THREAD
#define CLONE_THREAD 0x00010000
#endif

#ifndef CLONE_SETTLS
#define CLONE_SETTLS 0x00080000
#endif

#ifndef CLONE_VM
#define CLONE_VM 0x00000100
#endif

#ifndef CLONE_FILES
#define CLONE_FILES 0x00000400
#endif

#ifndef CLONE_FS
#  define CLONE_FS                0x00000200
#endif
#ifndef CLONE_NEWNS
#  define CLONE_NEWNS             0x00020000
#endif
#ifndef CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP         0x02000000
#endif
#ifndef CLONE_NEWUTS
#  define CLONE_NEWUTS            0x04000000
#endif
#ifndef CLONE_NEWIPC
#  define CLONE_NEWIPC            0x08000000
#endif
#ifndef CLONE_NEWUSER
#  define CLONE_NEWUSER           0x10000000
#endif
#ifndef CLONE_NEWPID
#  define CLONE_NEWPID            0x20000000
#endif
#ifndef CLONE_NEWNET
#  define CLONE_NEWNET            0x40000000
#endif

#include "pv_export.h"

__hidden extern const struct ns_info {
#define MAX_NS_PROC_NAME 6
	const char proc_name[MAX_NS_PROC_NAME + 1];
	const char *proc_path;
	int clone_flag;
	const char *flag_name;
	const char *env_name;
} ns_info[LXC_NS_MAX];

__hidden extern int lxc_namespace_2_cloneflag(const char *namespace);
__hidden extern int lxc_namespace_2_ns_idx(const char *namespace);
__hidden extern int lxc_namespace_2_std_identifiers(char *namespaces);
__hidden extern int lxc_fill_namespace_flags(char *flaglist, int *flags);

#endif /* __LXC_NAMESPACE_H */
