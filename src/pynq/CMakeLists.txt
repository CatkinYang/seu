add_library(seu_pynq OBJECT milp_model_pynq_with_partition.cc pynq.cc
                            pynq_fine_grained.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:seu_pynq>
    PARENT_SCOPE)
