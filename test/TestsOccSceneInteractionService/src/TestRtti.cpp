
#include "TestsOccSceneInteractionService/MockCameraListener.h"
#include "TestsOccSceneInteractionService/MockMouseClickHandler.h"

#include <OccSceneInteractionService/ICameraListener.h>

#include <gtest/gtest.h>

namespace osis::test
{

class TestRtti : public ::testing::Test
{
};

TEST_F(TestRtti, ICameraListener)
{
    MockCameraListener mockCameraListener;

    // We do not add RTTI for MockCameraListener, so this should be equal
    EXPECT_EQ(ICameraListener::get_type_descriptor(), mockCameraListener.DynamicType());
}

TEST_F(TestRtti, IMouseClickHandler)
{
    MockMouseClickHandler mockMouseClickHandler;

    // We do not add RTTI for MockMouseClickHandler, so this should be equal
    EXPECT_EQ(IMouseClickHandler::get_type_descriptor(), mockMouseClickHandler.DynamicType());
}

} // namespace osis::test
