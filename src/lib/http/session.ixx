module;

#include <boost/asio/awaitable.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/url/url.hpp>
#include <chrono>
#include <expected>
#include <memory>
#include <string>
#include <system_error>

export module session;

namespace cup::http
{
    using namespace std::chrono_literals;

    namespace http_impl = boost::beast::http;
    export enum class Version
    {
        V10 = 10,
        V11 = 11
    };

    export template <typename Base> class SessionI
    {
      private:
        SessionI() = default;
        friend Base;

      public:
        using http_method = http_impl::verb;
        using http_field = http_impl::field;
        using url = boost::urls::url;

        template <typename T> using async = boost::asio::awaitable<T>;

        constexpr static auto DefaultUserAgent = BOOST_BEAST_VERSION_STRING;
        constexpr static auto DefaultHttpVersion = Version::V10;
        constexpr static auto DefaultRequestTimeout = std::chrono::milliseconds{30s};

        struct Config
        {
            std::chrono::milliseconds ConnectTimeout{DefaultRequestTimeout};
            std::chrono::milliseconds TlsHandshakeTimeout{DefaultRequestTimeout};
            std::chrono::milliseconds WriteTimeout{DefaultRequestTimeout};
            std::chrono::milliseconds ReadTimeout{DefaultRequestTimeout};
        };

        struct Request
        {
            http_method Method{};
            url Url;
            std::string UserAgent{DefaultUserAgent};
            Version HttpVersion{DefaultHttpVersion};
        };

        struct Response
        {
        };

        using result = std::expected<Response, std::error_condition>;
        using async_result = async<result>;

        auto run(Request request) -> async_result
        {
            return static_cast<Base *>(this)->run(std::move(request));
        }
    };

    export class TcpSession : public SessionI<TcpSession>
    {
        class Impl;

      public:
        explicit TcpSession(Config config = Config{});
        ~TcpSession();

        auto run(Request request) -> async_result;

      private:
        std::unique_ptr<Impl> pImpl;
    };
} // namespace cup::http