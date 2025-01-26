#include <fstream>

#include <gtest/gtest.h>

#include "../Config.h"

class ConfigTest : public testing::Test
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

TEST_F(ConfigTest, ValidConfig)
{
    std::string validConfigPath = "/tmp/valid_config.ini";
    std::string validConfig =
        "[General]\n"
        "queue_size = 1028\n"
        "\n"
        "[Source]\n"
        "path = artists.json\n"
        "\n"
        "[Sink]\n"
        "number_items_per_bundle = 2000\n"
        "bloom_false_positive_probability = 0.01\n";
    std::ofstream tempFile(validConfigPath);
    tempFile << validConfig;
    tempFile.close();

    auto config = new Config(validConfigPath);

    ASSERT_EQ(config->generalConfig.QueueSize, 1028);
    ASSERT_EQ(config->sourceConfig.Path, "artists.json");
    ASSERT_EQ(config->sinkConfig.NumberItemsPerBundle, 2000);
    ASSERT_NEAR(config->sinkConfig.BloomFalsePositiveProbability, 0.01, 1e-5);
}

TEST_F(ConfigTest, UndefinedSourcePath)
{
    std::string invalidConfigPath = "/tmp/missing_sourcepath_config.ini";
    std::string invalidConfig =
        "[General]\n"
        "queue_size = 1028\n"
        "\n"
        "[Sink]\n"
        "number_items_per_bundle = 2000\n"
        "bloom_false_positive_probability = 0.01\n";
    std::ofstream tempFile(invalidConfigPath);
    tempFile << invalidConfig;
    tempFile.close();

    try
    {
        EXPECT_EXIT({ auto config = new Config(invalidConfigPath); }, ::testing::ExitedWithCode(EXIT_FAILURE), "");
    }
    catch (const std::exception& e)
    {
        FAIL() << "Expected exit, but caught exception: " << e.what();
    }
}

TEST_F(ConfigTest, DefaultConfig)
{
    std::string defaultConfigPath = "/tmp/default_config.ini";
    std::string defaultConfig =
        "[Source]\n"
        "path = artists.json\n";
    
    std::ofstream tempFile(defaultConfigPath);
    tempFile << defaultConfigPath;
    tempFile.close();
}