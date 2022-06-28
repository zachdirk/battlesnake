ADD_LIBRARY(core
  String.cpp String.h
  Vector.cpp Vector.h
)

TARGET_INCLUDE_DIRECTORIES(core
  PUBLIC
  ../
)