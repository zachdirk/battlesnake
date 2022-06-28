#include "Server/Server.h"

using namespace Battlesnake;

int main(int argc, char** argv)
{
  Server::Server server;
  server.Init("0.0.0.0", 8080);
  return 0;
}