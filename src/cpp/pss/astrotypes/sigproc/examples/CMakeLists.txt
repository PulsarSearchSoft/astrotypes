set(examples_src
    src/ExtendedHeader.cpp
   )

add_library("sigproc_examples" ${examples_src})
add_executable("sigproc_cat" src/sigproc_cat.cpp)
target_link_Libraries(sigproc_cat)
