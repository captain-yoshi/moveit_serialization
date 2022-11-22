#include <c4/yml/common.hpp>

#include <moveit_serialization/ryml/std/std.h>
#include <moveit_serialization/ryml/format.h>

#include <moveit_serialization/ryml/error_handler.h>

namespace {

struct ErrorHandler
{
    // this will be called on error
    void on_error(const char* msg, size_t len, c4::yml::Location loc)
    {
        throw moveit_serialization::yaml_error(c4::formatrs<std::string>(
            "{}:{}:{} ({}B): ERROR: {}", loc.name, loc.line, loc.col, loc.offset, c4::csubstr(msg, len)));
    }

    // bridge
    c4::yml::Callbacks callbacks()
    {
        return c4::yml::Callbacks(this, nullptr, nullptr, ErrorHandler::s_error);
    }
    static void s_error(const char* msg, size_t len, c4::yml::Location loc, void* this_)
    {
        return ((ErrorHandler*)this_)->on_error(msg, len, loc);
    }

    ErrorHandler() : defaults(c4::yml::get_callbacks())
    {}
    c4::yml::Callbacks defaults;
};

// https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#solving-the-static-initialization-order-fiasco
auto& registerErrorHandler()
{
    c4::set_error_flags(c4::get_error_flags() & ~c4::ON_ERROR_DEBUGBREAK);

    static auto err = ErrorHandler();
    c4::yml::set_callbacks(err.callbacks());

    return err;
}

auto ERROR_HANDLER = registerErrorHandler();

}  // namespace
