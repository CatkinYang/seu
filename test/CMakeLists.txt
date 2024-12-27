cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE SEU_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/test_*.cc")

# ##############################################################################
# MAKE TARGETS
# ##############################################################################

add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)

# ##############################################################################
# "make XYZ_test"
# ##############################################################################
message(STATUS "SEU_TEST_SOURCES: ${SEU_TEST_SOURCES}")
foreach(seu_test_source ${SEU_TEST_SOURCES})
  # Create a human readable name.
  get_filename_component(seu_test_filename ${seu_test_source} NAME)
  string(REPLACE ".cc" "" seu_test_name ${seu_test_filename})

  # Add the test target separately and as part of "make check-tests".
  # add_executable(${seu_test_name} EXCLUDE_FROM_ALL)
  add_executable(${seu_test_name} ${seu_test_source})
  # add_backward(${seu_test_name})
  add_dependencies(build-tests ${seu_test_name})

  target_link_libraries(${seu_test_name} seu jsoncpp_lib ${GUROBI_LIBRARIES})

  # Set test target properties and dependencies.
  set_target_properties(
    ${seu_test_name}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test"
               COMMAND ${seu_test_name})
endforeach()
