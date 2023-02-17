#include <gtest/gtest.h>
#include "util.hpp"

// Verify Null
TEST(VerifyNull, GoodNull)
{
    EXPECT_FALSE(util::verify_null("Null")) << "util::verify_null() returned an error for argument \"Null\"";
    EXPECT_FALSE(util::verify_null("null")) << "util::verify_null() returned an error for argument \"null\"";
}

TEST(VerifyNull, StartsWithNButNotNull)
{
    EXPECT_TRUE(util::verify_null("Nill")) << "util::verify_null() did not return an error when one was expected";
}

// Verify bool
TEST(VerifyBool, GoodBool)
{
    EXPECT_FALSE(util::verify_bool("True")) << "util::verify_bool() returned an error for argument \"True\"";
    EXPECT_FALSE(util::verify_bool("true")) << "util::verify_bool() returned an error for argument \"true\"";
    EXPECT_FALSE(util::verify_bool("False")) << "util::verify_bool() returned an error for argument \"False\"";
    EXPECT_FALSE(util::verify_bool("false")) << "util::verify_bool() returned an error for argument \"false\"";
}

TEST(VerifyBool, StartsWithBButNotNull)
{
    EXPECT_TRUE(util::verify_bool("Three")) << "util::verify_bool() did not return an error when one was expected";
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
    EXPECT_TRUE(util::verify_bool("Foobar")) << "util::verify_number() did not return an error for argument \"Foobar\"";
}

// Verify String
TEST(VerifyString, GoodString)
{
    // Normal String
    EXPECT_FALSE(util::verify_string(R"("This is a normal string")")) << R"(util::verify_string() returned an error for argument "This is a normal string")";
    // String containing quotes
    EXPECT_FALSE(util::verify_string(R"("This \"string\" contains escaped quotes")")) << R"(util::verify_string() returned an error for argument "This \"string\" contains escaped quotes")";
    // String containing escaped characters
    EXPECT_FALSE(util::verify_string(R"("\\\/\b\f\n\r\t\ua13d")")) << R"(util::verify_string() returned an error for argument "\\\/\b\f\n\r\t\ua13d")";
    // String containing non-ascii unicode
    EXPECT_FALSE(util::verify_string(R"("∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),

  ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),

  2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm")") << R"(util::verify_string() returned an error for argument "∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),

  ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),

  2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm")";
}

TEST(VerifyString, BadString)
{
    // Missing Initial Quote
    EXPECT_TRUE(util::verify_string(R"(This string is missing the initial quote")")) << R"(util::verify_string() did not return an error for argument This string is missing the initial quote")";
    // Missing Final Quote
    EXPECT_TRUE(util::verify_string(R"("This string is missing the final quote)")) << R"(util::verify_string() did not return an error for argument "This string is missing the final quote)";
    // Bad Escape Character
    EXPECT_TRUE(util::verify_string(R"("\q")")) << R"(util::verify_string() did not return an error for argument "\q")";
    // Bad Unicode character
    EXPECT_TRUE(util::verify_string(R"(\u11g4)")) << R"(util::verify_string() did not return an error for argument "\u11g4")";
}

// Verify Array
TEST(VerifyArray, GoodArray)
{
    // Standard Array
    EXPECT_FALSE(util::verify_array("[6.022e23, \"foobar\", true, null]")) << "util::verify_array() returned an error for argument [6.022e23, \"foobar\", true, null]";
    // Empty Array
    EXPECT_FALSE(util::verify_array("[]")) << "util::verify_array() returned an error for argument []";
    // Array containing array
    EXPECT_FALSE("[\"foobar\", [1, 2, 3, 4, 5], 3.14159]") << "util::verify_array() returned an error for argument [\"foobar\", [1, 2, 3, 4, 5], 3.14159]";
}

TEST(VerifyArray, BadArray)
{
    // Missing Initial Bracket
    EXPECT_TRUE(util::verify_array("\"This\", \"array\", \"is\", \"missing\", \"the\", \"inital\", \"brace\"]")) << "util::verify_array() did not return an error for argument \"This\", \"array\", \"is\", \"missing\", \"the\", \"inital\", \"brace\"]";
    // Missing Final Bracket
    EXPECT_TRUE(util::verify_array("\"This\", \"array\", \"is\", \"missing\", \"the\", \"final\", \"brace\"]")) << "util::verify_array() did not return an error for argument \"This\", \"array\", \"is\", \"missing\", \"the\", \"final\", \"brace\"]";
    // Missing Comma
    EXPECT_TRUE(util::verify_array("[1, 2, 3, 4 5, 6, 7]")) << "util::verify_array() did not return an error for argument [1, 2, 3, 4 5, 6, 7]";
    // Missing Field
    EXPECT_TRUE(util::verify_array("[1, 2, 3, , 5, 6]")) << "util::verify_array() did not return an error for argument [1, 2, 3, , 5, 6]";
}

// Verify Object
TEST(VerifyObject, GoodObject)
{
    // Standard Object
    EXPECT_FALSE(util::verify_object(R"({"String Field": "Foobar", "Number Field": 6.022e23, "Array Field": ["Lorem", "ipsum", "dolor", "sit", "amet", consectetur", "adipiscing", "elit."], "Boolean Field": false})")) << R"(util::verify_object() returned an error for argument {"String Field": "Foobar", "Number Field": 6.022e23, "Array Field": ["Lorem", "ipsum", "dolor", "sit", "amet", consectetur", "adipiscing", "elit."], "Boolean Field": false})";
    // empty Object
    EXPECT_FALSE(util::verify_object(R"({})")) << R"(util::verify_object() returned an error for argument {})";
    // Object containing Object
    EXPECT_FALSE(util::verify_object(R"({"Key": "Val", "Object Field": {"key 1": "subfield 1", "key 2": "subfield 2"}, "Key 3", 12345})")) << R"(util::verify_object() returned an error for argument {"Key": "Val", "Object Field": {"key 1": "subfield 1", "key 2": "subfield 2"}, "Key 3", 12345})";
}

TEST(VerifyObject, BadObject)
{
    // Missing Initial Brace
    EXPECT_TRUE(util::verify_object(R"("Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})")) << R"(util::verify_object() did not return an error for argument "Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})";
    // Missing Final Brace
    EXPECT_TRUE(util::verify_object(R"({"Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3")")) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2", "Key 3": "Object 3")";
    // Missing Key
    EXPECT_TRUE(util::verify_object(R"("Key 1": "Object 1", "Key 2": "Object 2", : "Object 3")")) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2", : "Object 3"})";
    // Missing Value
    EXPECT_TRUE(util::verify_object(R"({"Key 1": "Object 1", "Key 2": , "Key 3": "Object 3"})")) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": , "Key 3": "Object 3"})";
    // Missing Field
    EXPECT_TRUE(util::verify_object(R"({"Key 1": "Object 1",, "Key 3": "Object 3"})")) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1",, "Key 3": "Object 3"})";
    // Missing Colon
    EXPECT_TRUE(util::verify_object(R"({"Key 1" "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})")) << R"(util::verify_object() did not return an error for argument {"Key 1" "Object 1", "Key 2": "Object 2", "Key 3": "Object 3"})";
    // Missing Comma
    EXPECT_TRUE(util::verify_object(R"({"Key 1": "Object 1", "Key 2": "Object 2" "Key 3": "Object 3"})")) << R"(util::verify_object() did not return an error for argument {"Key 1": "Object 1", "Key 2": "Object 2" "Key 3": "Object 3"})";
}
