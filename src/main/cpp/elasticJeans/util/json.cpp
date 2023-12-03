#include "json.hpp"

#include <iostream>

namespace elasticJeans {

const char JsonBase::skippableChars[4] {' ', '\n', '\r', '\t'};
const char JsonBase::openningChars[3] {'{', '[', '"'};
const char JsonBase::closingChars[3] {'}', ']', '"'};
const char JsonNumber::mathChars[5] {'+', '-', '.', 'E', 'e'};

JsonBase& JsonBase::operator[](const std::string& attr) {
    return *this;
}

JsonBase& JsonBase::operator=(const std::string& value) {
    this->parse(value);
    return *this;
}

size_t JsonBase::parse(const std::string& jsonStr, size_t begin, size_t end) {
    return invalid();
}

bool JsonBase::skippable(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(skippableChars), std::end(skippableChars), 
            jsonStr[idx]) != std::end(skippableChars);
}

bool JsonBase::isOpening(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(openningChars), std::end(openningChars), 
            jsonStr[idx]) != std::end(openningChars);
}

bool JsonBase::isClosing(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(closingChars), std::end(closingChars), 
            jsonStr[idx]) != std::end(closingChars);
}

std::string JsonBase::parseQuoted(const std::string& jsonStr, size_t& from) {
    size_t end = jsonStr.size();
    size_t idx = from;
    if (end == 0) {
        from = invalid();
        return "";
    }
    bool openQuote = false;
    bool escape = false;
    int attrBegin = idx;
    while (idx < end) {
        if (escape) {
            escape = false;
        } else if (jsonStr[idx] == '\\') {
            escape = true;
        } else if (jsonStr[idx] == '"') {
            if (openQuote) break;
            attrBegin = idx;
            openQuote = true;
        } else {
            if (!openQuote && !skippable(jsonStr, idx)) {
                from = invalid();
                return "";
            }
        }
        idx++;
    }
    if (idx == end) {
        from = invalid();
        return "";
    }
    std::string rtn = jsonStr.substr(attrBegin+1, idx-attrBegin-1);
    from = idx+1;
    return rtn;
}

// std::optional<JsonBase::jsonPtr> JsonBase::at(const std::string& attr) {
//     if (!entityExist() || json_->count(attr) == 0) {
//         return std::nullopt;
//     }
//     return json_->at(attr);
// }

JsonBase& JsonObject::operator[](const std::string& attr) {
    // if (!entityExist()) {
    //     jsonObject_ = std::make_unique<std::unordered_map<std::string, jsonPtr>>();
    // }
    return *(jsonObject_.at(attr));
}

size_t JsonObject::parse(const std::string& jsonStr, size_t begin, size_t end) {
    size_t idx = begin;
    end = std::min(jsonStr.size(), end);
    bool openCurlyBrace = false;
    bool colon = false;
    bool allowNextAttr = true;
    std::string attr;
    while (idx < end) {
        char iterChar = jsonStr[idx];
        if (skippable(jsonStr, idx)) {
            idx++;
            continue;
        }
        if (iterChar == '{') {
            if (openCurlyBrace) {
                if (attr.empty()) return invalid();

                auto jsonObjPtr = std::make_shared<JsonObject>();
                jsonObject_[std::move(attr)] = jsonObjPtr;
                idx = jsonObjPtr->parse(jsonStr, idx, end);
                if (idx == std::string::npos) return invalid();
                continue;
            }
            openCurlyBrace = true;
            idx++;
            continue;
        }
        if (!openCurlyBrace) return invalid();

        if (iterChar == '"') {
            if (attr.empty()) {
                if (!allowNextAttr) return invalid();
                attr = parseQuoted(jsonStr, idx);
                allowNextAttr = false;
                if (!valid()) return std::string::npos;
                continue;
            }
            if (!colon) return invalid();
            auto jsonStringPtr = std::make_shared<JsonString>();
            jsonObject_[std::move(attr)] = jsonStringPtr;
            colon = false;
            idx = jsonStringPtr->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar == ',') {
            if (!attr.empty()) return invalid();
            allowNextAttr = true;
            idx++;
            continue;
        }
        
        if (iterChar == '}') break;

        if (attr.empty()) return invalid();

        if (iterChar == ':') {
            colon = true;
            idx++;
            continue;
        }

        if (iterChar == '[') {
            auto jsonArrPtr = std::make_shared<JsonArray>();
            jsonObject_[std::move(attr)] = jsonArrPtr;
            idx = jsonArrPtr->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar - '0' >= 0 && iterChar - '0' < 10) {
            auto jsonNumber = std::make_shared<JsonNumber>();
            jsonObject_[std::move(attr)] = jsonNumber;
            idx = jsonNumber->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar == 't') {
            if (jsonStr.size() - idx < 4 || jsonStr.substr(idx, 4) != "true") {
                return invalid();
            }
            jsonObject_[std::move(attr)] = std::make_shared<JsonBoolean>(true);
            idx += 4;
            continue;
        }

        if (iterChar == 'f') {
            if (jsonStr.size() - idx < 5 && jsonStr.substr(idx, 5) != "false") {
                return invalid();
            }
            jsonObject_[std::move(attr)] = std::make_shared<JsonBoolean>(false);
            idx += 5;
            continue;
        }

        if (iterChar == 'n') {
            if (jsonStr.size() - idx < 4 && jsonStr.substr(idx, 4) != "null") {
                return invalid();
            }
            jsonObject_[std::move(attr)] = std::make_shared<JsonVoid>();
            idx += 4;
            continue;
        }

        return invalid();
    }

    if (idx == end || !attr.empty()) return invalid();

    return valid_ ? idx+1 : std::string::npos;
}

std::string JsonObject::toString(bool compact, int indent) {
    std::ostringstream osstream;
    std::string indentation = "";
    std::string colon = compact ? ":" : ": ";
    if (!compact)
        for (int i = 0; i < indent; i++) indentation += "    ";
    
    osstream << '{';
    if (!compact) osstream << '\n';
    for (auto it = std::begin(jsonObject_); it != std::end(jsonObject_); it++) {
        auto& [k, val] = *it;
        if (!compact)
            osstream << indentation << "    ";
            
        osstream << "\"" << k << "\"" << colon << val->toString(compact, indent+1);
        if (std::next(it) != std::end(jsonObject_)) {
            osstream << ",";
        }
        if (!compact) osstream << "\n";
    }
    if (!compact) osstream << indentation;
    osstream << '}';
    return osstream.str();
}

size_t JsonArray::parse(const std::string& jsonStr, size_t begin, size_t end) {
    size_t idx = begin;
    end = std::min(jsonStr.size(), end);
    bool openBracket = false;
    bool allowNextValue = true;
    while (idx < end) {
        if (skippable(jsonStr, idx)) {
            idx++;
            continue;
        }
        char iterChar = jsonStr[idx];
        if (iterChar == ',') {
            if (!openBracket) return invalid();
            allowNextValue = true;
            idx++;
            continue;
        }
        
        if (iterChar == '[') {
            if (openBracket) {
                if (!allowNextValue) return invalid();
                auto jsonArrPtr = std::make_shared<JsonArray>();
                jsonArray_.push_back(jsonArrPtr);
                idx = jsonArrPtr->parse(jsonStr, idx, end);
                if (idx == std::string::npos) return invalid();
                allowNextValue = false;
                continue;
            }
            openBracket = true;
            idx++;
            continue;
        }
        
        if (iterChar == ']') break;

        if (!allowNextValue) return invalid();
        allowNextValue = false;
        if (!openBracket) return invalid();

        if (iterChar == '{') {
            auto jsonObjPtr = std::make_shared<JsonObject>();
            jsonArray_.push_back(jsonObjPtr);
            idx = jsonObjPtr->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar == '"') {
            auto jsonStringPtr = std::make_shared<JsonString>();
            jsonArray_.push_back(jsonStringPtr);
            idx = jsonStringPtr->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar - '0' >= 0 && iterChar - '0' < 10) {
            auto jsonNumber = std::make_shared<JsonNumber>();
            jsonArray_.push_back(jsonNumber);
            idx = jsonNumber->parse(jsonStr, idx, end);
            if (idx == std::string::npos) return invalid();
            continue;
        }

        if (iterChar == 't') {
            if (jsonStr.size() - idx < 4 || jsonStr.substr(idx, 4) != "true") {
                return invalid();
            }
            jsonArray_.push_back(std::make_shared<JsonBoolean>(true));
            idx += 4;
            continue;
        }

        if (iterChar == 'f') {
            if (jsonStr.size() - idx < 5 && jsonStr.substr(idx, 5) != "false") {
                return invalid();
            }
            jsonArray_.push_back(std::make_shared<JsonBoolean>(false));
            idx += 5;
            continue;
        }

        if (iterChar == 'n') {
            if (jsonStr.size() - idx < 4 && jsonStr.substr(idx, 4) != "null") {
                return invalid();
            }
            jsonArray_.push_back(std::make_shared<JsonVoid>());
            idx += 4;
            continue;
        }

        return invalid();
    }
    
    if (idx == end) return invalid();
    return valid_ ? idx+1 : std::string::npos;
}

std::string JsonArray::toString(bool compact, int indent) {
    std::ostringstream osstream;
    osstream << '[';
    for (auto it = std::begin(jsonArray_); it != std::end(jsonArray_); it++) {
        auto ptr = *it;
        osstream << ptr->toString();
        if (std::next(it) != std::end(jsonArray_)) {
            osstream << ",";
        }
    }
    osstream << ']';
    return osstream.str();
}

size_t JsonString::parse(const std::string& jsonStr, size_t begin, size_t end) {
    str_ = parseQuoted(jsonStr, begin);
    if (!valid_ || begin >= end) return invalid();
    return valid_ ? begin : std::string::npos;
}

bool JsonNumber::isMathSign(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(mathChars), std::end(mathChars), 
        jsonStr[idx]) != std::end(mathChars);
}

size_t JsonNumber::parse(const std::string& jsonStr, size_t begin, size_t end) {
    size_t idx = begin;
    end = std::min(jsonStr.size(), end);
    while (idx < end && skippable(jsonStr, idx)) {
        idx++;
    }
    size_t numBegin = idx;
    while (idx < end && ((jsonStr[idx]-'0' >= 0 && jsonStr[idx]-'0' < 10) || isMathSign(jsonStr, idx))) {
        idx++;
    }
    if (idx <= numBegin) return invalid();
    jsonNumber_ = std::stod(jsonStr.substr(numBegin, idx-numBegin));
    return valid_ ? idx : std::string::npos;
}

} // namespace elasticJeans