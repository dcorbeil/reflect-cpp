#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/cbor.hpp>
#include <string>

//FIXME: Have better names for T and Tt
template <class... Ps, class T, typename Tt = std::remove_cvref_t<T>>
Tt write_and_read_and_return(const T& _struct) {
  const auto serialized1 = rfl::cbor::write<Ps...>(_struct);
  const auto res = rfl::cbor::read<Tt, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::cbor::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
  return res.value();
}

template <class... Ps>
auto write_and_read(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::cbor::write<Ps...>(_struct);
  const auto res = rfl::cbor::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::cbor::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

#endif
