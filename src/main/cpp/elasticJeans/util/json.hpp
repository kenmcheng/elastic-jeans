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

    virtual int parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos);

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

    virtual std::string toString() { return ""; }

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

    int invalid() {
        valid_ = false;
        return -1;
    }

    std::string parseQuoted(const std::string& jsonStr, size_t& from);

    bool skippable(const std::string& jsonStr, size_t idx);
};

class JsonObject : public JsonBase {
public:
    JsonObject() = default;

    JsonObject(const std::string& jsonStr) : JsonBase() { this->parse(jsonStr); }

    JsonBase& operator[](const std::string& attr) override;

    int parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString() override {
        std::ostringstream osstream;
        osstream << '{' << '\n';
        for (auto it = std::begin(jsonObject_); it != std::end(jsonObject_); it++) {
            auto& [k, val] = *it;
            osstream << "    \"" << k << "\":" << val->toString();
            if (std::next(it) != std::end(jsonObject_)) {
                osstream << ",";
            }
            osstream << "\n";
        }
        osstream << '}' << '\n';
        return osstream.str();
    }

private:
    std::unordered_map<std::string, jsonPtr> jsonObject_;

};

class JsonArray : public JsonBase {
public:
    int parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString() override {
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
private:
    std::vector<jsonPtr> jsonArray_;

};

class JsonString : public JsonBase {
public:
    int parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString() { return "\"" + str_ + "\""; }
private:
    std::string str_;
};

class JsonNumber : public JsonBase {
public:
    int parse(const std::string& jsonStr, size_t begin = 0, size_t end = std::string::npos) override;

    std::string toString() { return std::to_string(jsonNumber_); }

private:
    double jsonNumber_;
};



class JsonBoolean : public JsonBase {
public:
    JsonBoolean(bool jsonBool) : JsonBase(), jsonBoolean_(jsonBool) {}

    std::string toString() override { return jsonBoolean_ ? "true" : "false"; }

private:
    bool jsonBoolean_;
};

class JsonVoid : public JsonBase {
public:
    std::string toString() override { return "null"; }
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