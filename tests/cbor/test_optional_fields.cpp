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

  bool operator==(const Person& other) const {
    return first_name.get() == other.first_name.get() &&
           last_name.get() == other.last_name.get() &&
           children.get() == other.children.get();
  }
};

struct OptionalPerson {
    std::optional<Person> opt_person;
};

struct Empty{};

struct optional_test_t {
    int i;
    std::optional<int> opt_i;
    int j;
    std::optional<int> opt_j;
    double d;
    std::optional<double> opt_d;

    bool operator==(const optional_test_t& other) const = default;
};

struct optional_test_strings_t {
    std::string str;
    std::optional<std::string> opt_str;
};

struct optional_test_struct_t {
    std::optional<optional_test_t> opt_struct;
};

struct optional_test_array_t {
    std::array<std::optional<optional_test_t>, 4> opt_array;
};

TEST(cbor, test_optional_fields_array_optional_fields) {
  const optional_test_array_t expected = {
      .opt_array = {
          std::nullopt,
          optional_test_t{.i = 42, .opt_i = 12345, .j = 69, .d = 42.69},
          std::nullopt,
          optional_test_t{.i = 24, .opt_i = 54321, .j = 96, .d = 69.24},
      }};

  const auto actual = write_and_read_and_return(expected);

  EXPECT_FALSE(actual.opt_array[0]);
  EXPECT_EQ(expected.opt_array[1], actual.opt_array[1]);
  EXPECT_FALSE(actual.opt_array[2]);
  EXPECT_EQ(expected.opt_array[2], actual.opt_array[2]);
  EXPECT_EQ(expected.opt_array[3], actual.opt_array[3]);
}

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

  auto cbor = rfl::cbor::write(test);

  for( char c: cbor ) {
      printf( "%02x", (uint8_t)c );
  }
  printf( "\n" );
}

TEST(cbor, test_simple_optional_fields_strings) {

  const optional_test_strings_t test =
    optional_test_strings_t{
      .str = "str"
    };

  const optional_test_strings_t actual = write_and_read_and_return(test);
  EXPECT_EQ(test.str, actual.str);
  EXPECT_EQ(std::nullopt, actual.opt_str);

  auto cbor = rfl::cbor::write(test);

  for( char c: cbor ) {
      printf( "%02x", (uint8_t)c );
  }
  printf( "\n" );
}

TEST(cbor, test_simple_optional_fields_strings2) {

  const optional_test_strings_t test =
    optional_test_strings_t{
      .str = "str",
      .opt_str = "std::nullopt nah just kidding",
    };

  const optional_test_strings_t actual = write_and_read_and_return(test);
  EXPECT_EQ(test.str, actual.str);
  EXPECT_EQ("std::nullopt nah just kidding", *actual.opt_str);

  auto cbor = rfl::cbor::write(test);

  for( char c: cbor ) {
      printf( "%02x", (uint8_t)c );
  }
  printf( "\n" );
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

TEST(cbor, optional_test_struct_present) {

  const optional_test_struct_t expected =
    optional_test_struct_t{
      .opt_struct = optional_test_t{
          .i = 42,
          .opt_i = 12345,
          .j = 69,
          .d = 42.69,
      }
    };

  const optional_test_struct_t actual = write_and_read_and_return(expected);
  EXPECT_EQ(actual.opt_struct->i, expected.opt_struct->i);
  EXPECT_EQ(actual.opt_struct->opt_i, expected.opt_struct->opt_i);
  EXPECT_EQ(actual.opt_struct->j, expected.opt_struct->j);
  EXPECT_EQ(actual.opt_struct->d, expected.opt_struct->d);
  EXPECT_EQ(std::nullopt, actual.opt_struct->opt_j);
  EXPECT_EQ(std::nullopt, actual.opt_struct->opt_d);
}

TEST(cbor, test_optional_no_fields) {
  auto empty_struct_cbor = rfl::cbor::write(Empty{});
  auto no_person_cbor = rfl::cbor::write(OptionalPerson{});

  EXPECT_EQ(empty_struct_cbor.size(), no_person_cbor.size());
}

TEST(cbor, test_optional_fields) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  const OptionalPerson homer_optional = {
    .opt_person = homer
  };

  const auto homer_optional_cbor = rfl::cbor::write(homer_optional);
  const auto res = rfl::cbor::read<OptionalPerson>(homer_optional_cbor);

  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                              << res.error().what();

  const auto actual_homer = res.value();

  EXPECT_TRUE(actual_homer.opt_person.has_value());
  EXPECT_EQ(*actual_homer.opt_person, homer);
}

}  // namespace test_optional_fields
