# CLAUDE.md - Pantavisor LXC Fork

lxc-pv is a fork of [LXC](https://github.com/lxc/lxc) with Pantavisor-specific patches.

- Upstream: https://github.com/lxc/lxc
- Fork: https://gitlab.com/pantacor/lxc
- Current base: `stable-3.0` @ `2c5c780762981a5cfe699670c91397e29f6f6516`

## Pantavisor Patches

### 1. API Extensions (Required)

| Commit | Description | Used in pv_lxc.c |
|--------|-------------|------------------|
| `dc9f40c91` | `set_container_type()` - override `container=` env var | Yes (line 383) |
| `dc9f40c91` | `set_inherit_namespaces()` - inherit namespaces from PID | No (possibly legacy) |
| `e8e97ea89` | `lxc_log_set_alternative_output()` - pipe logs to fd | Yes (line 568) |
| `68fce9f67` | `lxc.rootfs.bdev_type` config key | Yes (line 250) |

### 2. Build System (Adaptation Required - autotools to meson)

| Commit | Description | Notes |
|--------|-------------|-------|
| `dc9f40c91` | Static library build (`liblxc.a`) | Needs meson adaptation |
| `a309b2054` | atom.mk for alchemy build | May be obsolete |
| `dc150ef45` | Support static lib from configure.ac | Needs meson adaptation |
| `d7db073f7` | Fixup static library creation | Needs meson adaptation |
| `9c52e4cd4` | `pv_export.h` header for namespace enums | Still needed |

### 3. Init Command Parsing (Required)

| Commit | Description | Notes |
|--------|-------------|-------|
| `f032c3f03` | Quoted arguments in init.cmd | Original implementation |
| `be0b56fcd` | Bug fix for rightmost char | Fix |
| `031460443` | State machine rewrite with escape handling | Final version |

### 4. Mount Handling (Required)

| Commit | Description | Notes |
|--------|-------------|-------|
| `8df1f4f9e` | `origin=mkdir` mount option | Used in pv_lxc.c |
| `b03b9cde2` | Relative symlink mount target support | Important |
| `e66bd42c2` | realpath 2013 implementation | Foundation |
| `4acd66a1e` | `realpath_x()` for virtual root | Core feature |
| `78c77294f` | Fix realpath include on glibc | Bug fix |
| `bf4be7b03` | Error handling in realpath_x | Bug fix |
| `7aa1ad8aa` | Fix illegal memory access in realpath_x | Bug fix |
| `937c195ca` | Overlay outside rootfs | Feature |
| `da3d6e465` | Support /exports mounts | Feature |

### 5. Cgroup/Environment (Required)

| Commit | Description | Notes |
|--------|-------------|-------|
| `ea36bada4` | `lxc/` prefix instead of `lxc.payload.` | Compatibility |
| `6fc218fba` | Omit container env for pv-root | Feature |

### 6. Bug Fixes/Improvements (Review if upstream)

| Commit | Description | Status |
|--------|-------------|--------|
| `79836fa9b` | Hook commands with parameters | Check upstream |
| `8c287d105` | Larger run_buffer | Check upstream |
| `8e50fba97` | run_buffer SIGCHLD blocking | Check upstream |
| `b456067f7` | Fix void arg prototypes | Check upstream |
| `f4b8df328` | lxc-top default rows for ttySXXX | Check upstream |
| `ab09a6a5e` | Fix sprintf nonnull warning in conf.c | Check upstream |
| `ad43e43fc` | Fix stringop-overflow in cgfsng.c | Check upstream |
| `9113e4767` | Fix sprintf NULL value in conf.c | Check upstream |
| `e202a8371` | Avoid reaping by global pantavisor | Check upstream |
| `5ec9e19a6` | Check only rootfs as filesystem type | Check upstream |
| `05dc7054c` | Fix build failure --disable-selinux | Check upstream |

### 7. Conditional Libraries (Check if upstream)

| Commit | Description | Notes |
|--------|-------------|-------|
| `ecb954cf8` | libseccomp as conditional library | Check upstream |
| `fefaac0aa` | apparmor as optional conditional library | Check upstream |
| `6dae5c40e` | libcap as optional library | Check upstream |
| `8185580f3` | Link liblxc dynamically by default | Check upstream |

### 8. Skip (CI/Cleanup)

| Commit | Description | Notes |
|--------|-------------|-------|
| `9739ecdd8` | Adding .gitlab-ci.yml | CI |
| `9cb56aed3` | Update .gitlab-ci.yml | CI |
| `18fe3fcfb` | Bump device-ci for docker login | CI |
| `8d0838165` | Check if ptmx exists | Reverted |
| `684afa6bf` | Revert ptmx check | Revert |
| `fdf06cc72` | Remove unnecessary comment | Cleanup |
| `12b46e826` | Remove COPYING wrongly added | Cleanup |
| Merge commits | Various merge commits | Skip |

---

## pv_lxc.c API Dependencies

The consumer plugin (`pantavisor/plugins/pv_lxc.c`) uses:

### Standard LXC API (should work)
- `lxc_container_new()`, `lxc_container_put()`
- `c->load_config()`, `c->save_config()`, `c->clear_config()`
- `c->start()`, `c->shutdown()`, `c->init_pid()`
- `c->get_config_item()`, `c->set_config_item()`
- `c->want_daemonize()`, `c->want_close_all_fds()`
- `c->console_getfd()`
- `lxc_log_init()`

### Custom API (needs porting)
- `c->set_container_type()` - **Required**
- `lxc_log_set_alternative_output()` - **Required**
- `lxc.rootfs.bdev_type` config key - **Required**

### Custom mount options (needs porting)
- `origin=mkdir` - **Required**

---

## Notes

### Upstream Changes (3.0 → 6.0)

- Build system changed from **autotools** to **meson**
- Data structures changed from `struct lxc_list` to `struct list_head` (kernel-style)
- Many APIs have been refactored

### Rebase Strategy

1. Create fresh branch from `upstream/stable-6.0`
2. Reimplement patches in order of dependency
3. Adapt autotools patches to meson build system
4. Test with Pantavisor build
