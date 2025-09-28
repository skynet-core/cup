module;

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <expected>

module client;

namespace cup::http
{
    class Client::Impl
    {
    };

    Client::Client() = default;

    Client::~Client() = default;

} // namespace cup::http
