#include "Server.h"

#include "httplib.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Core/String.h"
#include "Core/Vector.h"
#include "Server/ServerTypes.h"

#include <thread>
#include <random>

namespace Battlesnake
{
namespace Server
{
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
    std::cerr << "Received base request!" << std::endl;
    SnakeCustomizations info{"1", "Jozrozroz", "#FF0000", "gamer", "pixel", "0.1.0"};
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> w(s);
    info.Serialize(w);
    res.set_content(s.GetString(), "application/json");
  });
  mServer.Post("/Battlesnake/start", [](const Request& req, Response& res)
  {
    std::cerr << "Received start request!" << std::endl;
    rapidjson::Document d;
    d.Parse(req.body.c_str());
    GameState game;
    game.Deserialize(d);
  });
  mServer.Post("/Battlesnake/move", [](const Request& req, Response& res)
  {
    std::cerr << "Received move request!" << std::endl;
    rapidjson::Document d;
    d.Parse(req.body.c_str());
    GameState game;
    game.Deserialize(d);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist4(1,4);
    auto rand = dist4(rng);
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> w(s);
    switch(rand)
    {
      case 1: 
      {
        Move m{"up", "going up"};
        m.Serialize(w);
        break;
      }
      case 2:
      {
        Move m{"left", "going left"};
        m.Serialize(w);
        break;
      }
      case 3:
      {
        Move m{"right", "going right"};
        m.Serialize(w);
        break;
      }
      case 4:
      {
        Move m{"down", "going down"};
        m.Serialize(w);
        break;
      }
    };
    res.set_content(s.GetString(), "application/json");
  });
  mServer.Post("/Battlesnake/end", [](const Request& req, Response& res)
  {
    std::cerr << "Received end request!" << std::endl;
    rapidjson::Document d;
    d.Parse(req.body.c_str());
    GameState game;
    game.Deserialize(d);
  });

  mServer.listen(url.c_str(), port);
}
}
}