// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include "absl/container/flat_hash_set.h"
#include "absl/status/status.h"
#include "co/coroutine.h"
#include "co/coroutine_cpp20.h"
#include "retro/screen.h"

#include <string>
#include <utility>

namespace retro {

class Application {
public:
  Application(int min_rows = 0, int min_cols = 0)
      : min_rows_(min_rows), min_cols_(min_cols), screen_(*this) {}
  virtual ~Application() = default;

  // Must be provided by the derived class to initialize the application.
  virtual absl::Status Init() = 0;
  virtual void Run();

  // Pause updates on the application's windows until Resumed.
  virtual void Pause() {}
  virtual void Resume() {}

  void AddCoroutine(std::unique_ptr<co::Coroutine> c) {
    coroutines_.insert(std::move(c));
  }

  co::CoroutineScheduler &Scheduler() { return scheduler_; }

protected:
  bool OpenAndInitialize();
  void CloseScreen();

  int min_rows_;
  int min_cols_;
  Screen screen_;
  co::CoroutineScheduler scheduler_;
  absl::flat_hash_set<std::unique_ptr<co::Coroutine>> coroutines_;
};

// C++20 coroutine application. This is intentionally parallel to Application:
// existing users keep the stackful scheduler API, while new users can opt into
// co_await without running two event loops.
class Cpp20Application : public Application {
public:
  using Application::Application;

  void Run() override;

  template <typename Func>
  co20::Coroutine *Spawn(Func &&func, const std::string &name = "",
                         int interrupt_fd = -1) {
    return cpp20_scheduler_.Spawn(std::forward<Func>(func), name, interrupt_fd);
  }

  co20::Scheduler &Cpp20Scheduler() { return cpp20_scheduler_; }

private:
  co20::Scheduler cpp20_scheduler_;
};

} // namespace retro
