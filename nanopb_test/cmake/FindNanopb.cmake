# FindNanoPb.cmake - Try to find nanopb


if(APPLE)
  set(NANOPB_DOWNLOAD_LINK https://jpa.kapsi.fi/nanopb/download/nanopb-0.3.7-macosx-x86.tar.gz)
  set(NANOPB_ARCHIVE nanopb-0.3.7-macosx-x86.tar.gz)
  set(NANOPB_DIR nanopb-0.3.7-macosx-x86)
elseif(WIN32)
  set(NANOPB_DOWNLOAD_LINK https://jpa.kapsi.fi/nanopb/download/nanopb-0.3.7-windows-x86.zip)
  set(NANOPB_ARCHIVE nanopb-0.3.7-windows-x86.zip)
  set(NANOPB_DIR nanopb-0.3.7-windows-x86)
else()
  set(NANOPB_DOWNLOAD_LINK https://jpa.kapsi.fi/nanopb/download/nanopb-0.3.7-linux-x86.tar.gz)
  set(NANOPB_ARCHIVE nanopb-0.3.7-linux-x86.tar.gz)
  set(NANOPB_DIR nanopb-0.3.7-linux-x86)
endif()

set(NANOPB_ARCHIVE_PATH ${CMAKE_BINARY_DIR}/${NANOPB_ARCHIVE})
set(NANOPB_EXTRACT_PATH ${CMAKE_BINARY_DIR}/${NANOPB_DIR})

if(EXISTS ${NANOPB_ARCHIVE_PATH})
  message(STATUS "Nanopb archive already in build folder")
else()
  file(DOWNLOAD
      ${NANOPB_DOWNLOAD_LINK}
      ${NANOPB_ARCHIVE_PATH}
      SHOW_PROGRESS
    )
endif()

if(EXISTS ${NANOPB_EXTRACT_PATH})
  message(STATUS "Nanopb archive already extracted")
else()
  if(WIN32)
    message(ERROR "Not yet supported")
  else()
    message(STATUS "Extracting nanopb")
    execute_process(COMMAND tar -xf ${NANOPB_ARCHIVE_PATH})
  endif()
endif()

set(NANOPB_INCLUDE_DIR ${NANOPB_EXTRACT_PATH})
set(NANOPB_SRC ${NANOPB_EXTRACT_PATH}/pb_common.c ${NANOPB_EXTRACT_PATH}/pb_decode.c ${NANOPB_EXTRACT_PATH}/pb_encode.c)
set(NANOPB_PROTOC ${NANOPB_EXTRACT_PATH}/generator-bin/protoc)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Nanopb DEFAULT_MSG NANOPB_INCLUDE_DIR NANOPB_SRC NANOPB_PROTOC)

MARK_AS_ADVANCED(NANOPB_INCLUDE_DIR NANOPB_SRC NANOPB_PROTOC)