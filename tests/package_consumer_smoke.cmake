if(NOT DEFINED source_dir OR NOT DEFINED binary_dir OR NOT DEFINED project_build_dir OR NOT DEFINED package_subdir)
    message(FATAL_ERROR "source_dir, binary_dir, project_build_dir, and package_subdir must be provided")
endif()

file(REMOVE_RECURSE "${binary_dir}")
set(install_prefix "${binary_dir}/prefix")
set(package_dir "${install_prefix}/${package_subdir}")

execute_process(
    COMMAND "${CMAKE_COMMAND}" --install "${project_build_dir}" --prefix "${install_prefix}"
    RESULT_VARIABLE install_result
)
if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Package install step failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -S "${source_dir}" -B "${binary_dir}" -Dneurodraw_DIR=${package_dir}
    RESULT_VARIABLE configure_result
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Package consumer configure failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${binary_dir}"
    RESULT_VARIABLE build_result
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Package consumer build failed")
endif()
