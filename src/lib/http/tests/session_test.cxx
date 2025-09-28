#include <catch2/catch_test_macros.hpp>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/url/url.hpp>
#include <boost/asio/co_spawn.hpp>
#include <exception>

import session;

using namespace cup::http;
using namespace std::literals::string_literals;

TEST_CASE( "Http Module", "[session]" ) {
    boost::asio::io_context io;

    TcpSession session{};
    using Method = TcpSession::Method;
    using Url = TcpSession::Url;
    auto url = Url{"http://ifconfig.me/test"s};

    auto fut = boost::asio::co_spawn(io, session.run(TcpSession::Request{
        .Method = Method::get,
        .Url = std::move(url),
    }),
        boost::asio::as_tuple(boost::asio::use_future));
    io.run();
    auto [err, result] = fut.get();
    REQUIRE( !err );
}
