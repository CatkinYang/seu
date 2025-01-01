add_subdirectory(pynq)
add_subdirectory(solver)

add_library(
  seu STATIC
  ${PROJECT_SOURCE_DIR}/src/task.cc ${PROJECT_SOURCE_DIR}/src/task_manager.cc
  ${PROJECT_SOURCE_DIR}/src/task_manager.cc ${PROJECT_SOURCE_DIR}/src/utils.cc
  ${ALL_OBJECT_FILES})

set(SEU_LIBS seu_solver seu_pynq)
