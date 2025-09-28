module;

#include <boost/asio/awaitable.hpp>
#include <expected>
#include <filesystem>
#include <memory>
#include <string_view>
#include <system_error>

export module fetcher;

namespace depot
{
    export class Fetcher
    {
        class Impl;
        class Error : public std::error_category
        {
          public:
            [[nodiscard]] auto name() const noexcept -> const char * override;
            [[nodiscard]] auto message(int code) const -> std::string override;
        };

      public:
        enum class ErrorCode
        {
            Ok = 0,
        };
        static auto error_category() noexcept -> const Fetcher::Error &;

        Fetcher();
        explicit Fetcher(std::filesystem::path storageBaseDir);
        ~Fetcher();

        using AsyncExecutor = boost::asio::io_context;
        using Result = std::expected<std::string, std::error_condition>;
        using AsyncResult = boost::asio::awaitable<Result>;

        [[nodiscard]] auto fetch(const AsyncExecutor& execCtx, std::string_view url) -> AsyncResult;

      private:
        std::unique_ptr<Impl> pImpl;
    };
} // namespace depot
