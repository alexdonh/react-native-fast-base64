cmake_minimum_required(VERSION 3.9.0)

project(fastbase64)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

include_directories(
  "${NODE_MODULES_DIR}/react-native/React"
  "${NODE_MODULES_DIR}/react-native/React/Base"
  "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
  ../cpp
)

add_library(fastbase64
  SHARED
  "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/jsi.cpp"
  ../cpp/Base64.cpp
  ../cpp/Base64.h
  ../cpp/react-native-fast-base64.cpp
  ../cpp/react-native-fast-base64.h
  ./cpp-adapter.cpp
)

target_link_libraries(fastbase64)
