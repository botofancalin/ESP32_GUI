#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

ifdef CONFIG_LVGL_GUI_ENABLE

    ifndef CONFIG_LVGL_USE_CUSTOM_DRIVER
        COMPONENT_SRCDIRS := 
        COMPONENT_ADD_INCLUDEDIRS := . \
            ./include \

        #Display driver
        ifdef CONFIG_LVGL_DRIVER_ILI9341
        COMPONENT_SRCDIRS += ./gdisp/ILI9341
        COMPONENT_ADD_INCLUDEDIRS += ./gdisp/ILI9341
        endif

        #Input driver 
        ifdef CONFIG_LVGL_DRIVER_TOUCH_SCREEN_ENABLE

            ifdef CONFIG_LVGL_DRIVER_TOUCH_XPT2046
            COMPONENT_SRCDIRS += ./ginput/XPT2046
            COMPONENT_ADD_INCLUDEDIRS += ./ginput/XPT2046
            endif
            
        endif
            
    endif

else
    COMPONENT_SRCDIRS := 
    COMPONENT_ADD_INCLUDEDIRS := 
endif 
