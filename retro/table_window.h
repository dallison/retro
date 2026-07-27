// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include "co/coroutine_cpp20.h"
#include "retro/panel.h"

namespace retro {

class TableWindow : public Panel {
public:
  TableWindow(Screen *screen, WindowOptions opts,
              const std::vector<std::string> &titles)
      : Panel(screen, opts), display_table_(this, titles) {}
  TableWindow(Window *win, WindowOptions opts,
              const std::vector<std::string> &titles)
      : Panel(win, opts), display_table_(this, titles) {}
  ~TableWindow() = default;

  void Draw(bool refresh = true) override {
    if (paused_) {
      return;
    }
    Panel::Draw(refresh);
    display_table_.Draw();
  }

  void Run() override;

protected:
  Table display_table_;

private:
  virtual void RunnerCoroutine(co::Coroutine *c) = 0;
};

class Cpp20TableWindow : public Panel {
public:
  Cpp20TableWindow(Screen *screen, WindowOptions opts,
                   const std::vector<std::string> &titles)
      : Panel(screen, opts), display_table_(this, titles) {}
  Cpp20TableWindow(Window *win, WindowOptions opts,
                   const std::vector<std::string> &titles)
      : Panel(win, opts), display_table_(this, titles) {}
  ~Cpp20TableWindow() = default;

  void Draw(bool refresh = true) override {
    if (paused_) {
      return;
    }
    Panel::Draw(refresh);
    display_table_.Draw();
  }

  void Run() override;

protected:
  Table display_table_;

private:
  virtual co20::ValueTask<void> Runner(co20::Coroutine &c) = 0;
};
} // namespace retro