#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"

#include "Core/Common.h"
#include "Core/String.h"
#include "Server/ServerTypes.h"

#include <gtest/gtest.h>
#include <fstream>

using namespace Battlesnake;
using namespace Server;
using namespace rapidjson;

static Core::String kJSONDirectory = STRINGIFY(JSON_BASE_FOLDER);

TEST(Deserialize, SnakeCustomizations)
{
  Core::String path = kJSONDirectory + "/SnakeCustomizations.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  SnakeCustomizations sc;
  sc.Deserialize(d);

  EXPECT_STREQ(sc.apiversion.c_str(), "1");
  EXPECT_STREQ(sc.author.c_str(), "MyUsername");
  EXPECT_STREQ(sc.color.c_str(), "#888888");
  EXPECT_STREQ(sc.head.c_str(), "default");
  EXPECT_STREQ(sc.tail.c_str(), "default");
  EXPECT_STREQ(sc.version.c_str(), "0.0.1-beta");
}

TEST(Deserialize, Snake)
{
  Core::String path = kJSONDirectory + "/Snake.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  Snake s;
  s.Deserialize(d);
  
  EXPECT_STREQ(s.id.c_str(), "totally-unique-snake-id");
  EXPECT_STREQ(s.name.c_str(), "Sneky McSnek Face");
  EXPECT_EQ(s.health, 54);
  EXPECT_STREQ(s.latency.c_str(), "123");
  EXPECT_EQ(s.head.x, 0);
  EXPECT_EQ(s.head.y, 2);
  EXPECT_EQ(s.length, 3);
  EXPECT_STREQ(s.shout.c_str(), "why are we shouting??");
  EXPECT_STREQ(s.squad.c_str(), "1");
  ASSERT_EQ(s.bodies.size(), 3);
  EXPECT_EQ(s.bodies[0].x, 0);
  EXPECT_EQ(s.bodies[0].y, 2);
  EXPECT_EQ(s.bodies[1].x, 1);
  EXPECT_EQ(s.bodies[1].y, 1);
  EXPECT_EQ(s.bodies[2].x, 2);
  EXPECT_EQ(s.bodies[2].y, 0);
  EXPECT_STREQ(s.customizations.color.c_str(), "#26CF04");
  EXPECT_STREQ(s.customizations.head.c_str(), "smile");
  EXPECT_STREQ(s.customizations.tail.c_str(), "bolt");
}

TEST(Deserialize, Board)
{
  Core::String path = kJSONDirectory + "/Board.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  Board b;
  b.Deserialize(d);

  EXPECT_EQ(b.height, 11);
  EXPECT_EQ(b.width, 11);
  // we've tested parsing coordinates already
  ASSERT_EQ(b.foods.size(), 3);
  ASSERT_EQ(b.hazards.size(), 3);
  // we've tested parsing snakes already
  ASSERT_EQ(b.snakes.size(), 3);
  EXPECT_STREQ(b.snakes[0].id.c_str(), "totally-unique-snake-id-1");
  EXPECT_STREQ(b.snakes[1].id.c_str(), "totally-unique-snake-id-2");
  EXPECT_STREQ(b.snakes[2].id.c_str(), "totally-unique-snake-id-3");
}

TEST(Deserialize, Ruleset)
{
  Core::String path = kJSONDirectory + "/Ruleset.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  Ruleset r;
  r.Deserialize(d);

  EXPECT_STREQ(r.name.c_str(), "standard");
  EXPECT_STREQ(r.version.c_str(), "v1.2.3");
  EXPECT_EQ(r.settings.foodSpawnChance, 25);
  EXPECT_EQ(r.settings.minimumFood, 1);
  EXPECT_EQ(r.settings.hazardDamagePerTurn, 14);
  EXPECT_EQ(r.settings.royaleShrinkEveryNTurns, 5);
  EXPECT_TRUE(r.settings.squadAllowBodyCollisions);
  EXPECT_TRUE(r.settings.squadSharedElimination);
  EXPECT_TRUE(r.settings.squadSharedHealth);
  EXPECT_TRUE(r.settings.squadSharedLength);
}

TEST(Deserialize, GameInfo)
{
  Core::String path = kJSONDirectory + "/GameInfo.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  GameInfo g;
  g.Deserialize(d);

  EXPECT_STREQ(g.id.c_str(), "totally-unique-game-id");
  EXPECT_STREQ(g.rules.name.c_str(), "standard");
  EXPECT_STREQ(g.rules.version.c_str(), "v1.2.3");
  EXPECT_STREQ(g.map.c_str(), "standard");
  EXPECT_EQ(g.timeout, 500);
  EXPECT_STREQ(g.source.c_str(), "league");
}

TEST(Deserialize, GameState)
{
  Core::String path = kJSONDirectory + "/GameState.json";
  std::ifstream in(path.c_str());
  IStreamWrapper inw { in };

  Document d;
  d.ParseStream(inw);
  GameState g;
  g.Deserialize(d);

  EXPECT_EQ(g.turn, 15);
  // we've tested deserializing these before, so I'm just
  // checking one field to make sure it recursed
  EXPECT_STREQ(g.gameInfo.id.c_str(), "totally-unique-game-id");
  EXPECT_EQ(g.board.height, 11);
  EXPECT_STREQ(g.snake.id.c_str(), "totally-unique-snake-id");
}

TEST(Serialize, Move)
{
  Move m{"up", "woo!"};
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> w(s);
  m.Serialize(w);
  EXPECT_STREQ(s.GetString(), "{\"move\":\"up\",\"shout\":\"woo!\"}");
}

TEST(Serialize, SnakeCustomizations)
{
  SnakeCustomizations info{"1", "Jozrozroz", "#FF0000", "gamer", "pixel", "0.1.0"};
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> w(s);
  info.Serialize(w);
  EXPECT_STREQ(s.GetString(), 
    "{\"apiversion\":\"1\",\"author\":\"Jozrozroz\",\"color\":\"#FF0000\",\"head\":\"gamer\",\"tail\":\"pixel\",\"version\":\"0.1.0\"}"
  );
}