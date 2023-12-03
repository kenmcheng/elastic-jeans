#include "json.hpp"

#include <iostream>

namespace elasticJeans {

const char JsonBase::skippableChars[4] {' ', '\n', '\r', '\t'};

JsonBase& JsonBase::operator[](const std::string& attr) {
    return *this;
}

JsonBase& JsonBase::operator=(const std::string& value) {
    this->parse(value);
    return *this;
}

int JsonBase::parse(const std::string& jsonStr, size_t begin, size_t end) {
    return invalid();
}

bool JsonBase::skippable(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(skippableChars), std::end(skippableChars), 
            jsonStr[idx]) != std::end(skippableChars);
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

int JsonObject::parse(const std::string& jsonStr, size_t begin, size_t end) {
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
                if (idx < 0) return invalid();
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
                if (!valid()) return -1;
                continue;
            }
            if (!colon) return invalid();
            auto jsonStringPtr = std::make_shared<JsonString>();
            jsonObject_[std::move(attr)] = jsonStringPtr;
            colon = false;
            idx = jsonStringPtr->parse(jsonStr, idx, end);
            if (idx < 0) return invalid();
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
            if (idx < 0) return invalid();
            continue;
        }

        if (iterChar - '0' >= 0 && iterChar - '0' < 10) {
            auto jsonNumber = std::make_shared<JsonNumber>();
            jsonObject_[std::move(attr)] = jsonNumber;
            idx = jsonNumber->parse(jsonStr, idx, end);
            if (idx < 0) return invalid();
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

    return valid_ ? idx+1 : -1;
}

int JsonArray::parse(const std::string& jsonStr, size_t begin, size_t end) {
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
        if (!allowNextValue) return invalid();
        if (iterChar == '[') {
            if (openBracket) {
                // if (!allowNextValue) return invalid();
                auto jsonArrPtr = std::make_shared<JsonArray>();
                jsonArray_.push_back(jsonArrPtr);
                idx = jsonArrPtr->parse(jsonStr, idx, end);
                if (idx < 0) return invalid();
                allowNextValue = false;
                continue;
            }
            openBracket = true;
            idx++;
            continue;
        }
        allowNextValue = false;
        if (!openBracket) return invalid();

        if (iterChar == '{') {
            auto jsonObjPtr = std::make_shared<JsonObject>();
            jsonArray_.push_back(jsonObjPtr);
            idx = jsonObjPtr->parse(jsonStr, idx, end);
            if (idx < 0) return invalid();
            continue;
        }

        if (iterChar == '"') {
            auto jsonStringPtr = std::make_shared<JsonString>();
            jsonArray_.push_back(jsonStringPtr);
            idx = jsonStringPtr->parse(jsonStr, idx, end);
            if (idx < 0) return invalid();
            continue;
        }

        if (iterChar - '0' >= 0 && iterChar - '0' < 10) {
            auto jsonNumber = std::make_shared<JsonNumber>();
            jsonArray_.push_back(jsonNumber);
            idx = jsonNumber->parse(jsonStr, idx, end);
            if (idx < 0) return invalid();
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

        if (iterChar == ']') break;

        return invalid();
    }
    if (idx == end) return invalid();
    return valid_ ? idx+1 : -1;
}

int JsonString::parse(const std::string& jsonStr, size_t begin, size_t end) {
    str_ = parseQuoted(jsonStr, begin);
    if (!valid_ || begin >= end) return invalid();
    return valid_ ? begin : -1;
}

int JsonNumber::parse(const std::string& jsonStr, size_t begin, size_t end) {
    size_t idx = begin;
    end = std::min(jsonStr.size(), end);
    while (idx < end && skippable(jsonStr, idx)) {
        idx++;
    }
    size_t numBegin = idx;
    while (idx < end && (!skippable(jsonStr, idx) || jsonStr[idx] == ',')) {
        idx++;
    }
    jsonNumber_ = std::stod(jsonStr.substr(numBegin, idx-numBegin));
    return valid_ ? idx : -1;
}

} // namespace elasticJeans