// =========================================================================
// Copyright 2021 -- present Liam Huang (Yuuki) [liamhuang0205@gmail.com].
// Author: Liam Huang
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =========================================================================
#include "countdown/countdown.h"

#include <catch.h>

#include <chrono>
#include <future>
#include <string>

TEST_CASE("constructors") {
  REQUIRE_NOTHROW(yuuki::Countdown());
}

TEST_CASE("observers") {
  yuuki::Countdown cd;
  REQUIRE(cd.running());
}

TEST_CASE("modifiers") {
  yuuki::Countdown cd;
  REQUIRE(cd.running());

  SECTION("terminate") {
    cd.terminate();
    REQUIRE_FALSE(cd.running());
  }

  SECTION("terminate then reset") {
    cd.terminate();
    REQUIRE_FALSE(cd.running());
    cd.reset();
    REQUIRE(cd.running());
  }
}

TEST_CASE("mutable observer") {
  yuuki::Countdown cd;
  REQUIRE(cd.running());

  SECTION("timeout") {
    REQUIRE(cd.wait_for(std::chrono::seconds(1)));
    REQUIRE(cd.running());
  }

  SECTION("terminate") {
    auto f = std::async(std::launch::async, [&]() -> bool { return cd.wait_for(std::chrono::seconds(1)); });
    cd.terminate();
    REQUIRE_FALSE(f.get());
    REQUIRE_FALSE(cd.running());
  }

  SECTION("terminate then reset") {
    auto f = std::async(std::launch::async, [&]() -> bool { return cd.wait_for(std::chrono::seconds(1)); });
    cd.terminate();
    REQUIRE_FALSE(f.get());
    REQUIRE_FALSE(cd.running());

    cd.reset();
    REQUIRE(cd.running());
    REQUIRE(cd.wait_for(std::chrono::seconds(1)));
    REQUIRE(cd.running());
  }

  SECTION("timer check") {
    auto begin = std::chrono::steady_clock::now();
    REQUIRE(cd.wait_for(std::chrono::milliseconds(1000)));
    auto end = std::chrono::steady_clock::now();
    REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() ==
            Approx(1000).epsilon(0.01));
  }
}
