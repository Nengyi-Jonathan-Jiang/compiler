#include "token.h"

std::string token::to_string() const {
    return type->name == value
           ? "<" + type->name + ">"
           : type->name + "<" + value + ">";
}

token::token(symbol type, std::string value, size_t startIndex, size_t endIndex) : type(std::move(type)),
                                                                                   value(std::move(value)),
                                                                                   startIndex(startIndex),
                                                                                   endIndex(endIndex) {}

bool token::is_end_token() const {
    return !type.operator bool();
}

token::operator bool() const {
    return !is_end_token();
}

token token::create_end_token(size_t position) {
    return {{}, "__END__", position, position};
}
