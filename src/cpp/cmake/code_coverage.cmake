# Adapted from https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake

if(NOT CODE_COVERAGE_GUARD_VAR)
    set(CODE_COVERAGE_GUARD_VAR TRUE)
else()
    return()
endif()

if(CMAKE_BUILD_TYPE MATCHES profile)

    #============================================
    # coverage_target
    #
    # brief: function to generate a coverage report of the specified target
    #
    # usage: coverage_target(target)
    #
    # details: the target will be executed (e.g. 'make targetname') and coverage report generated for that launch
    #============================================
    function(coverage_target targetname)

        set(outputname coverage_${targetname})
        set(outputdir "coverage")
        set(coverage_target_general coverage_report_${targetname}) # Generic target name (collect from this and all subprojects)
        set(coverage_target coverage_report_${targetname}_${PROJECT_NAME}) # Target name specific to this project

        add_custom_target(${coverage_target}
                          COMMAND ${LCOV_EXE} --directory ${outputdir} --zerocounters # Clean up lcov
                          COMMAND ${CMAKE_MAKE_PROGRAM} ${targetname} ${ARGN} # Run the target
                          COMMAND ${LCOV_EXE} --directory . --capture --output-file ${outputdir}/${outputname}.info # Capture lcov counters and generate report
                          COMMAND ${LCOV_EXE} --remove ${outputdir}/${outputname}.info 'panda/*' 'test/*' '/usr/*' --output-file ${outputdir}/${outputname}.info.cleaned
                          COMMAND ${GENHTML_EXE} -o ${outputdir}/${outputname} ${outputdir}/${outputname}.info.cleaned
                          COMMAND ${CMAKE_COMMAND} -E remove ${outputdir}/${outputname}.info ${outputdir}/${outputname}.info.cleaned
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

        # Show info on where to find the report
        add_custom_command(TARGET ${coverage_target} POST_BUILD
                           COMMAND ;
                           COMMENT "see ${outputdir}/${outputname}/index.html for the coverage report."
        )

        if(NOT COVERAGE_TARGET_DEFINED_${coverage_target_general})
            # Define the general target
            set(COVERAGE_TARGET_DEFINED_${coverage_target_general} TRUE PARENT_SCOPE)
            add_custom_target(${coverage_target_general})
        endif()

        add_dependencies(${coverage_target_general} ${coverage_target})

    endfunction(coverage_target)

    # Find the coverage generators
    find_program(GCOV_EXE gcov)
    find_program(LCOV_EXE lcov)
    find_program(GENHTML_EXE genhtml)

    # Sanity checking
    if(NOT GCOV_EXE)
        message(FATAL_ERROR "gcov not found; specify with GCOV_EXE")
    endif()

    if(NOT LCOV_EXE)
        message(FATAL_ERROR "lcov not found; specify with LCOV_EXE")
    endif()

    if(NOT GENHTML_EXE)
        message(FATAL_ERROR "genhtml not found; specify with GENHTML_EXE")
    endif()

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

else()

    #============================================
    # coverage_target
    #
    # brief: function to generate a coverage report of the specified target
    #
    # usage: coverage_target(target)
    #
    # details: coverage is not available, output a suitable message if someone tries to build the target
    #============================================
    function(coverage_target targetname)
        set(coverage_target coverage_report_${targetname})
        if(NOT COVERAGE_TARGET_DEFINED_${coverage_target})
            set(COVERAGE_TARGET_DEFINED_${coverage_target} TRUE PARENT_SCOPE)
            add_custom_target(${coverage_target}
                              COMMENT "target ${coverage_target} requires a profile build. Did you forget to set CMAKE_BUILD_TYPE=profile?"
            )
        endif()
    endfunction(coverage_target)

endif()

coverage_target(test)
