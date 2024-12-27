add_library(seu_solver OBJECT kmeanspp.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:seu_solver>
    PARENT_SCOPE)
