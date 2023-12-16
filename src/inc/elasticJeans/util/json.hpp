#ifndef _ELASTICJEANS_JSON_H
#define _ELASTICJEANS_JSON_H

#include <elasticJeans/util/serializable.hpp>

#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>

#include <sstream>

namespace elasticJeans {

enum class JsonElement {
    Base,
    Object,
    Array,
    String,
    Number,
    Boolean,
    Void
};

class JsonBase {
    
public:
    using jsonPtr = std::shared_ptr<JsonBase>;

    JsonBase() = default;

    virtual ~JsonBase() {};

    JsonBase(const std::string& jsonStr) { this->parse(jsonStr); }

    virtual JsonBase& operator[](const std::string& attr);

    virtual JsonBase& operator[](size_t idx);

    JsonBase& operator=(const std::string& attr);

    virtual size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos);

    bool valid() { return valid_; }

    virtual std::optional<jsonPtr> at(const std::string& attr);

    virtual std::optional<jsonPtr> at(size_t idx);

    template<typename... Args>
    std::optional<jsonPtr> at(const std::string& attr, Args... args) {
        auto rtn = this->at(attr);
        if (rtn.has_value()) {
            return rtn->get()->at(args...);
        }
        return rtn;
    }

    template<typename... Args>
    std::optional<jsonPtr> at(int idx, Args... args) {
        auto rtn = this->at(idx);
        if (rtn.has_value()) {
            return rtn->get()->at(args...);
        }
        return rtn;
    }

    virtual std::string toString(bool compact = true, int indent = 0) { return serialize(compact, indent, false); }

    virtual std::string jsonize(bool compact = true, int indent = 0) { return serialize(compact, indent, true); }

    virtual std::string serialize(bool compact = true, int indent = 0, bool convertJson = false) { return ""; }
protected:
    bool valid_ = true;
    bool isObject_ = false;
    bool isArray_ = false;
    bool isString_ = false;
    bool isBoolean_ = false;
    bool isNumeric_ = false;
    bool isInteger_ = false;
    bool isDouble_ = false;
    bool isNull_ = false;

    JsonElement element_ = JsonElement::Base;

    static const char whitespaces[4];
    static const char openningChars[3];
    static const char closingChars[3];

    size_t invalid() {
        valid_ = false;
        return std::string::npos;
    }

    std::string parseQuoted(const std::string& jsonStr, size_t& from);

    std::string processString(const std::string& str, bool convertEscape);

    std::string jsonizeString(const std::string& str);

    bool skippable(const std::string& jsonStr, size_t idx);

    bool isOpening(const std::string& jsonStr, size_t idx);

    bool isClosing(const std::string& jsonStr, size_t idx);
};

class JsonObject : public JsonBase {
public:
    JsonObject() : JsonBase() {
        isObject_ = true;
        element_ = JsonElement::Object;
    }

    JsonObject(const std::string& jsonStr) : JsonObject() { this->parse(jsonStr); }

    JsonBase& operator[](const std::string& attr) override;

    std::optional<jsonPtr> at(const std::string& attr) override;

    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override;

    std::string jsonize(bool compact = true, int indent = 0) override;

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = false) override;

    template<typename T>
    void add(const std::string& attr, T value) {
        jsonObject_[attr] = std::make_shared<T>(value);
    }

private:
    std::unordered_map<std::string, jsonPtr> jsonObject_;

};

class JsonArray : public JsonBase {
public:
    JsonArray() : JsonBase() {
        isArray_ = true;
        element_ = JsonElement::Array;
    }

    JsonBase& operator[](size_t idx) override;

    std::optional<jsonPtr> at(size_t idx) override;

    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = true) override;

    template<typename T>
    void append(T value) {
        jsonArray_.push_back(std::make_shared<T>(value));
    }

private:
    std::vector<jsonPtr> jsonArray_;

};

class JsonString : public JsonBase {
public:
    JsonString() : JsonBase() {
        isString_ = true;
        element_ = JsonElement::String;
    }

    JsonString(const std::string& str) : JsonString() { this->str_ = str; }

    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString(bool compact = true, int indent = 0) override { return serialize(compact, indent, false); }

    std::string jsonize(bool compact = true, int indent = 0) override { return serialize(compact, indent, true); }

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = true) override {
        return convertJson ? "\"" + jsonizeString(str_) + "\"" : "\"" + str_ + "\"";
    }

private:
    std::string str_;

};

class JsonNumber : public JsonBase {
    static const char mathChars[5];
public:
    JsonNumber() : JsonNumber(0) {}

    JsonNumber(double number) : JsonBase(), jsonNumber_(number) {
        isNumeric_ = true;
        isDouble_ = true;
        element_ = JsonElement::Number;
    }

    size_t parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = true) override { return std::to_string(jsonNumber_); }

private:
    double jsonNumber_;

    bool isMathSign(const std::string& jsonStr, size_t idx);
};



class JsonBoolean : public JsonBase {
public:
    JsonBoolean() : JsonBoolean(false) {}

    JsonBoolean(bool jsonBool) : JsonBase(), jsonBoolean_(jsonBool) {
        isBoolean_ = true;
        element_ = JsonElement::Boolean;
    }

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = true) override { return jsonBoolean_ ? "true" : "false"; }

private:
    bool jsonBoolean_;
};

class JsonVoid : public JsonBase {
public:
    JsonVoid() : JsonBase() {
        isNull_ = true;
        element_ = JsonElement::Void;
    }

    std::string serialize(bool compact = true, int indent = 0, bool convertJson = true) override { return "null"; }
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