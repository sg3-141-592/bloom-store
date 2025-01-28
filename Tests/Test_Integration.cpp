#include <fstream>
#include <vector>

#include <gtest/gtest.h>

#include "../Config.h"
#include "../DataPipeline.h"
#include "../DataSource/FileDataSource.h"
#include "../DataSink/FolderDataSink.h"
#include "../RecordProcessor/JsonDeserializer.h"
#include "../Utilities/TSQueue.h"
#include "../Search/SearchRecords.h"

class IntegrationTest : public testing::Test
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

TEST_F(IntegrationTest, SmallFile)
{
    // Push a 5 entry file and check if all of the values can be retrieved
    auto testConfig = std::make_shared<Config>("./test_config.ini");
    auto pipeline = std::make_shared<DataPipeline<FileDataSource, FolderDataSink>>(
        testConfig
    );
    
    pipeline->process<JsonDeserializer>();
    
    std::vector<std::string> BandNames = {"Rados", "Power Tool"};
    for (auto &bandName : BandNames)
    {
        auto results = SearchRecords(bandName, "");
        ASSERT_EQ(results.size(), 1);
    }
    
}