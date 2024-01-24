// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#include "retro/screen.h"

#include <locale.h>

namespace retro {

Screen::Screen(Application &app) : app_(app) {}

Screen::~Screen() { Close(); }

void Screen::Open() {
  setlocale(LC_ALL, "");
  win_ = initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  start_color();

  int background = COLOR_BLACK;

  init_pair(kColorPairBlack, COLOR_BLACK, background);
  init_pair(kColorPairRed, COLOR_RED, background);
  init_pair(kColorPairGreen, COLOR_GREEN, background);
  init_pair(kColorPairYellow, COLOR_YELLOW, background);
  init_pair(kColorPairBlue, COLOR_BLUE, background);
  init_pair(kColorPairMagenta, COLOR_MAGENTA, background);
  init_pair(kColorPairCyan, COLOR_CYAN, background);
  init_pair(kColorPairWhite, COLOR_WHITE, background);
  init_pair(kColorPairYes, COLOR_GREEN, background);
  init_pair(kColorPairNo, COLOR_RED, background);
  init_pair(kColorPairYesHighlight, COLOR_WHITE, COLOR_GREEN);
  init_pair(kColorPairNoHighlight, COLOR_WHITE, COLOR_RED);
  init_pair(kColorPairSelected, COLOR_BLACK, COLOR_WHITE);

  is_open_ = true;
}

void Screen::Close() {
  if (is_open_) {
    endwin();
  }
}

void Screen::PrintAt(int row, int col, const std::string &s, int color) {
  move(row, col);
  ColorOn(color);
  addstr(s.c_str());
  ColorOff(color);
  refresh();
}

void Screen::PrintInMiddle(int row, const std::string &s, int color) {
  int col = (Width() - s.size()) / 2;
  PrintAt(row, col, s, color);
}

int Screen::Width() const { return getmaxx(stdscr); }

int Screen::Height() const { return getmaxy(stdscr); }
} // namespace retro