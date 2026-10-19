# LXC-PV Rebase Notes: stable-6.0 to v7.0.0

This document describes the porting of Pantavisor-specific patches from LXC stable-6.0 to v7.0.0.

## Branch Information

- **Source branch:** `stable-6.0-BASE-f9ff9ea2a` (tip `c2017d421`)
- **Target branch:** `stable-v7.0.0` (based on upstream tag `v7.0.0`, `e4415af07`)

## Ported Patches (12 commits)

All patches applied without conflicts and without changes (`git range-diff` shows them identical).

| 7.0 Commit | 6.0 Commit | Description |
|------------|------------|-------------|
| `65abdf4fe` | `0f4910966` | Add pv_export.h header for namespace enum export |
| `c6090f5a4` | `897b2c546` | Add set_container_type() API |
| `5125a30cb` | `568fd7fd9` | Add lxc.rootfs.bdev_type config key |
| `736e8d0ff` | `38180347e` | Add realpath_x() for virtual root path resolution |
| `55f4913f8` | `21cf380b1` | Add origin=mkdir mount option for source directory creation |
| `7a726a3d6` | `5efac56de` | Enhance lxc_string_split_quoted to handle escaped characters |
| `a5f8279a2` | `afa148a5a` | Change cgroup prefix from lxc.payload. to lxc/ |
| `105c25f1e` | `aa2b45c33` | Support hook commands with parameters |
| `0111eca35` | `764899613` | Allow overlay mount entries with upper/workdir outside container dir |
| `9dbe388da` | `d99a550c2` | Support /exports mounts for Pantavisor |
| `1acdb510e` | `9343fe098` | Block SIGCHLD in run_buffer to prevent child reaping issues |
| `577c9fe53` | `c2017d421` | Rebase notes 3.0 to 6.0 |

## Not Ported

| Branch | Commit | Why |
|--------|--------|-----|
| `fix/chroot-mount-propagation-6.0` | `fb29ad946` | Never merged into stable-6.0 |

## Upstream 7.0 Changes That Touch Our Patches

- **Hook quoting** (`589981f4f`, `2bc6bac9c`): upstream now single-quotes the extra
  hook arguments but leaves the hook command itself unquoted, so
  `lxc.hook.start = /path/script arg1 arg2` still works with our hook patch.
- **Mount options** (`afb35f3de`, `2e08794e0`, `30b11e54a`): new mount flags
  (`nosymfollow`, `move`, flag-style options) are mount flags, not
  `lxc_mount_options_t` entries, so `origin=mkdir` keeps its slot.

## Upstream 7.0 Changes That Affect Pantavisor at Runtime

These are not patch conflicts, but they change what devices LXC 7.0 runs on:

1. **cgroup v1 removed** (`f29c3a3a6`, `dfa8ea976`): the cgroup v1 limit keys
   (`lxc.cgroup.<controller>.*`, e.g. `lxc.cgroup.devices.allow`) and the cgroup v1
   device controller are gone; `lxc.cgroup.dir*` and `lxc.cgroup.relative` remain.
   Limits must use `lxc.cgroup2.*` and the host must run a unified (v2) hierarchy.
2. **New kernel baseline** (`9e541fa86`, `87ff1e31e`): LXC now assumes
   `fsopen`/`open_tree`/`mount_setattr` (Linux 5.12+) and `clone3`/`CLONE_PIDFD`
   (Linux 5.5+) without fallback.
3. **Landlock on the monitor** (`226bbf62f`): new `landlock-monitor` meson option.
4. **`lxc.environment` split** (`fbd37f04c`): new `lxc.environment.runtime` and
   `lxc.environment.hooks` keys; plain `lxc.environment` still sets both.

## Testing Notes

Same feature list as in `REBASE-NOTES-PVLXC-3-to-6.md`, plus:

1. Containers on a cgroup v2 host, with `lxc.cgroup2.*` limits
2. Hook commands with parameters, given the new upstream quoting
