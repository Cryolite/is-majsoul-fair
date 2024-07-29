#include "common/mahjongsoul.pb.h"
#include "common/throw.hpp"
#include <regex>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <cstddef>


namespace {

using std::placeholders::_1;

std::uint_fast8_t decodeTile(char const number, char const color)
{
  if (number < '0' || '9' < number) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << number;
  }
  if (color != 'm' && color != 'p' && color != 's' && color != 'z') {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << color;
  }

  std::uint_fast8_t const number_ = number - '0';
  std::uint_fast8_t const color_ = [](char const c) -> std::uint_fast8_t {
    switch (c) {
    case 'm':
      return 0u;
    case 'p':
      return 10u;
    case 's':
      return 20u;
    case 'z':
      return 29u;
    default:
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
  }(color);

  std::uint_fast8_t const encode = color_ + number_;
  if (encode >= 37u) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << encode;
  }

  return encode;
}

std::vector<std::uint_fast8_t> restoreQipai(std::vector<std::string> const &qipai)
{
  if (qipai.size() != 14u && qipai.size() != 13u) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << qipai.size();
  }

  auto encode = [](std::string const &tile) -> std::uint_fast8_t {
    if (tile.size() != 2u) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << tile.size();
    }
    return decodeTile(tile[0u], tile[1u]);
  };

  std::vector<std::uint_fast8_t> result;
  result.reserve(qipai.size());
  std::transform(qipai.cbegin(), qipai.cend(), std::back_inserter(result), encode);

  return result;
}

std::vector<std::uint_fast8_t> restorePaishan(std::string const &paishan)
{
  std::vector<std::uint_fast8_t> result;
  result.reserve(paishan.size() / 2u);
  for (std::string::const_iterator iter = paishan.cbegin(); iter < paishan.cend(); iter += 2) {
    std::uint_fast8_t const tile = decodeTile(*iter, *(iter + 1u));
    result.push_back(tile);
  }

  return result;
}

std::string toString(std::vector<std::uint_fast8_t> const &tiles)
{
  std::ostringstream oss;
  bool is_first = true;
  for (std::uint_fast8_t const tile : tiles) {
    if (!is_first) {
      oss << ',';
    }
    oss << static_cast<unsigned>(tile);
    is_first = false;
  }
  return std::move(oss).str();
}

void process(std::filesystem::path const &ph) {
  std::string const data = [&ph]() -> std::string {
    std::ifstream ifs(ph, std::ios_base::in | std::ios_base::binary);
    for (std::size_t i = 0; i < 3u; ++i) {
      std::ifstream::int_type const c = ifs.get();
      if (c == std::ifstream::traits_type::eof()) {
        // Some files are empty.
        return "";
      }
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return std::move(oss).str();
  }();
  if (data.empty()) {
    // Skip empty files.
    return;
  }

  lq::Wrapper wrapper;
  wrapper.ParseFromString(data);
  if (wrapper.name() != "") {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << ph << ": " << wrapper.name();
  }

  lq::ResGameRecord msg0;
  msg0.ParseFromString(wrapper.data());

  std::uint_fast32_t const start_time = msg0.head().start_time();
  std::string const uuid = msg0.head().uuid();

  wrapper.ParseFromString(msg0.data());
  if (wrapper.name() != ".lq.GameDetailRecords") {
    // The version of game records before the maintenance on 2020/??/?? (JST).
    return;
  }

  lq::GameDetailRecords msg1;
  msg1.ParseFromString(wrapper.data());

  std::uint_fast32_t const game_record_version = msg1.version();
  if (game_record_version == 0u) {
    // The version of game records before the maintenance on 2021/07/28 (JST).
  }
  else if (game_record_version == 210715u) {
    // The version of game records after the maintenance on 2021/07/28 (JST).
  }
  else {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
      << game_record_version << ": An unsupported game record version.";
  }

  auto const &records_0 = msg1.records();
  auto const &records_210715 = msg1.actions();
  std::size_t const record_size = [&]() -> std::size_t
  {
    if (game_record_version == 0u) {
      if (records_210715.size() != 0u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>("A broken data.");
      }
      return records_0.size();
    }
    if (game_record_version == 210715u) {
      if (records_0.size() != 0u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>("A broken data.");
      }
      return records_210715.size();
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }();

  char chang;
  std::size_t ju;
  std::size_t ben;

  for (std::size_t record_count = 0u; record_count < record_size; ++record_count) {
    std::string const &r = [&]() -> std::string const &
    {
      if (game_record_version == 0u) {
        return records_0[record_count];
      }
      if (game_record_version == 210715u) {
        return records_210715[record_count].result();
      }
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }();
    if (game_record_version == 210715u && r.empty()) {
      continue;
    }
    wrapper.ParseFromString(r);
    if (wrapper.name() == ".lq.RecordAnGangAddGang") {
      lq::RecordAnGangAddGang record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordChiPengGang") {
      lq::RecordChiPengGang record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordDealTile") {
      lq::RecordDealTile record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordDiscardTile") {
      lq::RecordDiscardTile record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordHule") {
      lq::RecordHule record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordLiuJu") {
      lq::RecordLiuJu record;
      record.ParseFromString(wrapper.data());
    }
    else if (wrapper.name() == ".lq.RecordNewRound") {
      lq::RecordNewRound record;
      record.ParseFromString(wrapper.data());
      switch (record.chang()) {
      case 0u:
        chang = 'E';
        break;
      case 1u:
        chang = 'S';
        break;
      case 2u:
        chang = 'W';
        break;
      default:
        break;
      }
      ju = record.ju() + 1;
      ben = record.ben();

      std::vector<std::string> qipai0;
      std::vector<std::string> qipai1;
      std::vector<std::string> qipai2;
      std::vector<std::string> qipai3;
      switch (record.ju()) {
      case 0:
        qipai0.assign(record.tiles0().cbegin(), record.tiles0().cend());
        qipai1.assign(record.tiles1().cbegin(), record.tiles1().cend());
        qipai2.assign(record.tiles2().cbegin(), record.tiles2().cend());
        qipai3.assign(record.tiles3().cbegin(), record.tiles3().cend());
        break;
      case 1:
        qipai0.assign(record.tiles1().cbegin(), record.tiles1().cend());
        qipai1.assign(record.tiles2().cbegin(), record.tiles2().cend());
        qipai2.assign(record.tiles3().cbegin(), record.tiles3().cend());
        qipai3.assign(record.tiles0().cbegin(), record.tiles0().cend());
        break;
      case 2:
        qipai0.assign(record.tiles2().cbegin(), record.tiles2().cend());
        qipai1.assign(record.tiles3().cbegin(), record.tiles3().cend());
        qipai2.assign(record.tiles0().cbegin(), record.tiles0().cend());
        qipai3.assign(record.tiles1().cbegin(), record.tiles1().cend());
        break;
      case 3:
        qipai0.assign(record.tiles3().cbegin(), record.tiles3().cend());
        qipai1.assign(record.tiles0().cbegin(), record.tiles0().cend());
        qipai2.assign(record.tiles1().cbegin(), record.tiles1().cend());
        qipai3.assign(record.tiles2().cbegin(), record.tiles2().cend());
        break;
      default:
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ", " << chang << ju << '-' << ben;
      }

      std::vector<std::uint_fast8_t> const qipai0_ = restoreQipai(qipai0);
      if (qipai0_.size() != 14u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << qipai0_.size();
      }
      std::vector<std::uint_fast8_t> const qipai1_ = restoreQipai(qipai1);
      if (qipai1_.size() != 13u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << qipai1_.size();
      }
      std::vector<std::uint_fast8_t> const qipai2_ = restoreQipai(qipai2);
      if (qipai2_.size() != 13u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << qipai2_.size();
      }
      std::vector<std::uint_fast8_t> const qipai3_ = restoreQipai(qipai3);
      if (qipai3_.size() != 13u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << qipai3_.size();
      }
      std::vector<std::uint_fast8_t> const paishan = restorePaishan(record.paishan());
      if (game_record_version == 0u || game_record_version == 210715u) {
        if (paishan.size() != 83u && paishan.size() != 136u) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << paishan.size();
        }
      }
      else {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
      }

      std::cout << start_time
                << '\t' << uuid
                << '\t' << toString(qipai0_)
                << '\t' << toString(qipai1_)
                << '\t' << toString(qipai2_)
                << '\t' << toString(qipai3_)
                << '\t' << toString(paishan)
                << '\n';
      return;
    }
    else if (wrapper.name() == ".lq.RecordNoTile") {
      lq::RecordNoTile record;
      record.ParseFromString(wrapper.data());
    }
    else {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(wrapper.name());
    }
  }
}

void walk(std::filesystem::path const &ph, std::string const &suffix) {
  auto const end = std::filesystem::directory_iterator();
  for (std::filesystem::directory_iterator iter(ph); iter != end; ++iter) {
    if (iter->is_directory()) {
      walk(iter->path(), suffix);
      continue;
    }
    if (iter->is_regular_file()) {
      std::filesystem::path const path = iter->path();
      std::string const path_str = path.string();
      if (suffix.empty()) {
        process(path);
        continue;
      }
      if (path_str.size() < suffix.size()) {
        continue;
      }
      if (path_str.compare(path_str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        process(path);
        continue;
      }
    }
  }
}

} // namespace <anonymous>

int main(int const argc, char const * const * const argv) {
  if (argc >= 4) {
    std::cerr << "Usage: " << argv[0] << " [SUFFIX]" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string const suffix = [&]() -> std::string {
    if (argc == 3) {
      return argv[2];
    }
    return "";
  }();

  std::filesystem::path ph(argv[1]);
  walk(ph, suffix);

  return EXIT_SUCCESS;
}
