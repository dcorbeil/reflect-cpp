#include <iostream>
#include <rfl.hpp>
#include <rfl/cbor.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_optional_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::Rename<"children", std::optional<std::vector<Person>>> children;
};

struct optional_test_t {
    int i;
    std::optional<int> opt_i;
    int j;
    std::optional<int> opt_j;
    double d;
    std::optional<double> opt_d;
};

struct optional_test_strings_t {
    std::string str;
    std::optional<std::string> opt_str;
};

struct optional_test_struct_t {
    int i;
    std::optional<optional_test_t> opt_struct;
};

TEST(cbor, test_simple_optional_fields) {

  const optional_test_t test =
    optional_test_t{
        .i = 42,
        .opt_i = 12345,
        .j = 69,
        .d = 42.69,
    };

  const optional_test_t actual = write_and_read_and_return(test);

  EXPECT_EQ(std::nullopt, actual.opt_j);
  EXPECT_EQ(std::nullopt, actual.opt_d);
}

TEST(cbor, test_simple_optional_fields_strings) {

  const optional_test_strings_t test =
    optional_test_strings_t{
      .str = "str"
    };

  const optional_test_strings_t actual = write_and_read_and_return(test);
  EXPECT_EQ(test.str, actual.str);
  EXPECT_EQ(std::nullopt, actual.opt_str);
}

TEST(cbor, test_simple_optional_fields_read) {

  const optional_test_t expected =
    optional_test_t{
        .i = 42,
        .opt_i = 12345,
        .j = 69,
        .d = 42.69,
    };

  std::vector<char> cbor = {
      (char)0xbf, (char)0x61, (char)0x69, (char)0x18, (char)0x2a,
      (char)0x65, (char)0x6f, (char)0x70, (char)0x74, (char)0x5f,
      (char)0x69, (char)0x19, (char)0x30, (char)0x39, (char)0x61,
      (char)0x6a, (char)0x18, (char)0x45, (char)0x61, (char)0x64,
      (char)0xfb, (char)0x40, (char)0x45, (char)0x58, (char)0x51,
      (char)0xeb, (char)0x85, (char)0x1e, (char)0xb8, (char)0xff
  };

  auto result = rfl::cbor::read<optional_test_t>(cbor);

  EXPECT_TRUE(result);

  optional_test_t actual = *result;

  EXPECT_EQ(expected.i, actual.i);
  EXPECT_EQ(expected.opt_i, actual.opt_i);
  EXPECT_EQ(expected.j, actual.j);
  EXPECT_EQ(expected.d, actual.d);

  EXPECT_EQ(std::nullopt, actual.opt_j);
  EXPECT_EQ(std::nullopt, actual.opt_d);
}

TEST(cbor, test_optional_fields) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  const auto homer_actual = write_and_read_and_return(homer);

  ASSERT_TRUE(homer_actual.children.get().has_value());
  EXPECT_EQ(std::nullopt, homer_actual.children.get().value()[0].children.get());
  EXPECT_EQ(std::nullopt, homer_actual.children.get().value()[1].children.get());
  EXPECT_EQ(std::nullopt, homer_actual.children.get().value()[2].children.get());
}

TEST(cbor, test_optional_fields_read) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  std::vector<char> homer_cbor = {
    (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
    (char)0x61, (char)0x6D, (char)0x65, (char)0x65, (char)0x48, (char)0x6F, (char)0x6D, (char)0x65,
    (char)0x72, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61,
    (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73,
    (char)0x6F, (char)0x6E, (char)0x68, (char)0x63, (char)0x68, (char)0x69, (char)0x6C, (char)0x64,
    (char)0x72, (char)0x65, (char)0x6E, (char)0x83, (char)0xBF, (char)0x69, (char)0x66, (char)0x69,
    (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x64,
    (char)0x42, (char)0x61, (char)0x72, (char)0x74, (char)0x68, (char)0x6C, (char)0x61, (char)0x73,
    (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69,
    (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xBF, (char)0x69,
    (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D,
    (char)0x65, (char)0x64, (char)0x4C, (char)0x69, (char)0x73, (char)0x61, (char)0x68, (char)0x6C,
    (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67,
    (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF,
    (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
    (char)0x61, (char)0x6D, (char)0x65, (char)0x66, (char)0x4D, (char)0x61, (char)0x67, (char)0x67,
    (char)0x69, (char)0x65, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E,
    (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70,
    (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xFF,
  };

  auto result = rfl::cbor::read<Person>(homer_cbor);

  EXPECT_TRUE(result && true) << "Test failed on read. Error: "
                              << result.error().what();
  Person actual = *result;
  EXPECT_EQ(actual.first_name.get(), homer.first_name.get());
  EXPECT_EQ(actual.last_name.get(), homer.last_name.get());

  ASSERT_TRUE(actual.children.get().has_value());
  EXPECT_EQ(std::nullopt, actual.children.get().value()[0].children.get());
  EXPECT_EQ(std::nullopt, actual.children.get().value()[1].children.get());
  EXPECT_EQ(std::nullopt, actual.children.get().value()[2].children.get());
}

TEST(cbor, optional_test_struct_absent) {

  const optional_test_struct_t expected =
    optional_test_struct_t{
      .i = 42,
    };

  std::vector<char> cbor = {
      (char)0xbf, (char)0x61, (char)0x69, (char)0x18, (char)0x2a,
      (char)0x65, (char)0x6f, (char)0x70, (char)0x74, (char)0x5f,
      (char)0x69, (char)0x19, (char)0x30, (char)0x39, (char)0x61,
      (char)0x6a, (char)0x18, (char)0x45, (char)0x61, (char)0x64,
      (char)0xfb, (char)0x40, (char)0x45, (char)0x58, (char)0x51,
      (char)0xeb, (char)0x85, (char)0x1e, (char)0xb8, (char)0xff
  };

  const optional_test_struct_t actual = write_and_read_and_return(expected);
  EXPECT_EQ(actual.i, expected.i);
  EXPECT_EQ(std::nullopt, expected.opt_struct);
}

TEST(cbor, optional_test_struct_present) {

  const optional_test_struct_t expected =
    optional_test_struct_t{
      .i = 42,
      .opt_struct = optional_test_t{
          .i = 42,
          .opt_i = 12345,
          .j = 69,
          .d = 42.69,
      }
    };

  const optional_test_struct_t actual = write_and_read_and_return(expected);
  EXPECT_EQ(actual.i, expected.i);
  EXPECT_EQ(actual.opt_struct->i, expected.opt_struct->i);
  EXPECT_EQ(actual.opt_struct->opt_i, expected.opt_struct->opt_i);
  EXPECT_EQ(actual.opt_struct->j, expected.opt_struct->j);
  EXPECT_EQ(actual.opt_struct->d, expected.opt_struct->d);
  EXPECT_EQ(std::nullopt, actual.opt_struct->opt_j);
  EXPECT_EQ(std::nullopt, actual.opt_struct->opt_d);
}

}  // namespace test_optional_fields
