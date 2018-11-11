#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components
IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components/general
IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components/spi_devices
IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components/spi_devices/lcd
IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components/hmi
IOT_COMPONENT_DIRS += $(IOT_SOLUTION_PATH)/components/lv_examples

EXTRA_COMPONENT_DIRS += $(IOT_COMPONENT_DIRS)
