file(GLOB_RECURSE LIBRARY_FILES "src/*.cpp")

add_library(turbodbc_python ${LIBRARY_FILES})

set_target_properties(turbodbc_python PROPERTIES 
    PREFIX ""
    OUTPUT_NAME turbodbc_intern
    INSTALL_RPATH "../../.."
)

add_dependencies(turbodbc_python
    refresh_cmake_configuration
)

target_link_libraries(turbodbc_python
	${Boost_LIBRARIES}
	${Odbc_LIBRARIES}
	${PYTHON_LIBRARIES}
	cpp_odbc
	turbodbc
)

add_custom_command(TARGET turbodbc_python POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_BINARY_DIR}/turbodbc_intern.so ${CMAKE_BINARY_DIR})

install(
    TARGETS turbodbc_python
    LIBRARY DESTINATION lib
)

install(
	DIRECTORY turbodbc_python
	DESTINATION include
	FILES_MATCHING PATTERN "*.h"
	PATTERN ".git" EXCLUDE
)

install(
	DIRECTORY src/
	DESTINATION src/turbodbc_python
	FILES_MATCHING PATTERN "*.cpp"
	PATTERN ".git" EXCLUDE
)
