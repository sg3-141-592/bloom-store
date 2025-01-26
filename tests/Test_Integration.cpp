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
    Config testConfig;
    auto pipeline = new DataPipeline(
        std::make_shared<FileDataSource>("./test_files/small.json"),
        std::make_shared<FolderDataSink>(get_path_func),
        std::make_shared<JsonDeserializer>(),
        testConfig
    );
    
    pipeline->process();

    std::vector<std::string> BandNames = {"Green Day", "The Beatles", "The Rolling Stones", "The Who", "The Doors"};
    for (auto &bandName : BandNames)
    {
        SearchRecords(bandName);
    }
    
}