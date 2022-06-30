#include "Server.h"

#include "httplib.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Core/String.h"
#include "Core/Vector.h"

#include <thread>
#include <random>

// msoft apparently decided to redefine GetObject as a macro which means
// I can't use the function >:(
#ifdef _MSC_VER
#undef GetObject
#endif

namespace Battlesnake
{
namespace Server
{
struct SnakeCustomizations
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

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing SnakeCustomizations" << std::endl;
    
    // this is a "required" field >:(
    if(v.HasMember("apiversion"))
    {
      std::cerr << "Deserializing apiversion" << std::endl;
      apiversion = v["apiversion"].GetString();
    }
    // these are all optional fields
    if(v.HasMember("author"))
    {
      std::cerr << "Deserializing author" << std::endl;
      author = v["author"].GetString();
    }
    if(v.HasMember("color"))
    {
      std::cerr << "Deserializing color" << std::endl;
      color = v["color"].GetString();
    }
    if(v.HasMember("head"))
    {
      std::cerr << "Deserializing head" << std::endl;
      head = v["head"].GetString();
    }
    if(v.HasMember("tail"))
    {
      std::cerr << "Deserializing tail" << std::endl;
      tail = v["tail"].GetString();
    }
    if(v.HasMember("version"))
    {
      std::cerr << "Deserializing version" << std::endl;
      version = v["version"].GetString();
    }
    std::cerr << "Finished Deserializing SnakeCustomizations" << std::endl;
  }
};
struct Settings
{
  size_t foodSpawnChance;
  size_t minimumFood;
  size_t hazardDamagePerTurn;
  size_t royaleShrinkEveryNTurns;
  bool squadAllowBodyCollisions;
  bool squadSharedElimination;
  bool squadSharedHealth;
  bool squadSharedLength;

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing Settings" << std::endl;
    foodSpawnChance = v["foodSpawnChance"].GetInt();
    minimumFood = v["minimumFood"].GetInt();
    hazardDamagePerTurn = v["hazardDamagePerTurn"].GetInt();
    const auto& royale = v["royale"].GetObject();
    royaleShrinkEveryNTurns = royale["shrinkEveryNTurns"].GetInt();
    const auto& squad = v["squad"].GetObject();
    squadAllowBodyCollisions = squad["allowBodyCollisions"].GetBool();
    squadSharedElimination = squad["sharedElimination"].GetBool();
    squadSharedHealth = squad["sharedHealth"].GetBool();
    squadSharedLength = squad["sharedLength"].GetBool();
  }
};
struct Ruleset
{
  Core::String name;
  Core::String version;
  Settings settings;

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing Ruleset" << std::endl;
    name = v["name"].GetString();
    version = v["version"].GetString();
    settings.Deserialize(v["settings"]);
  }
};
struct GameInfo
{
  Core::String id;
  Ruleset rules;
  Core::String map;
  size_t timeout;
  Core::String source;

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing GameInfo" << std::endl;
    id = v["id"].GetString();
    rules.Deserialize(v["ruleset"]);
    map = v["map"].GetString();
    timeout = v["timeout"].GetInt();
    source = v["source"].GetString();
  }
};
struct Coordinate
{
  /// needed to default construct Snake objects
  Coordinate() : x(0), y(0) {}
  /// needed for emplace_back
  Coordinate(size_t inX, size_t inY) : x(inX), y(inY) {}
  size_t x;
  size_t y;
};
struct Snake
{
  Core::String id;
  Core::String name;
  size_t health;
  Core::Vector<Coordinate> bodies;
  Core::String latency;
  Coordinate head;
  size_t length;
  Core::String shout;
  Core::String squad;
  SnakeCustomizations customizations;

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing Snake" << std::endl;
    id = v["id"].GetString();
    name = v["name"].GetString();
    health = v["health"].GetInt();
    for(const auto& body: v["body"].GetArray())
    {
      bodies.emplace_back(body["x"].GetInt(), body["y"].GetInt());
    }
    latency = v["latency"].GetString();
    assert(bodies.size() > 0);
    head = bodies[0];
    length = v["length"].GetInt();
    shout = v["shout"].GetString();
    squad = v["squad"].GetString();
    customizations.Deserialize(v["customizations"]);
  }
};
struct Board
{
  size_t height;
  size_t width;
  Core::Vector<Coordinate> foods;
  Core::Vector<Coordinate> hazards;
  Core::Vector<Snake> snakes;

  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing Board" << std::endl;
    height = v["height"].GetInt();
    width = v["width"].GetInt();
    for(const auto& food: v["food"].GetArray())
    {
      foods.emplace_back(food["x"].GetInt(), food["y"].GetInt());
    }
    for(const auto& hazard: v["hazards"].GetArray())
    {
      hazards.emplace_back(hazard["x"].GetInt(), hazard["y"].GetInt());
    }
    for(const auto& snake: v["snakes"].GetArray())
    {
      Snake s;
      s.Deserialize(snake);
      snakes.push_back(s);
    }
  }
};
struct GameState
{
  GameInfo gameInfo;
  size_t turn;
  Board board;
  Snake snake;
  void Deserialize(const rapidjson::Value& v)
  {
    std::cerr << "Deserializing GameState" << std::endl;
    gameInfo.Deserialize(v["game"]);
    turn = v["turn"].GetInt();
    board.Deserialize(v["board"]);
    snake.Deserialize(v["you"]);
  }
};
struct Move
{
  Core::String move;
  Core::String shout;
  void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
  {
    std::cerr << "Moving " << move << std::endl;
    writer.StartObject();
    writer.Key("move");
    writer.String(move.c_str());
    writer.Key("shout");
    writer.String(shout.c_str());
    writer.EndObject();
  }
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