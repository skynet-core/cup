module;

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>
#include <unordered_map>

module fetcher;

namespace depot
{
    namespace
    {
        constexpr auto cei(const Fetcher::ErrorCode code) -> int
        {
            return static_cast<int>(code);
        }

        std::unordered_map<int, const char *> codeToMessage{{cei(Fetcher::ErrorCode::Ok), "OK"}};

        auto make_error(const Fetcher::ErrorCode code) -> std::error_condition
        {
            return Fetcher::error_category().default_error_condition(cei(code));
        }

    } // namespace

    class Fetcher::Impl
    {
      public:
        [[nodiscard]] auto fetch([[maybe_unused]] std::string_view url) -> Fetcher::AsyncResult
        {
            co_return std::unexpected(make_error(Fetcher::ErrorCode::Ok));
        }
    };

    Fetcher::Fetcher() = default;

    Fetcher::Fetcher([[maybe_unused]] std::filesystem::path storageBaseDir)
    {
    }

    Fetcher::~Fetcher() = default;

    auto Fetcher::error_category() noexcept -> const Fetcher::Error &
    {
        static Error error{};
        return error;
    }

    [[nodiscard]] auto Fetcher::fetch([[maybe_unused]] const AsyncExecutor &execCtx, std::string_view url)
        -> AsyncResult
    {
        return pImpl->fetch(url);
    }

    auto Fetcher::Error::name() const noexcept -> const char *
    {
        return "FETCHER_ERROR";
    }

    auto Fetcher::Error::message([[maybe_unused]] int code) const -> std::string
    {
        return codeToMessage[code];
    }

} // namespace depot
