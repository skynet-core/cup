execute_process(
  COMMAND dpkg-shlibdeps -O ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/usr/bin/myapp
  OUTPUT_VARIABLE deps)
string(REGEX REPLACE ".*shlibs:Depends=" "" deps "${deps}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "${deps}"
    PARENT_SCOPE)
