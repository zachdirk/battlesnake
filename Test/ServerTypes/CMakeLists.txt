ADD_EXECUTABLE(test_server_types
  TestServerTypes.cpp
)

TARGET_LINK_LIBRARIES(test_server_types
  PRIVATE
  core
  server
  gtest_main
)

TARGET_COMPILE_DEFINITIONS(test_server_types
  PRIVATE
  JSON_BASE_FOLDER=${CMAKE_CURRENT_SOURCE_DIR}/TestJSONs
)