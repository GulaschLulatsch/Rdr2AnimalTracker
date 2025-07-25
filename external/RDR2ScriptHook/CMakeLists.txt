cmake_minimum_required(VERSION 3.15)

set(TARGET_NAME ScriptHookRDR2)

project(${TARGET_NAME})

install(FILES lib/${TARGET_NAME}.lib DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${TARGET_NAME}-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-config.cmake"
	INSTALL_DESTINATION lib/cmake/${TARGET_NAME}
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-config.cmake"
	DESTINATION lib/cmake/${TARGET_NAME}
)