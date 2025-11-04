
#include <OccSceneInteractionService/UiRenderSyncObject.h>

#include <gtest/gtest.h>

namespace osis::test
{

class TestUiRenderSyncObject : public ::testing::Test
{
  protected:
    TestUiRenderSyncObject()
        : testing::Test{}
    {
    }
};

TEST_F(TestUiRenderSyncObject, defaultInitialized)
{
    UiRenderSyncObject<double> syncObject;
    EXPECT_EQ(syncObject.getUiData(), 0.0);
    EXPECT_EQ(syncObject.getRenderData(), 0.0);
}

TEST_F(TestUiRenderSyncObject, setUiData)
{
    UiRenderSyncObject<double> syncObject;

    for(const auto value : {1.0, 2.5, 6.7})
    {
        syncObject.setUiData(value);
        EXPECT_EQ(syncObject.getUiData(), value);
    }
}

TEST_F(TestUiRenderSyncObject, sync)
{
    UiRenderSyncObject<double> syncObject;

    syncObject.setUiData(1.0);

    auto wasChanged = syncObject.sync();
    EXPECT_EQ(syncObject.getUiData(), 1.0);
    EXPECT_EQ(syncObject.getRenderData(), 1.0);
    EXPECT_TRUE(wasChanged);

    wasChanged = syncObject.sync();
    EXPECT_FALSE(wasChanged);

    syncObject.setUiData(1.0);
    wasChanged = syncObject.sync();
    EXPECT_FALSE(wasChanged);
}

TEST_F(TestUiRenderSyncObject, resetRenderData)
{
    UiRenderSyncObject<double> syncObject;

    for(const auto value : {1.0, 2.5, 6.7})
    {
        syncObject.setUiData(value);
        syncObject.sync();
        EXPECT_EQ(syncObject.getRenderData(), value);

        syncObject.resetRenderData();
        EXPECT_EQ(syncObject.getRenderData(), 0.0);
    }
}

} // namespace osis::test
