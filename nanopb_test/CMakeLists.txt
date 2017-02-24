cmake_minimum_required (VERSION 2.8)

project (nanopb_test)

#
# The path for our own CMake modules
#
set ( CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake
)


find_package(LibUSB REQUIRED)
message(STATUS "libusb inc: " ${LIBUSB_INCLUDE_DIR})
message(STATUS "libusb lib: " ${LIBUSB_LIBRARY})

include_directories(${LIBUSB_INCLUDE_DIR} proto/nanopb)


add_executable(nanopb_test main.cpp proto/nanopb/pb_common.c proto/nanopb/pb_decode.c proto/protocols.pb.c)
target_link_libraries(nanopb_test  ${LIBUSB_LIBRARY})
target_link_libraries(nanopb_test "-framework CoreFoundation")
target_link_libraries(nanopb_test "-framework IOKit")
target_link_libraries(nanopb_test "-lobjc")
