cmake_minimum_required (VERSION 2.8)

project (nanopb_test)

#
# The path for our own CMake modules
#
set ( CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake
)


find_package(LibUSB REQUIRED)
find_package(Nanopb REQUIRED)
message(STATUS "libusb inc: " ${LIBUSB_INCLUDE_DIR})
message(STATUS "libusb lib: " ${LIBUSB_LIBRARY})

include_directories(${LIBUSB_INCLUDE_DIR} ${NANOPB_INCLUDE_DIR})

add_custom_target(
    ProtoC
    COMMAND ${NANOPB_PROTOC} -I=${CMAKE_CURRENT_SOURCE_DIR}/proto/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/protocols.proto 
    --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto/
)

set_source_files_properties(proto/protocols.pb.c proto/protocols.pb.h PROPERTIES GENERATED TRUE)

add_executable(nanopb_test main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/proto/protocols.pb.c ${NANOPB_SRC})

target_link_libraries(nanopb_test  ${LIBUSB_LIBRARY})
add_dependencies(nanopb_test ProtoC)
target_link_libraries(nanopb_test "-framework CoreFoundation")
target_link_libraries(nanopb_test "-framework IOKit")
target_link_libraries(nanopb_test "-lobjc")
