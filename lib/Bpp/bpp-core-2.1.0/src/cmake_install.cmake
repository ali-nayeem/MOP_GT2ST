# Install script for directory: /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src/libbpp-core.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so.2.0.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src/libbpp-core.so.2.0.3"
    "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src/libbpp-core.so.2"
    "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src/libbpp-core.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so.2.0.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbpp-core.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bpp" TYPE DIRECTORY FILES "/home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/src/Bpp/" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  EXECUTE_PROCESS(COMMAND /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-core-2.1.0/genIncludes.sh /home/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/include/Bpp)
endif()

