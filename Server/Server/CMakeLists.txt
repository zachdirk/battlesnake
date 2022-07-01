ADD_LIBRARY(server
  Server.cpp Server.h
  ServerTypes.h
)

TARGET_LINK_LIBRARIES(server
  PUBLIC
  httplib::httplib
  rapidjson
  core
)

TARGET_INCLUDE_DIRECTORIES(server
  PUBLIC
  ../
)