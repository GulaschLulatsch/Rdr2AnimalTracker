
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ScriptHookRDR2-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

if(NOT TARGET ScriptHookRDR2::ScriptHookRDR2)
	add_library(ScriptHookRDR2::ScriptHookRDR2 STATIC IMPORTED GLOBAL)
	set_target_properties(ScriptHookRDR2::ScriptHookRDR2 PROPERTIES
		IMPORTED_LOCATION "${PACKAGE_PREFIX_DIR}/lib/ScriptHookRDR2.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${PACKAGE_PREFIX_DIR}/include"
	)
endif()
