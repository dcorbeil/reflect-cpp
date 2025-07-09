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

TEST(cbor, test_simple_optional_fields) {

  const optional_test_t test =
    optional_test_t{
        .i = 42,
        .opt_i = 12345,
        .j = 69,
        .d = 42.69,
    };

  // const optional_test_strings_t test_strings =
  //   optional_test_strings_t {
  //       .str = "non-optional string",
  //       .opt_str = "optional string",
  //   };

  // std::cout << std::hex << std::setw(2) << std::setfill('0') << std::hex;

  // rfl::cbor::write(test, std::cout << std::hex << std::setw(2) << std::setfill('0') << std::hex) << std::endl;
  // std::cout<<std::endl;
  // std::cout<<std::dec;

  auto cbor = rfl::cbor::write(test);

  for( char c: cbor ) {
      printf( "%02x", (uint8_t)c );
  }
  printf( "\n" );

  // write_and_read(test);
}

// TEST(cbor, test_simple_optional_fields_read) {

//   const optional_test_t expected =
//     optional_test_t{
//         .i = 42,
//         .opt_i = 12345,
//         .j = 69,
//         .d = 42.69,
//     };

//   std::vector<char> cbor = {
//       (char)0xbf, (char)0x61, (char)0x69, (char)0x18, (char)0x2a,
//       (char)0x65, (char)0x6f, (char)0x70, (char)0x74, (char)0x5f,
//       (char)0x69, (char)0x19, (char)0x30, (char)0x39, (char)0x61,
//       (char)0x6a, (char)0x18, (char)0x45, (char)0x61, (char)0x64,
//       (char)0xfb, (char)0x40, (char)0x45, (char)0x58, (char)0x51,
//       (char)0xeb, (char)0x85, (char)0x1e, (char)0xb8, (char)0xff
//   };

//   auto result = rfl::cbor::read<optional_test_t>(cbor);

//   EXPECT_TRUE(result);

//   optional_test_t actual = *result;

//   EXPECT_EQ(expected.i, actual.i);
//   EXPECT_EQ(expected.opt_i, actual.opt_i);
//   EXPECT_EQ(expected.j, actual.j);
//   EXPECT_EQ(expected.d, actual.d);

//   // write_and_read(test);
// }


//   // (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
//   // (char)0x61, (char)0x6D, (char)0x65, (char)0x65, (char)0x48, (char)0x6F, (char)0x6D, (char)0x65,
//   // (char)0x72, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61,
//   // (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73,
//   // (char)0x6F, (char)0x6E, (char)0x68, (char)0x63, (char)0x68, (char)0x69, (char)0x6C, (char)0x64,
//   // (char)0x72, (char)0x65, (char)0x6E, (char)0x83, (char)0xBF, (char)0x69, (char)0x66, (char)0x69,
//   // (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x64,
//   // (char)0x42, (char)0x61, (char)0x72, (char)0x74, (char)0x68, (char)0x6C, (char)0x61, (char)0x73,
//   // (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69,
//   // (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xBF, (char)0x69,
//   // (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D,
//   // (char)0x65, (char)0x64, (char)0x4C, (char)0x69, (char)0x73, (char)0x61, (char)0x68, (char)0x6C,
//   // (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67,
//   // (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF,
//   // (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
//   // (char)0x61, (char)0x6D, (char)0x65, (char)0x66, (char)0x4D, (char)0x61, (char)0x67, (char)0x67,
//   // (char)0x69, (char)0x65, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E,
//   // (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70,
//   // (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xFF,

// TEST(cbor, test_optional_fields) {
//   const auto bart = Person{.first_name = "Bart"};

//   const auto lisa = Person{.first_name = "Lisa"};

//   const auto maggie = Person{.first_name = "Maggie"};

//   const auto homer =
//       Person{.first_name = "Homer",
//              .children = std::vector<Person>({bart, lisa, maggie})};




//   std::vector<char> homer_cbor = {
//     (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
//     (char)0x61, (char)0x6D, (char)0x65, (char)0x65, (char)0x48, (char)0x6F, (char)0x6D, (char)0x65,
//     (char)0x72, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61,
//     (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73,
//     (char)0x6F, (char)0x6E, (char)0x68, (char)0x63, (char)0x68, (char)0x69, (char)0x6C, (char)0x64,
//     (char)0x72, (char)0x65, (char)0x6E, (char)0x83, (char)0xBF, (char)0x69, (char)0x66, (char)0x69,
//     (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x64,
//     (char)0x42, (char)0x61, (char)0x72, (char)0x74, (char)0x68, (char)0x6C, (char)0x61, (char)0x73,
//     (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69,
//     (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xBF, (char)0x69,
//     (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D,
//     (char)0x65, (char)0x64, (char)0x4C, (char)0x69, (char)0x73, (char)0x61, (char)0x68, (char)0x6C,
//     (char)0x61, (char)0x73, (char)0x74, (char)0x4E, (char)0x61, (char)0x6D, (char)0x65, (char)0x67,
//     (char)0x53, (char)0x69, (char)0x6D, (char)0x70, (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF,
//     (char)0xBF, (char)0x69, (char)0x66, (char)0x69, (char)0x72, (char)0x73, (char)0x74, (char)0x4E,
//     (char)0x61, (char)0x6D, (char)0x65, (char)0x66, (char)0x4D, (char)0x61, (char)0x67, (char)0x67,
//     (char)0x69, (char)0x65, (char)0x68, (char)0x6C, (char)0x61, (char)0x73, (char)0x74, (char)0x4E,
//     (char)0x61, (char)0x6D, (char)0x65, (char)0x67, (char)0x53, (char)0x69, (char)0x6D, (char)0x70,
//     (char)0x73, (char)0x6F, (char)0x6E, (char)0xFF, (char)0xFF,
//   };

//   auto result = rfl::cbor::read<Person>(homer_cbor);

//   EXPECT_TRUE(result && true) << "Test failed on read. Error: "
//                               << result.error().what();
//   Person actual = *result;
//   EXPECT_EQ(actual.first_name.get(), homer.first_name.get());
//   EXPECT_EQ(actual.last_name.get(), homer.last_name.get());

//   // write_and_read(
//   //     homer,
//   //     R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson"},{"firstName":"Lisa","lastName":"Simpson"},{"firstName":"Maggie","lastName":"Simpson"}]})");
// }

}  // namespace test_optional_fields
