#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <optional>
#include <rfl/cbor.hpp>
#include <rfl/json.hpp>
#include <type_traits>
#include <vector>


#include <thread>
#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

namespace canada_nlohmann_vs_rfl {

// ----------------------------------------------------------------------------

struct Property {
  std::string name;
};

struct Geometry {
  rfl::Literal<"Polygon"> type;
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
};

struct Feature {
  rfl::Literal<"Feature"> type;
  Property properties;
  Geometry geometry;
};

struct FeatureCollection {
  rfl::Literal<"FeatureCollection"> type;
  std::vector<Feature> features;
};

// ----------------------------------------------------------------------------

static FeatureCollection load_data() {
  return rfl::json::load<FeatureCollection>("benchmarks/data/canada.json")
      .value();
}

static std::vector<char> load_cbor_data() {
  // return rfl::io::load_bytes("benchmarks/data/canada.json").value();
  return rfl::cbor::write(load_data());
}


// ----------------------------------------------------------------------------
// nlohmann/json

std::vector<std::vector<std::tuple<double, double>>> nlohmann_to_coordinates(
    const nlohmann::json &_val);

Property nlohmann_to_property(const nlohmann::json &_val);

Geometry nlohmann_to_geometry(const nlohmann::json &_val);

Feature nlohmann_to_feature(const nlohmann::json &_val);

FeatureCollection nlohmann_to_feature_collection(const nlohmann::json &_val);

std::vector<std::vector<std::tuple<double, double>>> nlohmann_to_coordinates(
    const nlohmann::json &_val) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  for (const auto &arr1 : _val) {
    std::vector<std::tuple<double, double>> vec;
    for (const auto &arr2 : arr1) {
      std::tuple<double, double> tup;
      std::get<0>(tup) = arr2[0].template get<double>();
      std::get<1>(tup) = arr2[1].template get<double>();
      vec.emplace_back(std::move(tup));
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

Property nlohmann_to_property(const nlohmann::json &_val) {
  Property property;
  property.name = _val["name"].template get<std::string>();
  return property;
}

Geometry nlohmann_to_geometry(const nlohmann::json &_val) {
  Geometry geometry;
  geometry.type = _val["type"].template get<std::string>();
  geometry.coordinates = nlohmann_to_coordinates(_val["coordinates"]);
  return geometry;
}

Feature nlohmann_to_feature(const nlohmann::json &_val) {
  Feature feature;
  feature.type = _val["type"].template get<std::string>();
  feature.properties = nlohmann_to_property(_val["properties"]);
  feature.geometry = nlohmann_to_geometry(_val["geometry"]);
  return feature;
}

FeatureCollection nlohmann_to_feature_collection(const nlohmann::json &_val) {
  FeatureCollection feature_collection;
  feature_collection.type = _val["type"].template get<std::string>();
  for (const auto &val : _val["features"]) {
    feature_collection.features.push_back(nlohmann_to_feature(val));
  }
  return feature_collection;
}

static rfl::Result<FeatureCollection> read_using_nlohmann(
    const std::vector<char> &_cbor_bytes) {
  try {
    // auto val = nlohmann::json::parse(_json_string);
    auto val = nlohmann::json::from_cbor(_cbor_bytes.begin(), _cbor_bytes.end());
    return nlohmann_to_feature_collection(val);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// nlohmann::json features_to_nlohmann(const std::vector<Feature>& _features) {
//   nlohmann::json j;

//   for (const Feature& feat: _features) {
//     j["type"] = feat.type.name();
//   }

//   j["list"] = { 1, 0, 2 };
// }

// nlohmann::json feature_collection_to_nlohmann(const FeatureCollection& _feature_collection) {
//   nlohmann::json j;

//   j["type"] = _feature_collection.type.name();
//   j["features"] = features_to_nlohmann(_feature_collection.features);

//   return j;
// }

// static const std::vector<unsigned char> write_using_nlohmann(
//     const FeatureCollection& _feature_collection) {

//   nlohmann::json j;

//   j = feature_collection_to_nlohmann(_feature_collection);

//   return nlohmann::json::to_cbor(j);

// }


static void BM_canada_nlohmann_vs_rfl_read_nlohmann(benchmark::State &state) {
  const std::vector<char> cbor_bytes = load_cbor_data();
  // std::this_thread::sleep_for (std::chrono::seconds(1));
  // Benchmarking starts here
  for (auto _ : state) {
    // Read from cbor to struct
    const rfl::Result<FeatureCollection> res = read_using_nlohmann(cbor_bytes);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_nlohmann_vs_rfl_read_nlohmann);

// static void BM_canada_nlohmann_vs_rfl_write_nlohmann(benchmark::State &state) {
//   const FeatureCollection feature_collection = load_data();
//   // std::this_thread::sleep_for (std::chrono::seconds(1));
//   // Benchmarking starts here
//   for (auto _ : state) {
//     // Write from struct to cbor
//     const std::vector<unsigned char> output = write_using_nlohmann(feature_collection);
//     if (output.size() == 0) {
//       std::cout << "No output" << std::endl;
//     }
//   }
// }
// BENCHMARK(BM_canada_nlohmann_vs_rfl_write_nlohmann);

// ----------------------------------------------------------------------------
// reflect-cpp

static void BM_canada_nlohmann_vs_rfl_read_reflect(benchmark::State &state) {
  // const auto data = rfl::cbor::write(load_data());
  const std::vector<char> cbor_bytes = load_cbor_data();
  for (auto _ : state) {
    const auto res = rfl::cbor::read<FeatureCollection>(cbor_bytes);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_nlohmann_vs_rfl_read_reflect);

static void BM_canada_nlohmann_vs_rfl_write_reflect(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_canada_nlohmann_vs_rfl_write_reflect);


// ----------------------------------------------------------------------------

}  // namespace canada_nlohmann_vs_rfl

