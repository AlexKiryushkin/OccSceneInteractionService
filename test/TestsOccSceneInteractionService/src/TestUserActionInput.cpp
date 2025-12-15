
#include <OccSceneInteractionService/UserActionInput.h>

#include <gtest/gtest.h>

namespace osis::test
{

class TestUserActionInput : public testing::Test
{
};

///
/// TESTS for THROW
///

/**
 * @brief Checks that an exception is thrown if we try to create UserActionInput with zero points
 */
TEST_F(TestUserActionInput, constructorWithZeroPoints_throws)
{
    EXPECT_THROW(UserActionInput(UserActionInput::ApplyOn::Stop, 0), std::exception);
}

/**
 * @brief Checks that an exception is thrown if `continueGatherInput` is called before calling startGatherInput
 */
TEST_F(TestUserActionInput, continueGatherInput_throwsWithoutStart)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    EXPECT_THROW(userActionInput.continueGatherInput(Graphic3d_Vec2i{}), std::exception);
}

/**
 * @brief Checks that an exception is thrown if `continueGatherInput` is called after calling stopGatherInput
 */
TEST_F(TestUserActionInput, continueGatherInput_throwsAfterStop)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    userActionInput.startGatherInput(Graphic3d_Vec2i{});
    userActionInput.stopGatherInput(Graphic3d_Vec2i{});
    EXPECT_THROW(userActionInput.continueGatherInput(Graphic3d_Vec2i{}), std::exception);
}

/**
 * @brief Checks that an exception is thrown if `stopGatherInput` is called before calling startGatherInput
 */
TEST_F(TestUserActionInput, stopGatherInput_throwsWithoutStart)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    EXPECT_THROW(userActionInput.stopGatherInput(Graphic3d_Vec2i{}), std::exception);
}

/**
 * @brief Checks that an exception is thrown if `stopGatherInput` is called twice
 */
TEST_F(TestUserActionInput, stopGatherInput_calledTwiceThrows)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    userActionInput.startGatherInput(Graphic3d_Vec2i{});
    userActionInput.stopGatherInput(Graphic3d_Vec2i{});
    EXPECT_THROW(userActionInput.stopGatherInput(Graphic3d_Vec2i{}), std::exception);
}

/**
 * @brief Checks that an exception is thrown if `startGatherInput` is called twice
 */
TEST_F(TestUserActionInput, startGatherInput_calledTwiceThrows)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    userActionInput.startGatherInput(Graphic3d_Vec2i{});
    EXPECT_THROW(userActionInput.startGatherInput(Graphic3d_Vec2i{}), std::exception);
}

/**
 * @brief Checks resetUiInput resets all the data
 */
TEST_F(TestUserActionInput, resetUiInput)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    userActionInput.startGatherInput(Graphic3d_Vec2i{});
    userActionInput.resetUiInput();

    EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 0ULL);
}

///
/// TESTS for gathering UI input
///

/**
 * @brief Checks that UI input data is set correctly on `startGatherInput` call
 */
TEST_F(TestUserActionInput, startGatherInput)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    userActionInput.startGatherInput(startPoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
}

/**
 * @brief Checks that UI input data is set correctly on `continueGatherInput` call
 */
TEST_F(TestUserActionInput, continueGatherInput)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint);
}

/**
 * @brief Checks that input contains only one point and this point is the point set by `continueGatherInput` if input
 * accepts only one point.
 */
TEST_F(TestUserActionInput, continueGatherInput_onlyOneAllowedPoint)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 1);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), continuePoint);
}

/**
 * @brief Checks that input contains only two points and the last point is overwritten by the last call to
 * `continueGatherInput`
 */
TEST_F(TestUserActionInput, continueGatherInputTwice_onlyTwoAllowedPoints)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint1{15, 20};
    const Graphic3d_Vec2i continuePoint2{20, 25};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint1);
    userActionInput.continueGatherInput(continuePoint2);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint2);
}

/**
 * @brief Checks that if we don't have a limit on input points, all points are added on `continueGatherInput`
 */
TEST_F(TestUserActionInput, continueGatherInputThrice_multipleAllowedPoints)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, {});

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint1{15, 20};
    const Graphic3d_Vec2i continuePoint2{20, 25};
    const Graphic3d_Vec2i continuePoint3{25, 30};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint1);
    userActionInput.continueGatherInput(continuePoint2);
    userActionInput.continueGatherInput(continuePoint3);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 4ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.at(1ULL), continuePoint1);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.at(2ULL), continuePoint2);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint3);
}

/**
 * @brief Checks that `toApply` flag is set if user action is created with `UserActionInput::ApplyOn::Continue`
 */
TEST_F(TestUserActionInput, continueGatherInput_applyOnContinue)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Continue, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_TRUE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint);
}

/**
 * @brief Checks that after calling `stopGatherInput` flag `toApply` is set to false if input is created with enum
 * `UserActionInput::ApplyOn::Continue`
 */
TEST_F(TestUserActionInput, stopGatherInput_applyOnContinue)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Continue, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i stopPoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.stopGatherInput(stopPoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), stopPoint);
}

/**
 * @brief Checks that input contains only one point and this point is the point set by `stopGatherInput` if input
 * accepts only one point.
 */
TEST_F(TestUserActionInput, stopGatherInput_onlyOneAllowedPoint)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Continue, 1);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i stopPoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.stopGatherInput(stopPoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), stopPoint);
}

/**
 * @brief Checks that input contains only two points and the last point is overwritten by call `stopGatherInput`
 */
TEST_F(TestUserActionInput, startContinueStopGatherInput_onlyTwoAllowedPoints)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    const Graphic3d_Vec2i stopPoint{20, 25};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint);
    userActionInput.stopGatherInput(stopPoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_TRUE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), stopPoint);
}

/**
 * @brief Checks that if we don't have a limit on input points, point is added on `stopGatherInput`
 */
TEST_F(TestUserActionInput, continueTwiceThenStopGatherInput_multipleAllowedPoints)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, {});

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint1{15, 20};
    const Graphic3d_Vec2i continuePoint2{20, 25};
    const Graphic3d_Vec2i stopPoint{25, 30};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint1);
    userActionInput.continueGatherInput(continuePoint2);
    userActionInput.stopGatherInput(stopPoint);

    EXPECT_TRUE(userActionInput.getUiUserInputData().toStart);
    EXPECT_TRUE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 4ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.at(1ULL), continuePoint1);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.at(2ULL), continuePoint2);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), stopPoint);
}

///
/// TESTS for syncing UI and Render input
///

/**
 * @brief Checks that `sync` does nothing if it is called before calling `startGatherInput` method
 */
TEST_F(TestUserActionInput, sync_beforeStartDoesNothing)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    // Dirty hack but should not cause UB as the object itself is not constant. We modify UI data to check that it won't
    // be copied to render data.

    // NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
    const_cast<UserActionInputData &>(userActionInput.getUiUserInputData()).inputPoints.emplace_back();
    const_cast<UserActionInputData &>(userActionInput.getUiUserInputData()).toStart = true;
    const_cast<UserActionInputData &>(userActionInput.getUiUserInputData()).toApply = true;
    // NOLINTEND(cppcoreguidelines-pro-type-const-cast)

    userActionInput.sync();

    EXPECT_FALSE(userActionInput.getRenderUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getRenderUserInputData().toApply);
    EXPECT_TRUE(userActionInput.getRenderUserInputData().inputPoints.empty());
}

/**
 * @brief Checks that syncing between UI and Render inputs are correct after calling `startGatherInput` method. All data
 * is copied and `toStart` flag of UI input is set to false.
 */
TEST_F(TestUserActionInput, startGatherInput_sync)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    userActionInput.startGatherInput(startPoint);
    userActionInput.sync();

    EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);

    EXPECT_TRUE(userActionInput.getRenderUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getRenderUserInputData().toApply);
    ASSERT_EQ(userActionInput.getRenderUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.front(), startPoint);
}

/**
 * @brief Checks that syncing between UI and Render inputs are correct after calling `startGatherInput` and
 * `continueGatherInput` in one frame. All data is copied and `toStart` flag of UI input is set to false.
 */
TEST_F(TestUserActionInput, startAndContinueGatherInput_sync)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.continueGatherInput(continuePoint);

    userActionInput.sync();

    EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint);

    EXPECT_TRUE(userActionInput.getRenderUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getRenderUserInputData().toApply);
    ASSERT_EQ(userActionInput.getRenderUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.back(), continuePoint);
}

/**
 * @brief Checks that syncing between UI and Render inputs are correct after calling `startGatherInput` and
 * `continueGatherInput` in different frames. All data is copied and `toStart` flag of UI input is set to false.
 */
TEST_F(TestUserActionInput, continueGatherInput_sync)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Stop, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.sync();
    userActionInput.resetRenderInput();

    userActionInput.continueGatherInput(continuePoint);
    userActionInput.sync();

    EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint);

    EXPECT_FALSE(userActionInput.getRenderUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getRenderUserInputData().toApply);
    ASSERT_EQ(userActionInput.getRenderUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.back(), continuePoint);
}

/**
 * @brief Checks that syncing between UI and Render inputs are correct after calling `startGatherInput` and
 * `continueGatherInput` in different frames and action is applied on continue. All data is copied and `toStart` flag
 * of UI input is set to false.
 */
TEST_F(TestUserActionInput, continueGatherInput_applyOnContinue_sync)
{
    auto userActionInput = UserActionInput(UserActionInput::ApplyOn::Continue, 2);

    const Graphic3d_Vec2i startPoint{10, 15};
    const Graphic3d_Vec2i continuePoint{15, 20};
    userActionInput.startGatherInput(startPoint);
    userActionInput.sync();
    userActionInput.resetRenderInput();

    userActionInput.continueGatherInput(continuePoint);
    userActionInput.sync();

    EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
    EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
    ASSERT_EQ(userActionInput.getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getUiUserInputData().inputPoints.back(), continuePoint);

    EXPECT_FALSE(userActionInput.getRenderUserInputData().toStart);
    EXPECT_TRUE(userActionInput.getRenderUserInputData().toApply);
    ASSERT_EQ(userActionInput.getRenderUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.back(), continuePoint);
}

/**
 * @brief Checks that syncing between UI and Render inputs are correct after calling `startGatherInput` and
 * `continueGatherInput` in one frame and `stopGatherInput` in another frame. All data is copied and `toStart` flag
 * of UI input is set to false.
 */
TEST_F(TestUserActionInput, stopGatherInput_sync)
{
    struct TestData
    {
        UserActionInput::ApplyOn applyOn;
        bool toApply;
    };
    std::vector<TestData> testData{{UserActionInput::ApplyOn::Stop, true}, {UserActionInput::ApplyOn::Continue, false}};
    for(auto &&[applyOn, toApply] : testData)
    {
        auto userActionInput = UserActionInput(applyOn, 2);

        const Graphic3d_Vec2i startPoint{10, 15};
        const Graphic3d_Vec2i continuePoint{15, 20};
        const Graphic3d_Vec2i stopPoint{20, 25};
        userActionInput.startGatherInput(startPoint);
        userActionInput.continueGatherInput(continuePoint);
        userActionInput.sync();
        userActionInput.resetRenderInput();

        userActionInput.stopGatherInput(stopPoint);
        userActionInput.sync();

        EXPECT_FALSE(userActionInput.getUiUserInputData().toStart);
        EXPECT_FALSE(userActionInput.getUiUserInputData().toApply);
        EXPECT_TRUE(userActionInput.getUiUserInputData().inputPoints.empty());

        EXPECT_FALSE(userActionInput.getRenderUserInputData().toStart);
        EXPECT_EQ(userActionInput.getRenderUserInputData().toApply, toApply);
        ASSERT_EQ(userActionInput.getRenderUserInputData().inputPoints.size(), 2ULL);
        EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.front(), startPoint);
        EXPECT_EQ(userActionInput.getRenderUserInputData().inputPoints.back(), stopPoint);
    }
}

} // namespace osis::test
