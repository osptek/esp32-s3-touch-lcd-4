set(LV_BIRD_DIR gui/bird)

file(GLOB_RECURSE LV_BIRD_SOURCES ${LV_BIRD_DIR}/*.c)

idf_component_register(
    SRCS "main.c" "lvgl_port.c" ${LV_BIRD_SOURCES}
    INCLUDE_DIRS "." ${LV_BIRD_DIR})

idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)   
