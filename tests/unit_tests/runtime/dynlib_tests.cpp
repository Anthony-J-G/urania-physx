#include <gtest/gtest.h>

#include <filesystem>
using namespace std::filesystem;

#include "runtime/dynlib/common_api.hpp"


class EngineDynlibTest : public testing::Test {
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() {
        // const auto lib_path = path(ENGINE_LIB_PATH);
        // EXPECT_EQ(exists(lib_path), true) << lib_path.c_str() << " " << current_path().c_str();
        engine_library_instance_ = new EngineLibrary{};

        // If `engine_library_instance_` is **not deleted** in `TearDownTestSuite()`,
        // reallocation should be prevented because `SetUpTestSuite()` may be called
        // in subclasses of FooTest and lead to memory leak.
        //
        // if (engine_library_instance_ == nullptr) {
        //   engine_library_instance_ = new ...;
        // }
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() {
        delete engine_library_instance_;
        engine_library_instance_ = nullptr;
    }

    // You can define per-test set-up logic as usual.
    void SetUp() override { 
        // ...
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {
        // ...
    }

  // Some expensive resource shared by all tests.
  static EngineLibrary* engine_library_instance_;
};

EngineLibrary* EngineDynlibTest::engine_library_instance_ = nullptr;

TEST_F(EngineDynlibTest, LibraryLoadUnload) {    
    // ... you can refer to engine_library_instance_ here ...
    LoadEngineLibrary(*engine_library_instance_);
    EXPECT_TRUE(engine_library_instance_->handle != nullptr);
    EXPECT_TRUE(engine_library_instance_->is_fully_loaded);

    UnloadEngineLibrary(*engine_library_instance_);
    EXPECT_TRUE(engine_library_instance_->handle == nullptr);
}

TEST_F(EngineDynlibTest, Test2) {
    // ... you can refer to engine_library_instance_ here ...
}



