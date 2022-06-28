# chosen because it's what I have installed
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(BattleSnake)

# external libraries
ADD_SUBDIRECTORY(external)

# internal libraries
# this is ordered by dependency
ADD_SUBDIRECTORY(Core/Core)
ADD_SUBDIRECTORY(Server/Server)
ADD_SUBDIRECTORY(Battlesnake/Battlesnake)