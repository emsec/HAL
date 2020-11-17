option(PL_BITSTREAM_PARSER "PL_BITSTREAM_PARSER" ON)
if(PL_BITSTREAM_PARSER OR BUILD_ALL_PLUGINS)
    file(GLOB_RECURSE BITSTREAM_PARSER_INC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
    file(GLOB_RECURSE BITSTREAM_PARSER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

    hal_add_plugin(bitstream_parser
                   SHARED
                   HEADER ${BITSTREAM_PARSER_INC}
                   SOURCES ${BITSTREAM_PARSER_SRC}
                   )
endif()
