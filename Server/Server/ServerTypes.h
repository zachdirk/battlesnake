#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Core/String.h"
#include "Core/Vector.h"

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
    // this is a "required" field >:(
    if(v.HasMember("apiversion"))
    {
      apiversion = v["apiversion"].GetString();
    }
    // these are all optional fields
    if(v.HasMember("author"))
    {
      author = v["author"].GetString();
    }
    if(v.HasMember("color"))
    {
      color = v["color"].GetString();
    }
    if(v.HasMember("head"))
    {
      head = v["head"].GetString();
    }
    if(v.HasMember("tail"))
    {
      tail = v["tail"].GetString();
    }
    if(v.HasMember("version"))
    {
      version = v["version"].GetString();
    }
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
    name = v["name"].GetString();
    version = v["version"].GetString();
    // docs doesn't say this is optional, but the example
    // for GameInfo has it missing
    if(v.HasMember("settings"))
    {
      settings.Deserialize(v["settings"]);
    }
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
    writer.StartObject();
    writer.Key("move");
    writer.String(move.c_str());
    writer.Key("shout");
    writer.String(shout.c_str());
    writer.EndObject();
  }
};
}
}