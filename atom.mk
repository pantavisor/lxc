
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lxc
LOCAL_DESCRIPTION := Linux Containers
LOCAL_CONDITIONAL_LIBRARIES := OPTIONAL:libseccomp OPTIONAL:apparmor OPTIONAL:libcap

LOCAL_EXPORT_LDLIBS = -llxc

LOCAL_MESON_CONFIGURE_ARGS := -Dseccomp=false -Dselinux=false -Dexamples=false -Dman=false -Dtests=false

LOCAL_CLEAN_DIRS := $(call local-get-build-dir)/usr/share/ $(call local-get-build-dir)/usr/include/

include $(BUILD_MESON)

