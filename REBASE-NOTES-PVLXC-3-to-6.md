# LXC-PV Rebase Notes: stable-3.0 to stable-6.0

This document describes the porting of Pantavisor-specific patches from LXC stable-3.0 to stable-6.0.

## Branch Information

- **Source branch:** `stable-3.0-BASE-2c5c780762981a5cfe699670c91397e29f6f6516`
- **Target branch:** `stable-6.0-BASE-f9ff9ea2a` (based on upstream `stable-6.0`)
- **Upstream branchpoint:** `2c5c780762981a5cfe699670c91397e29f6f6516`

## Ported Patches (11 commits)

| Commit | Description | Original 3.0 Commits |
|--------|-------------|---------------------|
| `0f4910966` | Add pv_export.h header for namespace enum export | `9c52e4cd4` |
| `897b2c546` | Add set_container_type() API + lxc_log_set_alternative_output() | `dc9f40c91`, `e8e97ea89` |
| `568fd7fd9` | Add lxc.rootfs.bdev_type config key | `68fce9f67` |
| `38180347e` | Add realpath_x() for virtual root path resolution | `e66bd42c2`, `4acd66a1e`, `b03b9cde2`, `bf4be7b03`, `7aa1ad8aa`, `78c77294f` |
| `21cf380b1` | Add origin=mkdir mount option for source directory creation | `8df1f4f9e` |
| `5efac56de` | Enhance lxc_string_split_quoted to handle escaped characters | `031460443`, `be0b56fcd`, `f032c3f03` |
| `afa148a5a` | Change cgroup prefix from lxc.payload. to lxc/ | `ea36bada4` |
| `aa2b45c33` | Support hook commands with parameters | `79836fa9b` |
| `764899613` | Allow overlay mount entries with upper/workdir outside container dir | `937c195ca` |
| `d99a550c2` | Support /exports mounts for Pantavisor | `da3d6e465` |
| `9343fe098` | Block SIGCHLD in run_buffer to prevent child reaping issues | `e202a8371`, `8e50fba97` |

### Additional Changes in Ported Patches

- **pv-root platform support:** The `set_container_type()` commit also includes logic to omit setting `container=` env var when type is "pv-root" (from `6fc218fba`)

## Not Ported - Merge Commits (5)

These are merge commit metadata, not actual code changes:

| Commit | Description |
|--------|-------------|
| `42955234e` | Merge bugfix/reaping-2023 |
| `132558831` | Merge fix/sprintf-null-value |
| `b25e8436e` | Merge bugfix/lxc-top-no-rows-dev-console |
| `447b5b4b6` | Merge feature/lxc-log-capture |
| `b8a13cd40` | Merge feature/overlaymount-allow-upper-work-outside-rootfs |

## Not Ported - CI/Infrastructure (4)

These are CI configuration files not applicable to the new branch:

| Commit | Description |
|--------|-------------|
| `9cb56aed3` | Update .gitlab-ci.yml |
| `18fe3fcfb` | bump device-ci |
| `9739ecdd8` | Adding .gitlab-ci.yml |
| `a309b2054` | Add atom.mk for alchemy build |

## Not Ported - Autotools/Alchemy Build (7)

These patches modified `configure.ac` or `atom.mk` for the autotools/Alchemy build systems. LXC 6.0 uses meson which already handles these features:

| Commit | Description | Why Not Needed |
|--------|-------------|----------------|
| `8185580f3` | link liblxc dynamically by default | Meson builds both static and shared |
| `6dae5c40e` | add libcap as optional library | Meson handles via `-Dcapabilities=` |
| `fefaac0aa` | add apparmor as optional conditional library | Meson handles via `-Dapparmor=` |
| `ecb954cf8` | add libseccomp as a conditional library | Meson handles via `-Dseccomp=` |
| `05dc7054c` | fix build failure in buster; --disable-selinux | Meson handles via `-Dselinux=` |
| `dc150ef45` | Support the static lib from configure.ac | Meson already builds `liblxc.a` |
| `d7db073f7` | Fixup static library creation | Meson already builds `liblxc.a` |

## Not Ported - Cleanup/Trivial (2)

| Commit | Description | Why Not Needed |
|--------|-------------|----------------|
| `12b46e826` | remove COPYING wrongly added | Cleanup from previous rebase |
| `fdf06cc72` | remove unnecessary comment | Trivial cleanup |

## Not Ported - Already Fixed in 6.0 (1)

| Commit | Description | Status in 6.0 |
|--------|-------------|---------------|
| `5ec9e19a6` | Check only rootfs as filesystem type | Fixed upstream - uses `strnequal(p, "- rootfs ", 9)` |

## Not Ported - Bug Fixes (7)

These bug fixes were assessed and determined not to be needed in 6.0:

| Commit | Description | Why Not Needed |
|--------|-------------|----------------|
| `b456067f7` | fix void arg prototypes | May be fixed upstream; minor C compliance |
| `9113e4767` | Fixes sprintf NULL value in conf.c | Our origin=mkdir implementation is different |
| `8c287d105` | make run_buffer more resilient to large outputs | Optional - bumps buffer sizes |
| `f4b8df328` | lxc-top /dev/console rows default | Optional - minor tool fix for serial consoles |
| `ab09a6a5e` | fix nonnull build errors in conf.c | Our realpath_x integration handles NULL properly |
| `ad43e43fc` | fix stringop-overflow in cgfsng.c | Code pattern doesn't exist in 6.0 |
| `684afa6bf` + `8d0838165` | ptmx check (added then reverted) | Was reverted - not needed |

## Not Ported - Superseded (1)

| Commit | Description | Why Not Needed |
|--------|-------------|----------------|
| `f032c3f03` | init.cmd with spaces and quotes | Superseded by `031460443` (better state machine impl) |

## Key Differences Between 3.0 and 6.0

1. **Build System:** Autotools → Meson
2. **Data Structures:** `struct lxc_list` → `struct list_head` (kernel-style linked lists)
3. **String Functions:** Some new helper functions like `strnequal()`, `strequal()`
4. **Code Organization:** Some functions moved between files (e.g., `run_buffer` moved to utils.c)
5. **Mount Options:** Now uses `struct lxc_mount_options` with bitfields and `parse_lxc_mount_attrs()`

## Testing Notes

The following features should be tested after porting:

1. Container type override (`set_container_type()`)
2. Log capture (`lxc_log_set_alternative_output()`)
3. Mount handling with symlinks (realpath_x)
4. `origin=mkdir` mount option
5. Init commands with quoted arguments
6. Hook commands with parameters
7. Overlay mounts with external upper/workdir
8. /exports mount handling
9. pv-root platform (no container= env var)
