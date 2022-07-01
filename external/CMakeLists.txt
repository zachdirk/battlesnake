# sorted alphabetically
ADD_SUBDIRECTORY(cpp-httplib)
ADD_SUBDIRECTORY(googletest)
ADD_SUBDIRECTORY(rapidjson)
# why on earth does rapidjson not do this itself?
ADD_LIBRARY(rapidjson INTERFACE)
TARGET_INCLUDE_DIRECTORIES(rapidjson INTERFACE ./rapidjson/include)
