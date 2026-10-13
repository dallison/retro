// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include "co/coroutine_cpp20.h"
#include "retro/panel.h"

namespace retro {

// A dialog that asks a yes or no question.
class YesNoDialog : public Panel {
public:
  YesNoDialog(Screen *screen, WindowOptions opts, const std::string &yes,
              const std::string &no);

  bool GetUserInput(const std::string &prompt, co::Coroutine *c);
  co20::ValueTask<bool> GetUserInput(const std::string &prompt,
                                     co20::Coroutine &c);

private:
  std::string yes_;
  std::string no_;
};

// A dialog that displays some information and waits for the user to
// hit return for OK.
class InfoDialog : public Panel {
public:
  InfoDialog(Screen *screen, WindowOptions opts, const std::string &ok);

  void WaitForUser(const std::vector<std::string> &text, co::Coroutine *c);
  co20::ValueTask<void> WaitForUser(const std::vector<std::string> &text,
                                    co20::Coroutine &c);

private:
  std::string ok_;
};

// A dialog that asks the user for textual input.
class UserInputDialog : public Panel {
public:
  UserInputDialog(Screen *screen, WindowOptions opts, const std::string &ok);

  std::string GetUserInput(const std::string &prompt, co::Coroutine *c);
  co20::ValueTask<std::string> GetUserInput(const std::string &prompt,
                                            co20::Coroutine &c);

private:
  std::string ok_;
};

// A dialog that presents a vertical set of options and allows the user to
// select one.
class SelectionDialog : public Panel {
public:
  SelectionDialog(Screen *screen, WindowOptions opts, const std::string &ok,
                  const std::string &cancel);

  int GetSelection(const std::vector<std::string> &options, co::Coroutine *c);
  co20::ValueTask<int> GetSelection(const std::vector<std::string> &options,
                                    co20::Coroutine &c);

private:
  std::string ok_;
  std::string cancel_;
};

} // namespace retro
