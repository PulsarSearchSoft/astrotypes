if(CMAKE_BUILD_TYPE MATCHES documentation)
  set(ENABLE_DOC "true")
  set(doc_all_target "ALL")
else()
  set(doc_all_target "")
endif()
# Search for the DoxyfileAPI.in by looking through CMAKE_MODULE_PATH
find_file(DOXYFILE_IN "DoxyfileAPI.in" ${CMAKE_MODULE_PATH})

message("Doxyfile set to " ${DOXYFILE_IN})

if(ENABLE_DOC)
  # add a target to generate API documentation with Doxygen
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/doc)
    file(MAKE_DIRECTORY ${DOXYGEN_BUILD_DIR})
    configure_file(${DOXYFILE_IN} ${CMAKE_BINARY_DIR}/DoxyfileAPI @ONLY)
    add_custom_target(doc ${doc_all_target}
      ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/DoxyfileAPI
      WORKING_DIRECTORY ${DOXYGEN_BUILD_DIR}
      COMMENT "Generating documentation with Doxygen" VERBATIM)
    add_custom_target(publish_doc
      COMMAND git clone -b gh-pages git@github.com:PulsarSearchSoft/astrotypes.git gh-pages
      COMMAND cp -r ${DOXYGEN_BUILD_DIR}/html gh-pages/
      DEPENDS doc
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Publishing documentation on GitHub pages" VERBATIM)
    add_custom_command(TARGET publish_doc POST_BUILD
      COMMAND git add html
      COMMAND git commit -a -m "Updating documentation."
      COMMAND git push
      COMMAND rm -rf ${CMAKE_BINARY_DIR}/gh-pages
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gh-pages)
    install(DIRECTORY ${DOXYGEN_BUILD_DIR}/
            DESTINATION ${DOC_INSTALL_DIR}
            PATTERN "${DOXYGEN_BUILD_DIR}/*"
           )
   else(DOXYGEN_FOUND)
    add_custom_target(doc ${doc_all_target}
      COMMAND ${CMAKE_COMMAND} -E echo
      COMMENT "No doc target configured. Doxygen not found" VERBATIM
    )
   endif(DOXYGEN_FOUND)
else(ENABLE_DOC)
    add_custom_target(doc ${doc_all_target}
      COMMAND ${CMAKE_COMMAND} -E echo
      COMMENT "No doc target configured. Rebuild with -DENABLE_DOC=true" VERBATIM
    )
endif(ENABLE_DOC)
