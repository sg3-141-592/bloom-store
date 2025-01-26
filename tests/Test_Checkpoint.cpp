#include <gtest/gtest.h>

#include "../Utilities/Checkpoint.h"

class CheckpointTest : public testing::Test
{
protected:
    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(CheckpointTest, CreateLockFile)
{
    std::string checkpointPath = "/tmp/checkpoint.lock";
    Checkpoint checkpoint(checkpointPath);

    ASSERT_TRUE(true);
}

TEST_F(CheckpointTest, TryAndAccessLockMultipleTimes)
{
    std::string checkpointPath = "/tmp/checkpoint_multiple.lock";
    Checkpoint checkpoint(checkpointPath);

    Checkpoint checkpoint2(checkpointPath);

    try {
        Checkpoint checkpoint2(checkpointPath);
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        ASSERT_STREQ("Unable to acquire exclusive lock on checkpoint file", e.what());
    } catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}