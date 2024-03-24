#include <gtest/gtest.h>
#include <string>

#include <elasticJeans/util/json.hpp>

#define ML_STR(s) #s

namespace ej = elasticJeans;

TEST(JsonTest, UnmarshalString) {
    std::string s = ML_STR({
    "emoji": "😊😊😊😂🤣"
    });
    std::string expected = "😊😊😊😂🤣";

    ej::JsonObject json(s);

    EXPECT_EQ(expected, json["emoji"].toString());
}