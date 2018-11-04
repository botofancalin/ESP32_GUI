CSRCS += lcd_master.c

DEPPATH += --dep-path lv_drivers/display
VPATH += :lv_drivers/display

CFLAGS += "-I$(LVGL_DIR)/lv_drivers/display"
