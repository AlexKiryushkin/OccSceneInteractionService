
#include "TestsOccSceneInteractionService/MockCameraListener.h"
#include "TestsOccSceneInteractionService/MockCustomMouseAction.h"
#include "TestsOccSceneInteractionService/MockInputHandler.h"
#include "TestsOccSceneInteractionService/MockKeyHandler.h"
#include "TestsOccSceneInteractionService/MockMouseClickHandler.h"
#include "TestsOccSceneInteractionService/MockMouseHoverListener.h"
#include "TestsOccSceneInteractionService/MockOwnerHoverListener.h"

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

TEST_F(TestRtti, IOwnerHoverListener)
{
    MockOwnerHoverListener mockOwnerHoverListener;

    // We do not add RTTI for MockOwnerHoverListener, so this should be equal
    EXPECT_EQ(IOwnerHoverListener::get_type_descriptor(), mockOwnerHoverListener.DynamicType());
}

TEST_F(TestRtti, IMouseHoverListener)
{
    MockMouseHoverListener mockMouseHoverListener;

    // We do not add RTTI for MockMouseHoverListener, so this should be equal
    EXPECT_EQ(IMouseHoverListener::get_type_descriptor(), mockMouseHoverListener.DynamicType());
}

TEST_F(TestRtti, IKeyHandler)
{
    MockKeyHandler mockKeyHandler;

    // We do not add RTTI for MockKeyHandler, so this should be equal
    EXPECT_EQ(IKeyHandler::get_type_descriptor(), mockKeyHandler.DynamicType());
}

TEST_F(TestRtti, IInputHandler)
{
    MockInputHandler mockInputHandler;

    // We do not add RTTI for MockInputHandler, so this should be equal
    EXPECT_EQ(IInputHandler::get_type_descriptor(), mockInputHandler.DynamicType());
}

TEST_F(TestRtti, CustomMouseAction)
{
    UserActionInput userActionInput{UserActionInput::ApplyOn::Continue, 2};
    MockCustomMouseAction mockCustomMouseAction{
        userActionInput, std::vector<unsigned>{Aspect_VKeyMouse_LeftButton}, {}};

    // We do not add RTTI for CustomMouseAction, so this should be equal
    EXPECT_EQ(CustomMouseAction::get_type_descriptor(), mockCustomMouseAction.DynamicType());
}

} // namespace osis::test
