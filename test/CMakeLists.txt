function(add_python_test_target name test_script args comment)
  set(PYTHON_TEST_COMMAND
    ${PYTHON_EXECUTABLE}
    ${test_script}
    ${args}
    )

  add_custom_target(${name}
    COMMAND ${PYTHON_TEST_COMMAND} ${ARG_DEFAULT_ARGS}
    COMMENT "${comment}"
    )
endfunction()

if ("${LLDB_TEST_COMPILER}" STREQUAL "")
    string(REGEX REPLACE ".*ccache\ +" "" LLDB_TEST_COMPILER ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1})
endif()

# The default architecture with which to compile test executables is the default LLVM target
# architecture, which itself defaults to the host architecture.
string(TOLOWER "${LLVM_TARGET_ARCH}" LLDB_DEFAULT_TEST_ARCH)
if( LLDB_DEFAULT_TEST_ARCH STREQUAL "host" )
  string(REGEX MATCH "^[^-]*" LLDB_DEFAULT_TEST_ARCH ${LLVM_HOST_TRIPLE})
endif ()

# Allow the user to override the default by setting LLDB_TEST_ARCH
set(LLDB_TEST_ARCH
	${LLDB_DEFAULT_TEST_ARCH}
	CACHE STRING "Specify the architecture to run LLDB tests as (x86|x64).  Determines whether tests are compiled with -m32 or -m64")

# Users can override LLDB_TEST_USER_ARGS to specify arbitrary arguments to pass to the script
set(LLDB_TEST_USER_ARGS
  -C ${LLDB_TEST_COMPILER}
  CACHE STRING "Specify additional arguments to pass to test runner. For example: '-C gcc -C clang -A i386 -A x86_64'")

set(LLDB_TEST_COMMON_ARGS
  --arch=${LLDB_TEST_ARCH}
  --executable
  ${CMAKE_BINARY_DIR}/bin/lldb${CMAKE_EXECUTABLE_SUFFIX}
  -s
  ${CMAKE_BINARY_DIR}/lldb-test-traces
  -u CXXFLAGS
  -u CFLAGS
  )

add_python_test_target(check-lldb-single
  ${LLDB_SOURCE_DIR}/test/dotest.py
  "${LLDB_TEST_COMMON_ARGS};${LLDB_TEST_USER_ARGS}"
  "Testing LLDB with args: ${LLDB_TEST_COMMON_ARGS};${LLDB_TEST_USER_ARGS}"
  )

set(LLDB_DOSEP_ARGS -o;\"-q;${LLDB_TEST_COMMON_ARGS};${LLDB_TEST_USER_ARGS}\")

# If tests crash cause LLDB to crash, or things are otherwise unstable, or if machine-parsable
# output is desired (i.e. in continuous integration contexts) check-lldb-sep is a better target.
add_python_test_target(check-lldb
  ${LLDB_SOURCE_DIR}/test/dosep.py
  "${LLDB_DOSEP_ARGS}"
  "Testing LLDB (with a separate subprocess per test)"
  )
