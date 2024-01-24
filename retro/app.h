// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include "absl/container/flat_hash_set.h"
#include "absl/status/status.h"
#include "coroutine.h"
#include "retro/screen.h"

namespace retro {

class Application {
public:
  Application(int min_rows = 0, int min_cols = 0)
      : min_rows_(min_rows), min_cols_(min_cols), screen_(*this) {}
  virtual ~Application() = default;

  // Must be provided by the derived class to initialize the application.
  virtual absl::Status Init() = 0;
  void Run();

  // Pause updates on the application's windows until Resumed.
  virtual void Pause() {}
  virtual void Resume() {}

  void AddCoroutine(std::unique_ptr<co::Coroutine> c) {
    coroutines_.insert(std::move(c));
  }

  co::CoroutineScheduler &Scheduler() { return scheduler_; }

protected:
  int min_rows_;
  int min_cols_;
  Screen screen_;
  co::CoroutineScheduler scheduler_;
  absl::flat_hash_set<std::unique_ptr<co::Coroutine>> coroutines_;
};

} // namespace retro
