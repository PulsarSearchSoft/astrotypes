set(module_sigproc_lib_src_cpu
    src/Header.cpp
    src/HeaderField.cpp
    src/SigProcVariable.cpp
    PARENT_SCOPE
)

add_subdirectory(test)
add_subdirectory(examples)
