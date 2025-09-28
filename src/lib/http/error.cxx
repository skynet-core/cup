module;

#include <cstdlib>
#include <string>
#include <system_error>
#include <unordered_map>

module errors;

namespace cup::http::errors
{
    namespace
    {
        constexpr auto cti(const Code code)
        {
            return static_cast<int>(code);
        }
    } // namespace

    auto Error::name() const noexcept -> const char *
    {
        return "HTTP_ERROR";
    }

    auto Error::message(int code) const -> std::string
    {
        switch (code)
        {
        case cti(Code::Ok):
            return "OK";
        default:
            abort();
        };
    };

    auto make_error(const Code code) -> std::error_condition
    {
        static Error category{};
        return category.default_error_condition(cti(code));
    }

    auto ok() -> std::error_condition
    {
        return make_error(Code::Ok);
    }

} // namespace cup::http::errors
