target_sources(mast
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/filter_base.cpp
        ${CMAKE_CURRENT_LIST_DIR}/filter_base.h
        ${CMAKE_CURRENT_LIST_DIR}/indicator_function_constrain_dofs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/indicator_function_constrain_dofs.h
        ${CMAKE_CURRENT_LIST_DIR}/interface_dof_handler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/interface_dof_handler.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_boundary_velocity.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_boundary_velocity.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_constrain_dofs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_constrain_dofs.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_discipline.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_discipline.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_eigenproblem_assembly.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_eigenproblem_assembly.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_elem_base.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_elem_base.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_intersected_elem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_intersected_elem.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_intersection.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_intersection.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_perimeter_output.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_perimeter_output.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_nonlinear_implicit_assembly.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_nonlinear_implicit_assembly.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_reinitialization_transient_assembly.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_reinitialization_transient_assembly.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_system_initialization.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_system_initialization.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_transient_assembly.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_transient_assembly.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_void_solution.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_void_solution.h
        ${CMAKE_CURRENT_LIST_DIR}/level_set_volume_output.cpp
        ${CMAKE_CURRENT_LIST_DIR}/level_set_volume_output.h
        ${CMAKE_CURRENT_LIST_DIR}/material_patch.cpp
        ${CMAKE_CURRENT_LIST_DIR}/material_patch.h
        ${CMAKE_CURRENT_LIST_DIR}/sub_cell_fe.cpp
        ${CMAKE_CURRENT_LIST_DIR}/sub_cell_fe.h)

# Install MAST headers for this directory.
install(DIRECTORY ./ DESTINATION include/level_set
        FILES_MATCHING PATTERN "*.h")
