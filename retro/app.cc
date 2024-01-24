// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#include "retro/app.h"

#include <iostream>

namespace retro {

void Application::Run() {
  screen_.Open();
  if (screen_.Width() < min_cols_ || screen_.Height() < min_rows_) {
    screen_.Close();
    std::cerr << "You're gonna need a bigger boat, er, window.  At least "
              << min_cols_ << " cols by " << min_rows_ <<
        " rows\n";
    return;
  }

  if (absl::Status status = Init(); !status.ok()) {
    std::cerr << status << std::endl;
    return;
  }

  curs_set(0); // Cursor off.

  // Register a callback to be called when a coroutine completes.  The
  // server keeps track of all coroutines created.
  // This deletes them when they are done.
  scheduler_.SetCompletionCallback(
      [this](co::Coroutine *c) { coroutines_.erase(c); });

  scheduler_.Run();
}

} // namespace retro
