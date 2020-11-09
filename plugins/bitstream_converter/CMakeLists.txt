option(PL_BITSTREAM_CONVERTER "PL_BITSTREAM_CONVERTER" OFF)
if(PL_BITSTREAM_CONVERTER OR BUILD_ALL_PLUGINS)
    file(GLOB_RECURSE BITSTREAM_CONVERTER_INC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
    file(GLOB_RECURSE BITSTREAM_CONVERTER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE BITSTREAM_CONVERTER_PYTHON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/python/*.cpp)

    hal_add_plugin(bitstream_converter
                   SHARED
                   HEADER ${BITSTREAM_CONVERTER_INC}
                   SOURCES ${BITSTREAM_CONVERTER_SRC} ${BITSTREAM_CONVERTER_PYTHON_SRC}
                   )

    # Download and unpack fasm2bels at configure time
    configure_file(fasm2bels-CMakeLists.txt.in "${CMAKE_BINARY_DIR}/fasm2bels/CMakeLists.txt")
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
endif()
