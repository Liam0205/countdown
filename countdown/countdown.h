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
#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>

namespace yuuki {
class Countdown final {
 public:
#if __cplusplus >= 201703L
  using shared_mutex = std::shared_mutex;
#else
  using shared_mutex = std::shared_timed_mutex;
#endif

 private:
  bool running_{true};
  mutable shared_mutex mutex_;
  mutable std::condition_variable_any cv_;

 public:
  Countdown() = default;
  ~Countdown() {
    terminate();
  }

 private:
  Countdown(const Countdown&) = delete;
  Countdown& operator=(const Countdown&) = delete;
  Countdown(Countdown&&) = delete;
  Countdown& operator=(Countdown&&) = delete;

 public:  // modifiers
  void reset() {
    std::unique_lock<shared_mutex> lock(mutex_);
    running_ = true;
  }

  void terminate() {
    std::unique_lock<shared_mutex> lock(mutex_);
    running_ = false;
    cv_.notify_all();
  }

 public:  // observers
  bool running() const {
    std::shared_lock<shared_mutex> lock(mutex_);
    return running_;
  }

 public:  // mutable observers
  template <typename Rep, typename Peroid>
  bool wait_for(const std::chrono::duration<Rep, Peroid>& duration) const {
    std::unique_lock<shared_mutex> lock(mutex_);
    return !cv_.wait_for(lock, duration, [&] { return !running_; });
  }
};
}  // namespace yuuki
