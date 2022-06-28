#include "Server.h"

#include "httplib.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Core/String.h"
#include "Core/Vector.h"

#include <thread>

namespace Battlesnake
{
namespace Server
{
struct BattlesnakeInfo
{
  Core::String apiversion;
  Core::String author;
  // Americans
  Core::String color;
  Core::String head;
  Core::String tail;
  Core::String version;

  void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
  {
    writer.StartObject();
    writer.Key("apiversion");
    writer.String(apiversion.c_str());
    writer.Key("author");
    writer.String(author.c_str());
    writer.Key("color");
    writer.String(color.c_str());
    writer.Key("head");
    writer.String(head.c_str());
    writer.Key("tail");
    writer.String(tail.c_str());
    writer.Key("version");
    writer.String(version.c_str());
    writer.EndObject();
  }
};
struct Settings
{
  size_t foodSpawnChance;
  size_t minimumFood;
  size_t hazardDamagePerTurn;
  size_t royaleShrinkEveryNTurns;
  size_t squadAllowBodyCollisions;
  size_t squadSharedElimination;
  size_t squadSharedHealth;
  size_t squadSharedLength;
};
struct Ruleset
{
  Core::String name;
  Core::String version;
  Settings settings;
};
struct GameInfo
{
  Core::String gameId;
  Ruleset rules;
  Core::String map;
  size_t timeout;
  Core::String source;
};
struct Coordinate
{
  size_t x;
  size_t y;
};
struct Snake
{
  
};
struct Board
{
  size_t height;
  size_t width;
  Core::Vector<Coordinate> food;
  Core::Vector<Coordinate> hazards;
};
struct GameStartInfo
{
  GameInfo gameInfo;
  size_t turn;

};

using Request = httplib::Request;
using Response = httplib::Response;
// Server
Server::Server()
{
}
Server::~Server() = default;

void Server::Init(Core::String url, size_t port)
{
  mServer.Get("/Battlesnake", [](const Request& req, Response& res)
  {
    BattlesnakeInfo info{"1", "Jozrozroz", "#FF0000", "gamer", "pixel", "0.1.0"};
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> w(s);
    info.Serialize(w);
    res.set_content(s.GetString(), "application/json");
  });
  mServer.Post("/Battlesnake/Start", [](const Request& req, Response& res)
  {
    res.set_content("yo", "application/json");
  });
  mServer.Post("/Battlesnake/Move", [](const Request& req, Response& res)
  {
    res.set_content("yo", "application/json");
  });
  mServer.Post("/Battlesnake/End", [](const Request& req, Response& res)
  {
    res.set_content("yo", "application/json");
  });

  mServer.listen(url.c_str(), port);
}
}
}