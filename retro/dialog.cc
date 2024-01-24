// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#include "retro/dialog.h"

namespace retro {

YesNoDialog::YesNoDialog(Screen *screen, WindowOptions opts,
                         const std::string &yes, const std::string &no)
    : Panel(screen, opts), yes_(yes), no_(no) {}

bool YesNoDialog::GetUserInput(const std::string &prompt, co::Coroutine *c) {
  int yes_col = Width() / 4;
  int no_col = Width() * 3 / 4;
  int prompt_row = 2;
  int button_row = Height() - 2;

  Draw(false);
  PrintAt(prompt_row, (Width() - prompt.size()) / 2, prompt);
  PrintAt(button_row, yes_col, yes_, kColorPairYesHighlight);
  PrintAt(button_row, no_col, no_, kColorPairNo);
  Refresh();

  bool yes_selected = true;
  for (;;) {
    c->Wait(STDIN_FILENO, POLLIN);
    int ch = getch();
    switch (ch) {
    case KEY_RIGHT:
    case KEY_LEFT:
    case KEY_UP:
    case KEY_DOWN:
    case '\x09':
      // Swap selected button.
      if (yes_selected) {
        PrintAt(button_row, yes_col, yes_, kColorPairYes);
        PrintAt(button_row, no_col, no_, kColorPairNoHighlight);
      } else {
        PrintAt(button_row, yes_col, yes_, kColorPairYesHighlight);
        PrintAt(button_row, no_col, no_, kColorPairNo);
      }
      Refresh();
      yes_selected = !yes_selected;
      break;

    case '\033':
      // Escape is the same as selecting no.
      Hide();
      return false;
    case '\n':
    case '\r':
      // Newline or return return selected button.
      Hide();
      return yes_selected;
    }
  }
}

InfoDialog::InfoDialog(Screen *screen, WindowOptions opts,
                       const std::string &ok)
    : Panel(screen, opts), ok_(ok) {}

void InfoDialog::WaitForUser(const std::vector<std::string> &text,
                             co::Coroutine *c) {
  int ok_col = Width() / 2;
  int text_row = 2;
  int button_row = Height() - 2;

  Draw(false);
  for (auto &t : text) {
    PrintAt(text_row, 2, t);
    text_row++;
  }
  PrintAt(button_row, ok_col, ok_, kColorPairOk);
  Refresh();

  for (;;) {
    c->Wait(STDIN_FILENO, POLLIN);
    int ch = getch();
    switch (ch) {
    case '\033':
    case '\n':
    case '\r':
      Hide();
      return;
    }
  }
}

UserInputDialog::UserInputDialog(Screen *screen, WindowOptions opts,
                                 const std::string &ok)
    : Panel(screen, opts), ok_(ok) {}

std::string UserInputDialog::GetUserInput(const std::string &prompt,
                                          co::Coroutine *c) {
  int ok_col = Width() / 2;
  int prompt_row = 2;
  int button_row = Height() - 2;
  int input_row = 3;

  curs_set(1);      // Cursor on.

  Draw(false);
  PrintAt(prompt_row, 2, prompt);
  PrintAt(button_row, ok_col, ok_, kColorPairOk);

  Move(input_row, 2);
  Refresh();

  std::string str;
  int col = 2;
  for (;;) {
    c->Wait(STDIN_FILENO, POLLIN);
    int ch = getch();
    switch (ch) {
    case '\033':
      return "";
    case '\n':
    case '\r':
      curs_set(0);
      return str;
    case 127:
    case 8:
      // Backspace or delete.
      if (str.empty()) {
        beep();
        break;
      }
      str = str.substr(0, str.size() - 1);
      col--;
      wmove(win_, input_row, col);
      waddch(win_, ' ');
      wmove(win_, input_row, col);
      wrefresh(win_);
      break;

    default:
      if (ch >= ' ' && ch < 127) {
        if (col >= Width() - 2) {
          beep();
          break;
        }
        str += char(ch);
        wmove(win_, input_row, col);
        waddch(win_, ch);
        wrefresh(win_);
        col++;
        break;
      }
    }
  }
}

SelectionDialog::SelectionDialog(Screen *screen, WindowOptions opts,
                                 const std::string &ok,
                                 const std::string &cancel)
    : Panel(screen, opts), ok_(ok), cancel_(cancel) {}

int SelectionDialog::GetSelection(const std::vector<std::string> &options,
                                  co::Coroutine *c) {
  int ok_col = Width() / 4;
  int cancel_col = Width() * 3 / 4;
  int button_row = Height() - 2;

  bool ok_selected = true;
  int option_selected = 0;

  auto redraw = [&]() {
    Draw(false);
    int options_row = 2;
    int index = 0;
    for (auto &t : options) {
      PrintAt(options_row, 2, t,
              index == option_selected ? kColorPairSelected : kColorPairNormal);
      options_row++;
      index++;
    }

    PrintAt(button_row, ok_col, ok_,
            ok_selected ? kColorPairYesHighlight : kColorPairYes);
    PrintAt(button_row, cancel_col, cancel_,
            ok_selected ? kColorPairNo : kColorPairNoHighlight);
    Refresh();
  };

  redraw();

  for (;;) {
    c->Wait(STDIN_FILENO, POLLIN);
    int ch = getch();
    switch (ch) {
    case KEY_UP:
      if (option_selected == 0) {
        option_selected = options.size() - 1;
      } else {
        option_selected--;
      }
      redraw();
      break;

    case KEY_DOWN:
    case '\x09':
      if (option_selected == options.size() - 1) {
        option_selected = 0;
      } else {
        option_selected++;
      }
      redraw();
      break;

    case KEY_RIGHT:
    case KEY_LEFT:
      // Swap selected button.
      ok_selected = !ok_selected;
      redraw();
      break;

    case '\033':
      // Escape is the same as selecting cancel.
      Hide();
      return -1;
    case '\n':
    case '\r':
      // Newline or return return selected option if ok was selected.
      Hide();
      if (ok_selected) {
        return option_selected;
      }
      return -1;
    }
  }
}

} // namespace retro
