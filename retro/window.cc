// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#include "retro/window.h"
#include "absl/strings/str_format.h"
#include "retro/app.h"

namespace retro {

Window::Window(Screen *screen, WindowOptions opts)
    : opts_(opts), title_(opts.title), screen_(screen), is_subwin_(false) {
  win_ = newwin(opts.nlines, opts.ncols, opts.y, opts.x);
}

Window::Window(Window *parent, WindowOptions opts)
    : opts_(opts), title_(opts.title), parent_(parent), is_subwin_(true) {
  win_ = subwin(parent->win_, opts.nlines, opts.ncols, opts.y, opts.x);
}

Window::~Window() { delwin(win_); }

co::CoroutineScheduler &Window::Scheduler() { return App().Scheduler(); }

void Window::PrintTitle() const {
  std::string title =
      display_filter_.empty()
          ? absl::StrFormat(" %s ", title_)
          : absl::StrFormat(" %s (filter: %s) ", title_, display_filter_);
  PrintInMiddle(0, title, kColorPairGreen);
}

void Window::Draw(bool refresh) {
  if (paused_) {
    return;
  }
  EraseCanvas();
  wborder(win_, 0, 0, 0, 0, 0, 0, 0, 0);
  PrintTitle();
  if (refresh) {
    Refresh();
  }
}

void Window::EraseCanvas() const { werase(win_); }

void Window::PrintAt(int row, int col, const std::string &s, int color) const {
  ColorOn(color);
  mvwaddstr(win_, row, col, s.c_str());
  ColorOff(color);
}

void Window::PrintInMiddle(int row, const std::string &s, int color) const {
  int col = (Width() - s.size()) / 2;
  PrintAt(row, col, s, color);
}

void Window::Move(int row, int col) const { wmove(win_, row, col); }

void Window::HLine(int row) const {
  wmove(win_, row, 0);
  waddch(win_, ACS_LTEE);
  whline(win_, ACS_HLINE, Width() - 2);
  wmove(win_, row, Width() - 1);
  waddch(win_, ACS_RTEE);
}

void Window::VLine(int col) const {
  wmove(win_, 0, col);
  waddch(win_, ACS_TTEE);
  wvline(win_, ACS_VLINE, Height() - 2);
  wmove(win_, Height() - 1, col);
  waddch(win_, ACS_BTEE);
}

  void Window::DrawErrorBanner(const std::string& text) {
    EraseCanvas();
    Draw(false);
    int middle_row = Height() / 2;
    int middle_col = Width() / 2;
    PrintAt(middle_row, middle_col - text.size() / 2, text, kColorPairRed);
    Refresh();
  }


} // namespace retro