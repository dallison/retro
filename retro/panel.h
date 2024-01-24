// Copyright 2024 David Allison
// All Rights Reserved
// See LICENSE file for licensing information.

#pragma once

#include <panel.h>
#include "retro/window.h"
#include "retro/screen.h"

namespace retro {

class Panel : public Window {
public:
  Panel(Screen *screen, WindowOptions opts);
  Panel(Window *parent, WindowOptions opts);
  ~Panel();

  virtual void Show();
  void Hide();

  void Draw(bool refresh = true) override;

private:
  PANEL* panel_;

};

}
