// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#include "retro/table_window.h"
#include "retro/app.h"

namespace retro {

void TableWindow::Run() {
  Draw();
  App().AddCoroutine(std::make_unique<co::Coroutine>(
      Scheduler(), [this](co::Coroutine *c) { RunnerCoroutine(c); }));
}

void Cpp20TableWindow::Run() {
  Draw();
  auto *app = dynamic_cast<Cpp20Application *>(&App());
  if (app == nullptr) {
    DrawErrorBanner("Cpp20TableWindow requires Cpp20Application");
    return;
  }
  app->Spawn(
      [this](co20::Coroutine &c) -> co20::Task {
        co_await Runner(c);
        co_return;
      },
      "retro-table-window");
}

} // namespace retro
