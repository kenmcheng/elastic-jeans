#include <gtest/gtest.h>
#include <string>

#include <lattice/util/json.hpp>

#define ML_STR(s) #s

namespace lat = lattice;

TEST(JsonTest, UnmarshalString) {
    std::string s = ML_STR({
    "emoji": "😊😊😊😂🤣"
    });
    std::string expected = "😊😊😊😂🤣";

    lat::JsonObject json(s);

    EXPECT_EQ(expected, json["emoji"].toString());
}