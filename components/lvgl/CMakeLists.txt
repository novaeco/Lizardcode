set(LVGL_DIR ${CMAKE_CURRENT_LIST_DIR}/lvgl)
if(NOT EXISTS ${LVGL_DIR})
    message(FATAL_ERROR "LVGL source not found. Please add LVGL 9.2.2 to components/lvgl/lvgl")
endif()

idf_component_register(
    SRCS
        # Add LVGL source files here or use library's cmake
    INCLUDE_DIRS
        "${LVGL_DIR}"
)
