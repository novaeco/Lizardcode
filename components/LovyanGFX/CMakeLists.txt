set(LGFX_ROOT ${CMAKE_CURRENT_LIST_DIR}/lib)
include(${LGFX_ROOT}/boards.cmake/esp-idf.cmake)

idf_component_register(
    SRCS ${COMPONENT_SRCS}
    INCLUDE_DIRS "." ${COMPONENT_ADD_INCLUDEDIRS}
    REQUIRES ${COMPONENT_REQUIRES}
)
