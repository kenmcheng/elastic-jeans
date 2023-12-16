#include <elasticJeans/util/json.hpp>

#include <iostream>

namespace elasticJeans {

const char JsonBase::whitespaces[4] {' ', '\n', '\r', '\t'};
const char JsonBase::openningChars[3] {'{', '[', '"'};
const char JsonBase::closingChars[3] {'}', ']', '"'};
const char JsonNumber::mathChars[5] {'+', '-', '.', 'E', 'e'};

JsonBase& JsonBase::operator[](const std::string& attr) {
    return *this;
}

JsonBase& JsonBase::operator[](size_t idx) {
    return *this;
}

JsonBase& JsonBase::operator=(const std::string& value) {
    this->parse(value);
    return *this;
}

std::optional<JsonBase::jsonPtr> JsonBase::at(const std::string& attr) {
    return std::nullopt;
}

std::optional<JsonBase::jsonPtr> JsonBase::at(size_t idx) {
    return std::nullopt;
}

size_t JsonBase::parse(const std::string& jsonStr, size_t begin, size_t end) {
    return invalid();
}

bool JsonBase::skippable(const std::string& jsonStr, size_t idx) {
    return std::find(std::begin(whitespaces), std::end(whitespaces), 
            jsonStr[idx]) != std::end(whitespaces);
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
    std::string subJsonStr = jsonStr.substr(attrBegin+1, idx-attrBegin-1);
    from = idx+1;
    return processString(subJsonStr, true);
}

std::string JsonBase::processString(const std::string& str, bool convertEscape) {
    if (!convertEscape) return str;

    // std::string newStr(str);
    // newStr.erase(std::remove(newStr.begin(), newStr.end(), '\\'), str.end());
    // return newStr;
    std::ostringstream osstream;
    bool escape = false;
    for (char c : str) {
        if (!escape) {
            if (c == '\\') {
                escape = true;
            } else osstream << c;
            continue;
        }
        switch (c) {
        case '"':
        case '\\':
        case '/':
            osstream << c;
            break;
        case 'b':
            osstream << ' ';
            break;
        case 'f':
            osstream << '\f';
            break;
        case 'n':
            osstream << '\n';
            break;
        case 'r':
            osstream << '\r';
            break;
        case 't':
            osstream << '\t';
            break;
        case 'u':
            osstream << "\\u";
        default:
            break;
        }
        escape = false;
    }
    return osstream.str();
}


std::string JsonBase::jsonizeString(const std::string& str) {
    std::ostringstream osstream;
    for (char c : str) {
        // if (!escape) {
        //     if (c == '\\') {
        //         escape = true;
        //     } else osstream << c;
        //     continue;
        // }
        switch (c) {
        case '"':
        // case '\\':
        case '/':
            osstream << '\\' << c;
            break;
        case '\b':
            osstream << "\\b";
            break;
        case '\f':
            osstream << "\\f";
            break;
        case '\n':
            osstream << "\\n";
            break;
        case '\r':
            osstream << "\\r";
            break;
        case '\t':
            osstream << "\\t";
            break;

        default:
            osstream << c;
            break;
        }
    }
    return osstream.str();
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

std::optional<JsonBase::jsonPtr> JsonObject::at(const std::string& attr) {
    if (!jsonObject_.count(attr)) return std::nullopt;
    return jsonObject_[attr];
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
    return serialize(compact, indent, false);
}

std::string JsonObject::jsonize(bool compact, int indent) {
    return serialize(compact, indent, true);
}

std::string JsonObject::serialize(bool compact, int indent, bool convertJson) {
    std::ostringstream osstream;
    std::string indentation = "";
    std::string colon = compact ? ":" : ": ";
    if (!compact)
        for (int i = 0; i < indent; i++) indentation += "    ";
    
    osstream << '{';
    if (!compact) osstream << "\r\n";
    for (auto it = std::begin(jsonObject_); it != std::end(jsonObject_); it++) {
        auto& [k, val] = *it;
        if (!compact)
            osstream << indentation << "    ";
            
        osstream << "\"" << (convertJson ? jsonizeString(k) : k) << "\"" << colon << val->serialize(compact, indent+1, convertJson);
        if (std::next(it) != std::end(jsonObject_)) {
            osstream << ",";
        }
        if (!compact) osstream << "\r\n";
    }
    if (!compact) osstream << indentation;
    osstream << '}';
    return osstream.str();
}

JsonBase& JsonArray::operator[](size_t idx) {
    if (idx >= jsonArray_.size()) {
        return *this;
    }
    return *(jsonArray_.at(idx));
}

std::optional<JsonBase::jsonPtr> JsonArray::at(size_t idx) {
    if (idx >= jsonArray_.size()) return std::nullopt;
    return jsonArray_.at(idx);
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

std::string JsonArray::serialize(bool compact, int indent, bool convertJson) {
    std::ostringstream osstream;
    std::string indentation = "";
    if (!compact)
        for (int i = 0; i < indent; i++) indentation += "    ";

    osstream << '[';
    if (!compact) osstream << "\r\n";
    for (auto it = std::begin(jsonArray_); it != std::end(jsonArray_); it++) {
        auto ptr = *it;
        if (!compact) osstream << indentation << "    ";
        osstream << ptr->serialize(compact, indent+1, convertJson);
        if (std::next(it) != std::end(jsonArray_)) {
            osstream << ",";
            
        }
        if (!compact) osstream << "\r\n";
    }
    if (!compact) osstream << indentation;
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