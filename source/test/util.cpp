#include <gtest/gtest.h>
#include "util.hpp"
#include <optional>

// Verify Null
TEST(VerifyNull, GoodNull)
{
    EXPECT_FALSE(util::verify_null("Null")) << "util::verify_null() returned an error for argument \"Null\"";
    EXPECT_FALSE(util::verify_null("null")) << "util::verify_null() returned an error for argument \"null\"";
}

TEST(VerifyNull, StartsWithNButNotNull)
{
    std::optional<std::string> result = util::verify_null("Nill");
    EXPECT_TRUE(result) << "util::verify_null() did not return an error for argument \"Nill\"";
    EXPECT_EQ(result.value(), "Argument does not match [Nn]ull") << "returned error did not match expected";
}

// Verify bool
TEST(VerifyBool, GoodBool)
{
    EXPECT_FALSE(util::verify_bool("True")) << "util::verify_bool() returned an error for argument \"True\"";
    EXPECT_FALSE(util::verify_bool("true")) << "util::verify_bool() returned an error for argument \"true\"";
    EXPECT_FALSE(util::verify_bool("False")) << "util::verify_bool() returned an error for argument \"False\"";
    EXPECT_FALSE(util::verify_bool("false")) << "util::verify_bool() returned an error for argument \"false\"";
}

TEST(VerifyBool, StartsWithTorFButNotBool)
{
    std::optional<std::string> result = util::verify_bool("Three");
    EXPECT_TRUE(result) << "util::verify_bool() did not return an error for argument \"Three\"";
    EXPECT_EQ(result.value(), "Argument does not match [Tt]rue or [Ff]alse") << "returned error did not match expected";
    result = util::verify_bool("Four");
    EXPECT_TRUE(result) << "util::verify_bool() did not return an error for argument \"Four\"";
    EXPECT_EQ(result.value(), "Argument does not match [Tt]rue or [Ff]alse") << "returned error did not match expected";
}

// Verify Number
TEST(VerifyNumber, GoodNumber)
{
    // this would basically be a test of std::stod(), so I'm just going to assume it works
    // only other thing to test is the return from the std::optional
    EXPECT_FALSE(util::verify_number("3.14159265358979")) << "util::verify_number() returned an error for argument \"3.14159265358979\"";
}

TEST(VerifyNumber, NotANumber)
{
    std::optional<std::string> result = util::verify_number("Foobar");
    EXPECT_TRUE(result) << "util::verify_number() did not return an error for argument \"Foobar\"";
    EXPECT_EQ(result.value(), "Argument could not be parsed as a number") << "returned error did not match expected";
}

// Verify String
TEST(VerifyString, GoodString)
{
    // Normal String
    EXPECT_FALSE(util::verify_string(R"("This is a normal string")")) << R"(util::verify_string() returned an error for argument "This is a normal string")";
    // String containing quotes
    EXPECT_FALSE(util::verify_string(R"("This \"string\" contains escaped quotes")")) << R"(util::verify_string() returned an error for argument "This \"string\" contains escaped quotes")";
    // String containing escaped characters
    EXPECT_FALSE(util::verify_string(R"("\\\/\"\b\f\n\r\t\ua13d")")) << R"(util::verify_string() returned an error for argument "\\\/\"\b\f\n\r\t\ua13d")";
    // String containing non-ascii unicode
    EXPECT_FALSE(util::verify_string(R"("∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),

  ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),

  2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm")")) << R"(util::verify_string() returned an error for argument "∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),

  ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),

  2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm")";
}

TEST(VerifyString, BadString)
{
    // Fully empty string
    std::optional<std::string> result = util::verify_string("");
    EXPECT_TRUE(result) << "util::verify_string() did not return an error for an empty string argument";
    EXPECT_EQ(result.value(), "String is less than empty") << "returned error did not match expected";
    // Missing Initial Quote
    result = util::verify_string(R"(This string is missing the initial quote")");
    EXPECT_TRUE(result) << R"(util::verify_string() did not return an error for argument This string is missing the initial quote")";
    EXPECT_EQ(result.value(), "String missing initial quote") << "returned error did not match expected";
    // Missing Final Quote
    result = util::verify_string(R"("This string is missing the final quote)");
    EXPECT_TRUE(result) << R"(util::verify_string() did not return an error for argument "This string is missing the final quote)";
    EXPECT_EQ(result.value(), "String missing final quote") << "returned error did not match expected";
    result = util::verify_string(R"("This string "contains\" an unescaped quote")");
    // Bad Escape Character
    result = util::verify_string(R"("\q")");
    EXPECT_TRUE(result) << R"(util::verify_string() did not return an error for argument "\q")";
    EXPECT_EQ(result.value(), "Non escape character after '\\'") << "returned error did not match expected";
    // Bad Unicode character
    result = util::verify_string(R"("\u11g4")");
    EXPECT_TRUE(result) << R"(util::verify_string() did not return an error for argument "\u11g4")";
    EXPECT_EQ(result.value(), "Non hex character in \\uXXXX escape sequence") << "returned error did not match expected";
}

// Verify Array
TEST(VerifyArray, GoodArray)
{
    // Standard Array
    EXPECT_FALSE(util::verify_array("[6.022e23, \"foobar\", true, null]")) << "util::verify_array() returned an error for argument [6.022e23, \"foobar\", true, null]";
    // Empty Array
    EXPECT_FALSE(util::verify_array("[]")) << "util::verify_array() returned an error for argument []";
    // Array containing array
    EXPECT_FALSE(util::verify_array("[\"foobar\", [1, 2, 3, 4, 5], 3.14159]")) << "util::verify_array() returned an error for argument [\"foobar\", [1, 2, 3, 4, 5], 3.14159]";
    // TODO: add tests for odd whitespace
}

TEST(VerifyArray, BadArray)
{
    // Missing Initial Bracket
    std::optional<std::string> result = util::verify_array("\"This\", \"array\", \"is\", \"missing\", \"the\", \"inital\", \"brace\"]");
    EXPECT_TRUE(result) << "util::verify_array() did not return an error for argument \"This\", \"array\", \"is\", \"missing\", \"the\", \"inital\", \"brace\"]";
    EXPECT_EQ(result.value(), "Array string missing opening brace") << "returned error did not match expected";
    // Missing Final Bracket
    result = util::verify_array("[\"This\", \"array\", \"is\", \"missing\", \"the\", \"final\", \"brace\"");
    EXPECT_TRUE(result) << "util::verify_array() did not return an error for argument [\"This\", \"array\", \"is\", \"missing\", \"the\", \"final\", \"brace\"";
    EXPECT_EQ(result.value(), "Array string missing closing brace") << "Returned error did not match expected";
    // Missing Comma
    result = util::verify_array("[1, 2, 3, 4 5, 6, 7]");
    EXPECT_TRUE(result) << "util::verify_array() did not return an error for argument [1, 2, 3, 4 5, 6, 7]";
    EXPECT_EQ(result.value(), "Error in field: Whitespace in number field, possibly missing field delimiter") << "Returned error did not match expected";
    // Missing Field
    result = util::verify_array("[1, 2, 3, , 5, 6]");
    EXPECT_TRUE(result) << "util::verify_array() did not return an error for argument [1, 2, 3, , 5, 6]";
    EXPECT_EQ(result.value(), "Array contains empty field") << "Returned error did not match expected";
}

// Verify Object
TEST(VerifyObject, GoodObject)
{
    // Standard Object
    EXPECT_FALSE(util::verify_object(R"({"String Field": "Foobar", "Number Field": 6.022e23, "Array Field": ["Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit."], "Boolean Field": false})")) << R"(util::verify_object() returned an error for argument {"String Field": "Foobar", "Number Field": 6.022e23, "Array Field": ["Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit."], "Boolean Field": false})";
    // empty Object
    EXPECT_FALSE(util::verify_object(R"({})")) << R"(util::verify_object() returned an error for argument {})";
    // Object containing Object
    EXPECT_FALSE(util::verify_object(R"({"Key": "Val", "Object Field": {"key 1": "subfield 1", "key 2": "subfield 2"}, "Key 3": 12345})")) << R"(util::verify_object() returned an error for argument {"Key": "Val", "Object Field": {"key 1": "subfield 1", "key 2": "subfield 2"}, "Key 3": 12345})";
    // TODO: add tests for odd whitespace
}

TEST(VerifyObject, BadObject)
{
    // Missing Initial Brace
    std::optional<std::string> result = util::verify_object(R"("Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument "Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})";
    EXPECT_EQ(result.value(), "Object string missing opening brace") << "Returned error did not match expected"; 
    // Missing Final Brace
    result = util::verify_object(R"({"Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3")");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3")";
    EXPECT_EQ(result.value(), "Object string missing closing brace") << "Returned error did not match expected";
    // Missing Key
    result = util::verify_object(R"({"Key 1": "Object 1", "Key 2": "Object 2", : "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2", : "Object 3"})";
    EXPECT_EQ(result.value(), "Field is missing key") << "Returned error did not match expected";
    // Missing Value
    result = util::verify_object(R"({"Key 1": "Object 1", "Key 2": , "Key 3": "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": , "Key 3": "Object 3"})";
    EXPECT_EQ(result.value(), "Field is missing value") << "Returned error did not match expected";
    // Missing Field
    result = util::verify_object(R"({"Key 1": "Object 1",, "Key 3": "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1",, "Key 3": "Object 3"})";
    EXPECT_EQ(result.value(), "Object contains empty field") << "Returned error did not match expected";
    // Missing Colon
    result = util::verify_object(R"({"Key 1" "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1" "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})";
    EXPECT_EQ(result.value(), "':' delimiter missing between key and value") << "Returned error did not match expected";
    // Missing Comma
    result = util::verify_object(R"({"Key 1": "Object 1", "Key 2": "Object 2" "Key 3": "Object 3"})");
    EXPECT_TRUE(result) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2" "Key 3": "Object 3"})";
    EXPECT_EQ(result.value(), "Error in value: Unescaped quote in string") << "Returned error did not match expected";
}
