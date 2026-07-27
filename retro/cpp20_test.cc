#include "retro/app.h"

#include "gtest/gtest.h"

#include <chrono>
#include <unistd.h>

namespace retro {
namespace {

class TestApplication : public Cpp20Application {
public:
  absl::Status Init() override { return absl::OkStatus(); }
};

co20::ValueTask<int> NestedValue(co20::Coroutine &c, int value) {
  co_await c.Yield();
  co_return value + 1;
}

co20::ValueTask<int> OuterValue(co20::Coroutine &c, int value) {
  co_return co_await NestedValue(c, value);
}

TEST(Cpp20ApplicationTest, SpawnsAndCompletesNestedValueTask) {
  TestApplication app;
  int result = 0;

  app.Spawn(
      [&result](co20::Coroutine &c) -> co20::Task {
        result = co_await OuterValue(c, 41);
        co_return;
      },
      "nested-value");

  app.Cpp20Scheduler().Run();
  EXPECT_EQ(result, 42);
}

TEST(Cpp20ApplicationTest, WaitsForFileDescriptor) {
  TestApplication app;
  int pipe_fds[2];
  ASSERT_EQ(pipe(pipe_fds), 0);
  char received = '\0';

  app.Spawn(
      [&](co20::Coroutine &c) -> co20::Task {
        int fd = co_await c.Wait(pipe_fds[0], POLLIN);
        if (fd == pipe_fds[0]) {
          (void)read(pipe_fds[0], &received, 1);
        }
        co_return;
      },
      "reader");
  app.Spawn(
      [&](co20::Coroutine &c) -> co20::Task {
        co_await c.Yield();
        const char value = 'x';
        (void)write(pipe_fds[1], &value, 1);
        co_return;
      },
      "writer");

  app.Cpp20Scheduler().Run();
  close(pipe_fds[0]);
  close(pipe_fds[1]);
  EXPECT_EQ(received, 'x');
}

TEST(Cpp20ApplicationTest, SupportsWaitTimeout) {
  TestApplication app;
  int pipe_fds[2];
  ASSERT_EQ(pipe(pipe_fds), 0);
  bool timed_out = false;

  app.Spawn(
      [&](co20::Coroutine &c) -> co20::Task {
        int fd = co_await c.Wait(pipe_fds[0], POLLIN, 2'000'000ULL);
        timed_out = fd != pipe_fds[0];
        co_return;
      },
      "timeout");

  app.Cpp20Scheduler().Run();
  close(pipe_fds[0]);
  close(pipe_fds[1]);
  EXPECT_TRUE(timed_out);
}

} // namespace
} // namespace retro
