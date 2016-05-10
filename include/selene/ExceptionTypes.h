#pragma once

#include <exception>
#include <string>
#include <utility>
#include <string>

namespace sel {

class SeleneException : public std::exception {};

class TypeError : public SeleneException {
    std::string _message;
public:
    explicit TypeError(std::string expected)
      : _message(std::move(expected)
                 + " expected, got no object.") {}
    explicit TypeError(std::string expected, std::string const & actual)
      : _message(std::move(expected)
                 + " expected, got " + actual + '.') {}

# if _MSC_VER <= 1800
    char const * what() const override {
#else
    char const * what() const noexcept override {
#endif
        return _message.c_str();
    }
};

class CopyUnregisteredType : public SeleneException {
public:
    using TypeID = std::reference_wrapper<const std::type_info>;
    explicit CopyUnregisteredType(TypeID type) : _type(type) {}

    TypeID getType() const
    {
        return _type;
    }

# if _MSC_VER <= 1800
    char const * what() const override {
#else
    char const * what() const noexcept override {
#endif
        return "Tried to copy an object of an unregistered type. "
               "Please register classes before passing instances by value.";
    }
private:
    TypeID _type;
};
}
