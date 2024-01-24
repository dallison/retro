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

} // namespace retro
