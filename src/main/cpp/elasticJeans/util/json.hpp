#ifndef _ELASTICJEANS_JSON_H
#define _ELASTICJEANS_JSON_H

#include "serializable.hpp"

#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>

#include <sstream>

namespace elasticJeans {


class JsonBase {
    
public:
    using jsonPtr = std::shared_ptr<JsonBase>;

    JsonBase() = default;

    JsonBase(const std::string& jsonStr) { this->parse(jsonStr); }

    virtual JsonBase& operator[](const std::string& attr);

    JsonBase& operator=(const std::string& attr);

    virtual size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos);

    bool valid() { return valid_; }

    std::optional<jsonPtr> at(const std::string& attr);

    template<typename... Args>
    std::optional<jsonPtr> at(const std::string& attr, Args... args) {
        auto rtn = this->at(attr);
        if (rtn.has_value()) {
            return rtn->get()->at(args...);
        }
        return rtn;
    }

    virtual std::string toString(bool compact = true, int indent = 0) { return ""; }

protected:
    bool valid_ = true;
    bool isObject_ = false;
    bool isString_ = false;
    bool isBoolean_ = false;
    bool isNumeric_ = false;
    bool isInteger_ = false;
    bool isDouble_ = false;
    bool isArray_ = false;

    static const char skippableChars[4];
    static const char openningChars[3];
    static const char closingChars[3];

    size_t invalid() {
        valid_ = false;
        return std::string::npos;
    }

    std::string parseQuoted(const std::string& jsonStr, size_t& from);

    bool skippable(const std::string& jsonStr, size_t idx);

    bool isOpening(const std::string& jsonStr, size_t idx);

    bool isClosing(const std::string& jsonStr, size_t idx);
};

class JsonObject : public JsonBase {
public:
    JsonObject() = default;

    JsonObject(const std::string& jsonStr) : JsonBase() { this->parse(jsonStr); }

    JsonBase& operator[](const std::string& attr) override;

    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override;

private:
    std::unordered_map<std::string, jsonPtr> jsonObject_;

};

class JsonArray : public JsonBase {
public:
    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override;
    
private:
    std::vector<jsonPtr> jsonArray_;

};

class JsonString : public JsonBase {
public:
    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override { return "\"" + str_ + "\""; }
private:
    std::string str_;
};

class JsonNumber : public JsonBase {
    static const char mathChars[5];
public:
    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override { return std::to_string(jsonNumber_); }

private:
    double jsonNumber_;

    bool isMathSign(const std::string& jsonStr, size_t idx);
};



class JsonBoolean : public JsonBase {
public:
    JsonBoolean(bool jsonBool) : JsonBase(), jsonBoolean_(jsonBool) {}

    std::string toString(bool compact = true, int indent = 0) override { return jsonBoolean_ ? "true" : "false"; }

private:
    bool jsonBoolean_;
};

class JsonVoid : public JsonBase {
public:
    std::string toString(bool compact = true, int indent = 0) override { return "null"; }
};

template<typename T>
class Json : public JsonBase {
public:
    Json() = default;

    Json(const std::string& jsonStr) : JsonBase(jsonStr) {}

    T& entity() { return t_; }

private:
    T t_;
};



} // namespace elasticJeans

#endif // _ELASTICJEANS_JSON_H