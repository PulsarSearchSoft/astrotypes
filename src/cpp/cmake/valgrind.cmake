if(NOT VALGRIND_GUARD_VAR)
    set(VALGRIND_GUARD_VAR TRUE)
else()
    return()
endif()

# Find valgrind
find_program(VALGRIND_EXE valgrind)

if(NOT VALGRIND_EXE)
    #================================================
    # valgrind_target
    #
    # brief: Function to report a "valgrind not installed" message for the target
    #
    # usage: valgrind_target(target)
    #================================================
    function(valgrind_target targetname)
        set(valgrind_target valgrind_report_${targetname})
        if(NOT VALGRIND_TARGET_EXISTS_${valgrind_target})
            add_custom_target(${valgrind_target}
                              COMMENT "target ${valgrind_target} requires valgrind to be available"
            )
            set(VALGRIND_TARGET_EXISTS_${valgrind_target} TRUE PARENT_SCOPE)
        endif()
    endfunction(valgrind_target)
else()
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/valgrind)
    #================================================
    # valgrind_target
    #
    # brief: Function to generate a valgrind report for the target
    #
    # usage: valgrind_target(target)
    #================================================
    function(valgrind_target targetname)
        set(outputdir valgrind)
        set(outputname valgrind_${targetname})
        set(valgrind_target valgrind_report_${targetname})
        if(NOT VALGRIND_TARGET_EXISTS_${valgrind_target})
            set(VALGRIND_TARGET_EXISTS_${valgrind_target} TRUE PARENT_SCOPE)
            add_custom_target(${valgrind_target})
        endif()
        set(valgrind_target_local valgrind_report_${targetname}_${PROJECT_NAME})
        add_custom_target(${valgrind_target_local}
                          COMMAND ${VALGRIND_EXE} --xml=yes --xml-file=${outputdir}/${outputname} --trace-children=yes ${CMAKE_MAKE_PROGRAM} ${targetname}
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        add_custom_command(TARGET ${valgrind_target_local} POST_BUILD
                           COMMAND ;
                           COMMENT "see ${outputdir}/${outputname} for the valgrind report."
        )
        add_dependencies(${valgrind_target} ${valgrind_target_local})
    endfunction(valgrind_target)
endif()

valgrind_target(test)
