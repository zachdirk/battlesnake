#include "httplib.h"

#include "Core/String.h"

namespace Battlesnake
{
namespace Server
{
/// A wrapper around the httplib server
class Server
{
public:
  Server();
  ~Server();

  void Init(Core::String url, size_t port);

private:
  httplib::Server mServer;
};
}
}