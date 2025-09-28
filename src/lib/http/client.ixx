module;

#include <boost/asio/awaitable.hpp>
#include <expected>
#include <filesystem>
#include <memory>
#include <string_view>
#include <system_error>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

export module client;

namespace cup::http
{
    export class Client
    {
        class Impl;

      public:
        Client();
        ~Client();

        using Response = boost::beast::http::response<boost::beast::http::dynamic_body>;
        using AsyncExecutor = boost::asio::io_context;
        using Result = std::expected<Response, std::error_condition>;
        using AsyncResult = boost::asio::awaitable<Result>;

      private:
        std::unique_ptr<Impl> pImpl;
    };
} // namespace cup::http

