module;

#include <boost/asio/as_tuple.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/system_executor.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <chrono>
#include <expected>
#include <memory>
#include <string>
#include <utility>

module session;

import errors;

namespace cup::http
{
    namespace beast = boost::beast;
    namespace http_impl = beast::http;
    namespace net = boost::asio;

    class TcpSession::Impl : public std::enable_shared_from_this<Impl>
    {
      public:
        explicit Impl(Config config) : config{config}
        {
        }

        using tcp = boost::asio::ip::tcp;

        template <typename Body, typename Fields = http_impl::fields>
        using http_response = http_impl::response<Body, Fields>;
        template <typename Body, typename Fields = http_impl::fields>
        using http_request = http_impl::request<Body, Fields>;

        auto run(Request request) -> async_result
        {

            auto executor = co_await net::this_coro::executor;
            auto resolver_ = tcp::resolver{executor};
            auto stream_ = beast::tcp_stream{executor};

            auto req_ = http_request<http_impl::empty_body>{};

            auto port = std::to_string(request.Url.port_number());

            req_.version(static_cast<int>(request.HttpVersion));
            req_.method(request.Method);
            req_.set(http_field::host, request.Url.host_name());
            req_.set(http_field::user_agent, request.UserAgent);

            req_.target(request.Url.path());

            auto const results = co_await resolver_.async_resolve(request.Url.host_name(), port);
            beast::get_lowest_layer(stream_).expires_after(timeout_);
            co_await beast::get_lowest_layer(stream_).async_connect(results);

            beast::get_lowest_layer(stream_).expires_after(timeout_);
            co_await http_impl::async_write(stream_, req_);

            auto buffer = beast::flat_buffer{};
            auto res = http_response<http_impl::dynamic_body>{};
            beast::get_lowest_layer(stream_).expires_after(timeout_);
            co_await http_impl::async_read(stream_, buffer, res);

            co_return std::unexpected(errors::ok());
        }

      private:
        Config config;
        std::chrono::milliseconds timeout_{DefaultRequestTimeout};
    };

    TcpSession::TcpSession(Config config) : pImpl{std::make_unique<Impl>(config)}
    {
    }

    TcpSession::~TcpSession() = default;

    auto TcpSession::run(Request request) -> TcpSession::async_result
    {
        return pImpl->run(std::move(request));
    }
} // namespace cup::http
