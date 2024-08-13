// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

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
  if (qipai.size() == 0u) {
    return {};
  }
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

void print(
  std::string const &uuid,
  unsigned chang,
  unsigned ju,
  unsigned ben,
  std::array<std::vector<std::uint_fast8_t>, 4u> const &qipai,
  std::vector<std::uint_fast8_t> const &paishan,
  std::array<std::vector<std::uint_fast8_t>, 4u> const &zimo,
  std::array<long, 4u> const &delta_scores)
{
  {
    bool has_zhuangjia = false;
    for (std::uint_fast8_t seat = 0u; seat < 4u; ++seat) {
      if (qipai[seat].size() == 0u) {
        if (seat != 3u) {
          IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
        }
        continue;
      }
      if (qipai[seat].size() != 13u && qipai[seat].size() != 14u) {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[seat].size();
      }
      if (qipai[seat].size() == 14u) {
        if (has_zhuangjia) {
          IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
        }
        has_zhuangjia = true;
      }
    }
    if (!has_zhuangjia) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
        << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
    }
  }

  if (paishan.size() != 83u && paishan.size() != 136u && paishan.size() != 68u && paishan.size() != 108u) {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
      << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << paishan.size();
  }

  std::cout << "{\"uuid\":\"" << uuid << "\",";
  std::cout << "\"chang\":" << chang << ",\"ju\":" << ju << ",\"ben\":" << ben << ",";
  if (paishan.size() == 83u || paishan.size() == 68u) {
    std::cout << "\"qipai\":[";
    std::cout << '[' << toString(qipai[0u]) << ']';
    std::cout << ",[" << toString(qipai[1u]) << ']';
    std::cout << ",[" << toString(qipai[2u]) << ']';
    if (paishan.size() == 83u) {
      if (qipai[3u].size() == 0u) {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
      }
      std::cout << ",[" << toString(qipai[3u]) << ']';
    }
    else if (qipai[3u].size() != 0u) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
        << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[3u].size();
    }
    std::cout << "],";
  }
  std::cout << "\"paishan\":[" << toString(paishan) << "],";
  std::cout << "\"zimo\":[";
  std::cout << '[' << toString(zimo[0u]) << ']';
  std::cout << ",[" << toString(zimo[1u]) << ']';
  std::cout << ",[" << toString(zimo[2u]) << ']';
  if (paishan.size() == 83u || paishan.size() == 136u) {
    std::cout << ",[" << toString(zimo[3u]) << ']';
  }
  else if (zimo[3u].size() != 0u) {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
      << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << zimo[3u].size();
  }
  std::cout << "],";
  std::cout << "\"delta_scores\":[";
  std::cout << delta_scores[0u];
  std::cout << ',' << delta_scores[1u];
  std::cout << ',' << delta_scores[2u];
  if (paishan.size() == 83u || paishan.size() == 136u) {
    std::cout << ',' << delta_scores[3u];
  }
  else if (delta_scores[3u] != 0) {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
      << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << delta_scores[3u];
  }
  std::cout << "]}\n";
}

void process(std::filesystem::path const &path, std::string uuid) {
  std::string const data = [&]() -> std::string {
    std::ifstream ifs(path, std::ios_base::in | std::ios_base::binary);
    if (!ifs) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to open.";
    }
    if (uuid.empty()) {
      for (std::size_t i = 0; i < 3u; ++i) {
        std::ifstream::int_type const c = ifs.get();
        if (c == std::ifstream::traits_type::eof()) {
          // Some files are empty.
          return "";
	}
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
  if (uuid.empty()) {
    wrapper.ParseFromString(data);
    if (wrapper.name() != "") {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": " << wrapper.name();
    }

    lq::ResGameRecord msg0;
    msg0.ParseFromString(wrapper.data());
    uuid = msg0.head().uuid();

    wrapper.ParseFromString(msg0.data());
  }
  else {
    wrapper.ParseFromString(data);
  }
  if (wrapper.name() != ".lq.GameDetailRecords") {
    // The version of game records before the maintenance on 2020/??/?? (JST).
    std::cerr << uuid << ": Failed to parse." << std::endl;
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
  std::size_t const record_size = [&]() -> std::size_t {
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

  unsigned chang;
  unsigned ju;
  unsigned ben;
  std::array<long, 4u> old_scores;
  std::array<std::vector<std::uint_fast8_t>, 4u> qipai;
  std::vector<std::uint_fast8_t> paishan;
  std::array<std::vector<std::uint_fast8_t>, 4u> zimo;
  std::array<bool, 4u> liqi_list;

  for (std::size_t record_count = 0u; record_count < record_size; ++record_count) {
    std::string const &r = [&]() -> std::string const & {
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
      continue;
    }
    else if (wrapper.name() == ".lq.RecordBaBei") {
      continue;
    }
    else if (wrapper.name() == ".lq.RecordChiPengGang") {
      lq::RecordChiPengGang record;
      record.ParseFromString(wrapper.data());

      if (record.has_liqi()) {
        unsigned const liqi_seat = record.liqi().seat();
        if (liqi_list[liqi_seat]) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << liqi_seat;
        }
        liqi_list[liqi_seat] = true;
      }
    }
    else if (wrapper.name() == ".lq.RecordDealTile") {
      lq::RecordDealTile record;
      record.ParseFromString(wrapper.data());

      if (record.has_liqi()) {
        unsigned const liqi_seat = record.liqi().seat();
        if (liqi_list[liqi_seat]) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << liqi_seat;
        }
        liqi_list[liqi_seat] = true;
      }

      unsigned const seat = record.seat();
      if (seat >= 4u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << seat;
      }
      std::string const tile_ = record.tile();
      unsigned const tile = decodeTile(tile_[0u], tile_[1u]);

      zimo[seat].push_back(tile);
      continue;
    }
    else if (wrapper.name() == ".lq.RecordDiscardTile") {
      continue;
    }
    else if (wrapper.name() == ".lq.RecordHule") {
      lq::RecordHule record;
      record.ParseFromString(wrapper.data());

      std::array<long, 4u> delta_scores;
      if (record.scores().size() == 4u) {
        delta_scores = {
          record.scores(0u) - old_scores[0u],
          record.scores(1u) - old_scores[1u],
          record.scores(2u) - old_scores[2u],
          record.scores(3u) - old_scores[3u],
        };
      }
      else {
        if (record.scores().size() != 3u) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << record.scores().size();
        }
        delta_scores = {
          record.scores(0u) - old_scores[0u],
          record.scores(1u) - old_scores[1u],
          record.scores(2u) - old_scores[2u],
          0,
        };
      }
      print(uuid, chang, ju, ben, qipai, paishan, zimo, delta_scores);

      for (auto &q : qipai) {
        q.clear();
      }
      paishan.clear();
      for (auto &z : zimo) {
        z.clear();
      }
      liqi_list.fill(false);
      continue;
    }
    else if (wrapper.name() == ".lq.RecordLiuJu") {
      lq::RecordLiuJu record;
      record.ParseFromString(wrapper.data());

      if (record.has_liqi()) {
        unsigned const liqi_seat = record.liqi().seat();
        if (liqi_list[liqi_seat]) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << liqi_seat;
        }
        liqi_list[liqi_seat] = true;
      }

      std::array<long, 4u> delta_scores;
      for (std::uint_fast8_t seat = 0u; seat < 4u; ++seat) {
        delta_scores[seat] = liqi_list[seat] ? -1000 : 0;
      }
      print(uuid, chang, ju, ben, qipai, paishan, zimo, delta_scores);

      for (auto &q : qipai) {
        q.clear();
      }
      paishan.clear();
      for (auto &z : zimo) {
        z.clear();
      }
      liqi_list.fill(false);
      continue;
    }
    else if (wrapper.name() == ".lq.RecordNewRound") {
      for (auto &q : qipai) {
        if (q.size() != 0u) {
          IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
        }
      }
      if (paishan.size() != 0u) {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
      }
      for (auto &z : zimo) {
        if (z.size() != 0u) {
          IS_MAJSOUL_FAIR_THROW<std::logic_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << ')';
        }
      }

      lq::RecordNewRound record;
      record.ParseFromString(wrapper.data());

      chang = record.chang();
      ju = record.ju();
      ben = record.ben();

      old_scores = {record.scores(0u), record.scores(1u), record.scores(2u), record.scores(3u)};

      std::vector<std::string> qipai0(record.tiles0().cbegin(), record.tiles0().cend());
      std::vector<std::string> qipai1(record.tiles1().cbegin(), record.tiles1().cend());
      std::vector<std::string> qipai2(record.tiles2().cbegin(), record.tiles2().cend());
      std::vector<std::string> qipai3(record.tiles3().cbegin(), record.tiles3().cend());

      qipai[0u] = restoreQipai(qipai0);
      if (qipai[0u].size() != 13u && qipai[0u].size() != 14u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[0u].size();
      }
      if ((ju == 0u) != (qipai[0u].size() == 14u)) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[0u].size();
      }
      qipai[1u] = restoreQipai(qipai1);
      if (qipai[1u].size() != 13u && qipai[1u].size() != 14u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[1u].size();
      }
      if ((ju == 1u) != (qipai[1u].size() == 14u)) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[1u].size();
      }
      qipai[2u] = restoreQipai(qipai2);
      if (qipai[2u].size() != 13u && qipai[2u].size() != 14u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[2u].size();
      }
      if ((ju == 2u) != (qipai[2u].size() == 14u)) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[2u].size();
      }
      qipai[3u] = restoreQipai(qipai3);
      if (qipai[3u].size() != 13u && qipai[3u].size() != 14u && qipai[3u].size() != 0u) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[3u].size();
      }
      if ((ju == 3u) != (qipai[3u].size() == 14u)) {
        IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
          << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << qipai[3u].size();
      }
      paishan = restorePaishan(record.paishan());
      if (game_record_version == 0u || game_record_version == 210715u) {
        if (paishan.size() != 83u && paishan.size() != 136u && paishan.size() != 68u && paishan.size() != 108u) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << uuid << ": " << paishan.size();
        }
      }
      else {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
      }

      liqi_list.fill(false);

      continue;
    }
    else if (wrapper.name() == ".lq.RecordNoTile") {
      lq::RecordNoTile record;
      record.ParseFromString(wrapper.data());

      std::array<long, 4u> delta_scores{0, 0, 0, 0};
      if (!record.liujumanguan()) {
        if (record.scores().size() != 1u) {
          IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
            << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << record.scores().size();
        }
        auto const &score = record.scores(0u);
        if (score.delta_scores().size() == 0u) {
          // Do nothing.
        }
        else {
          if (score.delta_scores().size() == 4u) {
            delta_scores = {
              score.delta_scores(0u),
              score.delta_scores(1u),
              score.delta_scores(2u),
              score.delta_scores(3u),
            };
          }
          else {
            if (score.delta_scores().size() != 3u) {
              IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
                << uuid << " (" << chang << '-' << ju << '-' << ben << "): "
                << score.delta_scores().size();
            }
            delta_scores = {
              score.delta_scores(0u),
              score.delta_scores(1u),
              score.delta_scores(2u),
              0,
            };
          }
        }
      }
      else {
        for (auto const &score : record.scores()) {
          if (score.seat() >= 4u) {
            IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
              << uuid << " (" << chang << '-' << ju << '-' << ben << "): " << score.seat();
          }
          if (score.delta_scores().size() != 4u) {
            IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1)
              << uuid << " (" << chang << '-' << ju << '-' << ben << "): "
              << score.delta_scores().size();
          }
          for (std::uint_fast8_t i = 0u; i < 4u; ++i) {
            delta_scores[i] += score.delta_scores(i);
          }
        }
      }
      for (std::uint_fast8_t seat = 0u; seat < 4u; ++seat) {
        delta_scores[seat] -= liqi_list[seat] ? 1000 : 0;
      }
      print(uuid, chang, ju, ben, qipai, paishan, zimo, delta_scores);

      for (auto &q : qipai) {
        q.clear();
      }
      paishan.clear();
      for (auto &z : zimo) {
        z.clear();
      }
      liqi_list.fill(false);
      continue;
    }
    else {
      std::cerr << uuid << ": An unsupported record `" << wrapper.name() << "`." << std::endl;
      continue;
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
        process(path, "");
        continue;
      }
      if (path_str.size() < suffix.size()) {
        continue;
      }
      if (path_str.compare(path_str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        std::string const filename = path.filename().string();
        process(path, filename.substr(0, filename.size() - suffix.size()));
        continue;
      }
    }
  }
}

} // namespace <anonymous>

int main(int const argc, char const * const * const argv) {
  if (argc >= 4) {
    std::cerr << "Usage: " << argv[0] << " <path prefix of game records> [SUFFIX]" << std::endl;
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
