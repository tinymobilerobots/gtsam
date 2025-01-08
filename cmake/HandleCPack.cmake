#JLBC: is all this actually used by someone? could it be removed?

# Flags for choosing default packaging tools
set(CPACK_SOURCE_GENERATOR "TGZ" CACHE STRING "CPack Default Source Generator")
set(CPACK_GENERATOR        "TGZ" CACHE STRING "CPack Default Binary Generator")

###############################################################################
# Set up CPack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GTSAM")
set(CPACK_PACKAGE_VENDOR "Frank Dellaert, Georgia Institute of Technology")
set(CPACK_PACKAGE_CONTACT "Frank Dellaert, dellaert@cc.gatech.edu")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${GTSAM_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GTSAM_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${GTSAM_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
#set(CPACK_INSTALLED_DIRECTORIES "doc;.") # Include doc directory
#set(CPACK_INSTALLED_DIRECTORIES ".") # FIXME: throws error
set(CPACK_SOURCE_IGNORE_FILES "/build*;/\\\\.;/makestats.sh$")
set(CPACK_SOURCE_IGNORE_FILES "${CPACK_SOURCE_IGNORE_FILES}" "/gtsam_unstable/")
set(CPACK_SOURCE_IGNORE_FILES "${CPACK_SOURCE_IGNORE_FILES}" "/package_scripts/")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "gtsam-${GTSAM_VERSION_MAJOR}.${GTSAM_VERSION_MINOR}.${GTSAM_VERSION_PATCH}")
#set(CPACK_SOURCE_PACKAGE_FILE_NAME "gtsam-aspn${GTSAM_VERSION_PATCH}") # Used for creating ASPN tarballs

# Deb-package specific cpack
set(CPACK_DEBIAN_PACKAGE_NAME "libgtsam-dev")
if(GTSAM_ENABLE_BOOST_SERIALIZATION OR GTSAM_USE_BOOST_FEATURES)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev (>= 1.65), libboost-serialization-dev (>= 1.65), libboost-system-dev (>= 1.65), libboost-filesystem-dev (>= 1.65), libboost-thread-dev (>= 1.65), libboost-program-options-dev (>= 1.65), libboost-date-time-dev (>= 1.65), libboost-timer-dev (>= 1.65), libboost-chrono-dev (>= 1.65), libboost-regex-dev (>= 1.65), libeigen3-dev")
endif()

# TMR Specific settings
set(DISTRO_CODENAME $ENV{DISTRO_CODENAME})
find_program(LSB_RELEASE_CMD lsb_release)
mark_as_advanced(LSB_RELEASE_CMD)
if(LSB_RELEASE_CMD)
  execute_process(
    COMMAND "${LSB_RELEASE_CMD}" --codename --short
    OUTPUT_VARIABLE DISTRO_CODENAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
if(DISTRO_CODENAME)
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}~${DISTRO_CODENAME}")
else()
  set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
  message(STATUS "Could not find lsb_release nor is DISTRO_CODENAME set.")
endif()
set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "DEB")

#set(CPACK_PACKAGING_PREFIX /usr/local)
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Magnus Aagaard")
set(CPACK_PACKAGE_NAME gtsam-tmr)
set(CPACK_DEBIAN_PACKAGE_NAME "libgtsam-tmr")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/tmr_build/triggers")
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
