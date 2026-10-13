# retro

Curses-based application library with both legacy stackful coroutines and
C++20 `co_await` support.

Existing applications continue to derive from `retro::Application` and use
`co::Coroutine`. New applications derive from `retro::Cpp20Application`:

```cpp
class App : public retro::Cpp20Application {
 public:
  absl::Status Init() override {
    Spawn([](co20::Coroutine &c) -> co20::Task {
      co_await c.Sleep(std::chrono::milliseconds(100));
      co_return;
    });
    return absl::OkStatus();
  }
};
```

Use `retro::Cpp20TableWindow` for asynchronous tables. Dialogs provide
overloads that accept `co20::Coroutine&` and return `co20::ValueTask<T>`.
