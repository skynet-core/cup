module;

#include <system_error>

export module errors;

namespace cup::http::errors
{
    enum class Code : int
    {
        Ok = 0,
    };
    class Error : public std::error_category
    {
      public:
        [[nodiscard]] auto name() const noexcept -> const char * override;
        [[nodiscard]] auto message(int code) const -> std::string override;
    };

    export auto make_error(const Code code) -> std::error_condition;

    export auto ok() -> std::error_condition;
} // namespace cup::http::errors