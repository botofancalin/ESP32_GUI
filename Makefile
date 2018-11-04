#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32_LVL

COMPONENT_ADD_INCLUDEDIRS := components/
INC_DIR = include
DEPS = $(INC_DIR)/lv_conf.h

include $(IDF_PATH)/make/project.mk

# Copy some defaults into the sdkconfig by default
# so BT stack is enabled
EXTRA_COMPONENT_DIRS:=$(b)components
sdkconfig: sdkconfig.defaults
	$(Q) cp $< $@

menuconfig: sdkconfig
defconfig: sdkconfig
