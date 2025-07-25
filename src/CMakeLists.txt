set(TARGET_NAME AnimalTracker)

set(TARGET_INTERFACE
	IContainedInfo.h
	IContainingInfo.h
	IInfo.h
	IInfoPersister.h
	IMenu.h
	IMenuItem.h
	ISandwichedInfo.h
)

set(TARGET_HEADER
	AnimalInfo.h
	AnimalMenu.h
	AnimalTracker.h
	Categories.h
	CategoryInfo.h
	CategoryMenu.h
	ColorRgba.h
	CreateMap.h
	DrawUtils.h
	EMenuItemType.h
	IniOptions.h
	Keyboard.h
	MenuBase.h
	MenuController.h
	MenuInput.h
	MenuItemAnimal.h
	MenuItemBase.h
	MenuItemCategory.h
	MenuItemDefault.h
	MenuItemMenu.h
	MenuItemTitle.h
	NamesInfo.h
	QualityFilter.h
	StringComparator.h
)

set(TARGET_SOURCE
	AnimalInfo.cpp
	AnimalMenu.cpp
	AnimalTracker.cpp
	CategoryInfo.cpp
	CategoryMenu.cpp
	ColorRgba.cpp
	DrawUtils.cpp
	IniOptions.cpp
	Keyboard.cpp
	main.cpp
	MenuBase.cpp
	MenuController.cpp
	MenuInput.cpp
	MenuItemAnimal.cpp
	MenuItemBase.cpp
	MenuItemCategory.cpp
	MenuItemDefault.cpp
	MenuItemMenu.cpp
	MenuItemTitle.cpp
	QualityFilter.cpp
	StringComparator.cpp
)

list(TRANSFORM TARGET_INTERFACE PREPEND "include/${TARGET_NAME}/")
list(TRANSFORM TARGET_HEADER PREPEND "include/${TARGET_NAME}/")

source_group("Interface Files" FILES ${TARGET_INTERFACE})

add_library(${TARGET_NAME} SHARED)
set_target_properties(${TARGET_NAME} 
	PROPERTIES 
		SUFFIX ".asi"
		LINKER_LANGUAGE CXX
)

target_compile_options(${TARGET_NAME} 
	PRIVATE
		/Zc:preprocessor # required for extra macro argument for better-enums
)
target_sources(${TARGET_NAME}
	PRIVATE
		${TARGET_SOURCE}
	PRIVATE
		FILE_SET header_files
		TYPE HEADERS
		BASE_DIRS include
		FILES ${TARGET_HEADER} ${TARGET_INTERFACE}
)

target_compile_definitions(${TARGET_NAME}
	PRIVATE
		NOMINMAX
		NODRAWTEXT
		WIN32_LEAN_AND_MEAN
		SI_CONVERT_ICU
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
		ScriptHookRDR2::ScriptHookRDR2
		SimpleIni::SimpleIni
		better-enums::better-enums
		spdlog::spdlog 
		icu::icu 
)

target_include_directories(${TARGET_NAME} PRIVATE    
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_NAME}>
	$<INSTALL_INTERFACE:include/${TARGET_NAME}>  
)

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION "."
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/AnimalTracker/
	DESTINATION AnimalTracker
)