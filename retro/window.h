// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include "retro/screen.h"
#include "toolbelt/pipe.h"
#include "retro/table.h"
#include "coroutine.h"

#include <string>

namespace retro {

class Application;

struct WindowOptions {
  std::string title;
  int nlines;
  int ncols;
  int y;
  int x;
};

// This is a basic curses window.  Just maps onto the data structures provided
// by ncurses and provides some wrappers for common things.
class Window {
public:
  Window(Screen *screen, WindowOptions opts);
  Window(Window *parent, WindowOptions opts);
  virtual ~Window();

  virtual void Draw(bool refresh = true);
  virtual void Run() {}
  virtual void ApplyFilter() {}

  void EraseCanvas() const;

  void Refresh() const { wrefresh(win_);}

  int Width() const { return opts_.ncols; }
  int Height() const { return opts_.nlines; }

  void PrintAt(int row, int col, const std::string &s, int color = kColorPairNormal) const;
  void PrintInMiddle(int row, const std::string &s, int color = kColorPairNormal) const ;
  void Print(const std::string& s) const {
    waddstr(win_, s.c_str());
  }

  void Move(int row, int col) const;

  void HLine(int row) const;
  void VLine(int col) const;

  void ColorOn(int color) const {
    if (color != kColorPairNormal) {
      wattron(win_, COLOR_PAIR(color) | A_BOLD);
    } else {
      wattron(win_, A_BOLD);
    }
  }

 void ColorOff(int color) const {
    if (color != kColorPairNormal) {
      wattroff(win_, COLOR_PAIR(color) | A_BOLD);
   } else {
      wattroff(win_, A_BOLD);
    }
  }

  Application& App() const {
    if (is_subwin_) {
      return parent_->App();
    }
    return screen_->App();
  }

  co::CoroutineScheduler& Scheduler();

  void Pause() { paused_ = true; }
  void Resume() { paused_ = false; }

  void SetFilter(const std::string& filter) {
    display_filter_ = filter;
  }

protected:
  friend class Panel;
  void PrintTitle() const;

  Application *app_;
  WINDOW *win_;
  WindowOptions opts_;
  std::string title_;
  union {
    Screen *screen_;
    Window *parent_;
  };
  bool is_subwin_;
  bool paused_ = false;
  std::string display_filter_;
};
} // namespace retro