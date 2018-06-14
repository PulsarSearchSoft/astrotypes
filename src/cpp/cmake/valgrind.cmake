# -- find valgrind
find_program( VALGRIND_PATH valgrind )

if(NOT VALGRIND_PATH) 
    # add a valgrind not installed message for the target
    macro(VALGRIND_TARGET _targetname )
        set(valgrind_target valgrind_report_${_targetname})
        if(NOT _valgrind_target_exists_${valgrind_target})
            ADD_CUSTOM_TARGET(${valgrind_target}
                COMMENT "target ${valgrind_target} requires valgrind to be available"
            )
            set(_valgrind_target_exists_${valgrind_target} true)
        endif()
    endmacro(VALGRIND_TARGET)
else(NOT VALGRIND_PATH) 
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/valgrind)
    macro(VALGRIND_TARGET _targetname )
        set(_outputdir valgrind)
        set(_outputname valgrind_${_targetname})
        set(valgrind_target valgrind_report_${_targetname})
        if(NOT _valgrind_target_exists_${valgrind_target})
            set(_valgrind_target_exists_${valgrind_target} true)
            ADD_CUSTOM_TARGET(${valgrind_target})
        endif(NOT _valgrind_target_exists_${valgrind_target})
        set(valgrind_target_local valgrind_report_${_targetname}_${PROJECT_NAME})
        ADD_CUSTOM_TARGET(${valgrind_target_local}
            COMMAND ${VALGRIND_PATH} --xml=yes --xml-file=${_outputdir}/${_outputname} --trace-children=yes ${CMAKE_MAKE_PROGRAM} ${_targetname}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        ADD_CUSTOM_COMMAND(TARGET ${valgrind_target_local} POST_BUILD
            COMMAND ;
            COMMENT "see ${_outputdir}/${_outputname} for the valgrind report."
        )
        ADD_DEPENDENCIES(${valgrind_target} ${valgrind_target_local})
    endmacro(VALGRIND_TARGET)
endif(NOT VALGRIND_PATH) 


# -- targets
valgrind_target(test)
