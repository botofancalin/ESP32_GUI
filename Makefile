#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32_lvgl

#If SOLUTION_PATH is not defined, use relative path as default value
IOT_SOLUTION_PATH ?= $(abspath $(shell pwd)/)
 
include $(IOT_SOLUTION_PATH)/components/component_conf.mk
include $(IDF_PATH)/make/project.mk