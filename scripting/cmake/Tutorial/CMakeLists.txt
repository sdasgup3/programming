cmake_minimum_required  (VERSION 3.6.1)
project (Tutorial)
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")


# should we use our own math functions?
option (USE_MYMATH 
        "Use tutorial provided math implementation" ON)


if(USE_MYMATH) 
  #We also add another include directory so that the MathFunctions/mysqrt.h 
  #header file can be found for the function prototype
  include_directories("${PROJECT_SOURCE_DIR}/MathFunctions")
  add_subdirectory (MathFunctions) 
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)



message(warning ${PROJECT_BINARY_DIR})

add_executable (Tutorial Tutorial.cxx )
target_link_libraries (Tutorial ${EXTRA_LIBS})


install (TARGETS Tutorial DESTINATION bin)
install (FILES ${PROJECT_BINARY_DIR}/TutorialConfig.h DESTINATION include)
