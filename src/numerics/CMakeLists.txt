target_sources(mast
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/basis_matrix.cpp
        ${CMAKE_CURRENT_LIST_DIR}/basis_matrix.h
        ${CMAKE_CURRENT_LIST_DIR}/fem_operator_matrix.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fem_operator_matrix.h
        ${CMAKE_CURRENT_LIST_DIR}/lapack_dgeev_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lapack_dgeev_interface.h
        ${CMAKE_CURRENT_LIST_DIR}/lapack_dggev_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lapack_dggev_interface.h
        ${CMAKE_CURRENT_LIST_DIR}/lapack_zggev_base.h
        ${CMAKE_CURRENT_LIST_DIR}/lapack_zggev_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lapack_zggev_interface.h
        ${CMAKE_CURRENT_LIST_DIR}/lapack_zggevx_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lapack_zggevx_interface.h
        ${CMAKE_CURRENT_LIST_DIR}/utility.h)

# Install MAST headers for this directory.
install(DIRECTORY ./ DESTINATION include/numerics
        FILES_MATCHING PATTERN "*.h")