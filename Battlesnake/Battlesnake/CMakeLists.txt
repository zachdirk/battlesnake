ADD_EXECUTABLE(battlesnake
  battlesnake.cpp
)

TARGET_LINK_LIBRARIES(battlesnake
  httplib::httplib
)