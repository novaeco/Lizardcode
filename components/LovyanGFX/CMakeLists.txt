set(LGFX_ROOT ${CMAKE_CURRENT_LIST_DIR}/lib)
# Include the LovyanGFX ESP‑IDF configuration. This script sets the component
# sources, include directories and registers the component with ESP‑IDF via the
# internal `register_component` call. Calling `idf_component_register` here would
# create the component twice and results in a CMake error.
include(${LGFX_ROOT}/boards.cmake/esp-idf.cmake)
