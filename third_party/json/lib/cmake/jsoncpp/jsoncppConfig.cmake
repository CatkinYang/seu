
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was jsoncppConfig.cmake.meson.in ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################



add_library(jsoncpp_lib IMPORTED SHARED)
set_target_properties(jsoncpp_lib PROPERTIES
  IMPORTED_LOCATION "${PACKAGE_PREFIX_DIR}/lib/libjsoncpp.26.dylib"
  INTERFACE_INCLUDE_DIRECTORIES "${PACKAGE_PREFIX_DIR}/include")


include ( "${CMAKE_CURRENT_LIST_DIR}/jsoncpp-namespaced-targets.cmake" )
