
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include "TestsOccSceneInteractionService/MockCustomMouseAction.h"
#include "TestsOccSceneInteractionService/MockInputHandler.h"

#include <vector>

namespace osis::test
{
using ::testing::_;

static constexpr auto cLeftButton = Aspect_VKeyMouse_LeftButton;
static constexpr auto cRightButton = Aspect_VKeyMouse_RightButton;

static constexpr auto cNoneKey = Aspect_VKeyFlags_NONE;
static constexpr auto cCtrlKey = Aspect_VKeyFlags_CTRL;
static constexpr auto cShiftKey = Aspect_VKeyFlags_SHIFT;
static constexpr auto cAltKey = Aspect_VKeyFlags_ALT;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
class TestViewControllerCustomAction : public TestViewControllerBase
{
  protected:
    UserActionInput m_userActionInput{UserActionInput::ApplyOn::Continue, 2};
    Handle(::testing::StrictMock<MockInputHandler>) m_pMockInputHandler = new ::testing::StrictMock<MockInputHandler>();

    unsigned int m_defaultMouseCombination = static_cast<unsigned int>(cRightButton) |
                                             static_cast<unsigned int>(cCtrlKey) | static_cast<unsigned int>(cShiftKey);

    Handle(testing::StrictMock<MockCustomMouseAction>) m_pDefaultCustomAction =
        new testing::StrictMock<MockCustomMouseAction>(m_userActionInput, std::vector{m_defaultMouseCombination},
                                                       m_pMockInputHandler);
};

TEST_F(TestViewControllerCustomAction, input_wrongMouseCombination_noStart)
{
    EXPECT_CALL(*m_pDefaultCustomAction, setUp());
    getViewController().setCustomMouseActions({m_pDefaultCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    struct TestData
    {
        Aspect_VKeyMouse mouseButton;
        Aspect_VKeyFlags modifiers;
    };

    std::vector<TestData> testData{{cRightButton, cNoneKey},
                                   {cRightButton, cShiftKey},
                                   {cLeftButton, cCtrlKey},
                                   {cRightButton | cLeftButton, cCtrlKey},
                                   {cRightButton, cCtrlKey | cAltKey}};

    for(auto [mouseButton, modifiers] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons({}, mouseButton, modifiers, false);
        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().inputPoints.empty());
        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toStart);
        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toApply);
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationStart)
{
    struct TestData
    {
        Aspect_VKeyMouse prevMouseButton;
        Aspect_VKeyFlags prevModifiers;
        Aspect_VKeyMouse currMouseButton;
        Aspect_VKeyFlags currModifiers;
        Graphic3d_Vec2i startPoint;
    };

    std::vector<TestData> testData{
        {cRightButton, cNoneKey, cRightButton, cCtrlKey | cShiftKey, Graphic3d_Vec2i{10, 15}},
        {cRightButton | cLeftButton, cCtrlKey, cRightButton, cCtrlKey | cShiftKey, Graphic3d_Vec2i{15, 20}},
        {cRightButton, cCtrlKey, cRightButton, cCtrlKey | cShiftKey, Graphic3d_Vec2i{25, 20}}};

    EXPECT_CALL(*m_pDefaultCustomAction, setUp());
    getViewController().setCustomMouseActions({m_pDefaultCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    for(auto [prevMouseButton, prevModifiers, currMouseButton, currModifiers, point] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons(point, prevMouseButton, prevModifiers, false);
        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().inputPoints.empty());
        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toStart);
        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toApply);

        getViewController().UpdateMouseButtons(point, currMouseButton, currModifiers, false);
        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().toStart);
        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toApply);
        ASSERT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.size(), 1ULL);
        EXPECT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.front(), point);

        m_pDefaultCustomAction->clearUiInput();
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationContinue_sameFrame)
{
    EXPECT_CALL(*m_pDefaultCustomAction, setUp());
    getViewController().setCustomMouseActions({m_pDefaultCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    std::vector<std::pair<Graphic3d_Vec2i, Graphic3d_Vec2i>> testData{
        {Graphic3d_Vec2i{10, 15}, Graphic3d_Vec2i{15, 20}},
        {Graphic3d_Vec2i{60, 18}, Graphic3d_Vec2i{73, 9}},
        {Graphic3d_Vec2i{156, 187}, Graphic3d_Vec2i{148, 190}}};

    for(auto [startPoint, continuePoint] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);

        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().toStart);
        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().toApply);
        ASSERT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.size(), 2ULL);
        EXPECT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.front(), startPoint);
        EXPECT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.back(), continuePoint);

        m_pDefaultCustomAction->clearUiInput();
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationContinue_differentFrames)
{
    EXPECT_CALL(*m_pDefaultCustomAction, setUp());
    getViewController().setCustomMouseActions({m_pDefaultCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    std::vector<std::pair<Graphic3d_Vec2i, Graphic3d_Vec2i>> testData{
        {Graphic3d_Vec2i{10, 15}, Graphic3d_Vec2i{15, 20}},
        {Graphic3d_Vec2i{60, 18}, Graphic3d_Vec2i{73, 9}},
        {Graphic3d_Vec2i{156, 187}, Graphic3d_Vec2i{148, 190}}};

    for(auto [startPoint, continuePoint] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_TRUE(m_pDefaultCustomAction->getRenderUserInputData().toStart);
        EXPECT_FALSE(m_pDefaultCustomAction->getRenderUserInputData().toApply);
        ASSERT_EQ(m_pDefaultCustomAction->getRenderUserInputData().inputPoints.size(), 1ULL);
        EXPECT_EQ(m_pDefaultCustomAction->getRenderUserInputData().inputPoints.front(), startPoint);

        getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);

        EXPECT_FALSE(m_pDefaultCustomAction->getUiUserInputData().toStart);
        EXPECT_TRUE(m_pDefaultCustomAction->getUiUserInputData().toApply);
        ASSERT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.size(), 2ULL);
        EXPECT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.front(), startPoint);
        EXPECT_EQ(m_pDefaultCustomAction->getUiUserInputData().inputPoints.back(), continuePoint);

        m_pDefaultCustomAction->clearUiInput();
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationStop_sameFrame)
{
    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    Handle(CustomMouseAction) pCustomAction =
        new CustomMouseAction(userActionInput, std::vector{m_defaultMouseCombination}, m_pMockInputHandler);

    getViewController().setCustomMouseActions({pCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    std::vector<std::tuple<Graphic3d_Vec2i, Graphic3d_Vec2i, Graphic3d_Vec2i>> testData{
        {Graphic3d_Vec2i{10, 15}, Graphic3d_Vec2i{15, 20}, Graphic3d_Vec2i{20, 23}},
        {Graphic3d_Vec2i{60, 18}, Graphic3d_Vec2i{73, 9}, Graphic3d_Vec2i{71, 10}},
        {Graphic3d_Vec2i{156, 187}, Graphic3d_Vec2i{148, 190}, Graphic3d_Vec2i{146, 188}}};

    for(auto [startPoint, continuePoint, stopPoint] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().UpdateMouseButtons(stopPoint, cRightButton, cNoneKey, false);

        EXPECT_TRUE(pCustomAction->getUiUserInputData().toStart);
        EXPECT_TRUE(pCustomAction->getUiUserInputData().toApply);
        ASSERT_EQ(pCustomAction->getUiUserInputData().inputPoints.size(), 3ULL);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.front(), startPoint);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.at(1ULL), continuePoint);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.back(), stopPoint);

        pCustomAction->clearUiInput();
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationStop_differentFrames)
{
    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    Handle(CustomMouseAction) pCustomAction =
        new CustomMouseAction(userActionInput, std::vector{m_defaultMouseCombination}, m_pMockInputHandler);

    getViewController().setCustomMouseActions({pCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    std::vector<std::tuple<Graphic3d_Vec2i, Graphic3d_Vec2i, Graphic3d_Vec2i>> testData{
        {Graphic3d_Vec2i{10, 15}, Graphic3d_Vec2i{15, 20}, Graphic3d_Vec2i{20, 23}},
        {Graphic3d_Vec2i{60, 18}, Graphic3d_Vec2i{73, 9}, Graphic3d_Vec2i{71, 10}},
        {Graphic3d_Vec2i{156, 187}, Graphic3d_Vec2i{148, 190}, Graphic3d_Vec2i{146, 188}}};

    for(auto [startPoint, continuePoint, stopPoint] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);
        getViewController().FlushViewEvents(getContext(), getView(), false);
        //getViewController().HandleViewEvents(getContext(), getView());

        getViewController().UpdateMouseButtons(stopPoint, cRightButton, cNoneKey, false);
        EXPECT_FALSE(pCustomAction->getUiUserInputData().toStart);
        EXPECT_TRUE(pCustomAction->getUiUserInputData().toApply);
        ASSERT_EQ(pCustomAction->getUiUserInputData().inputPoints.size(), 3ULL);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.front(), startPoint);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.at(1ULL), continuePoint);
        EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.back(), stopPoint);

        pCustomAction->clearUiInput();
    }
}

TEST_F(TestViewControllerCustomAction, input_mouseMultipleCombinationsChangeModifiers_noStopAction)
{
    const auto mouseCombination1 = static_cast<unsigned int>(cRightButton) | static_cast<unsigned int>(cCtrlKey) |
                                   static_cast<unsigned int>(cShiftKey);
    const auto mouseCombination2 = static_cast<unsigned int>(cRightButton) | static_cast<unsigned int>(cCtrlKey) |
                                   static_cast<unsigned int>(cAltKey);

    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    auto pCustomAction = Handle(CustomMouseAction){
        new CustomMouseAction(userActionInput, std::vector{mouseCombination1, mouseCombination2}, m_pMockInputHandler)};

    getViewController().setCustomMouseActions({pCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    const auto startPoint = Graphic3d_Vec2i{};
    const auto continuePoint = Graphic3d_Vec2i{10, 10};
    const auto endPoint = Graphic3d_Vec2i{20, 30};

    getViewController().UpdateMouseButtons({}, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    getViewController().UpdateMouseButtons(endPoint, cRightButton, cCtrlKey | cAltKey, false);

    EXPECT_FALSE(pCustomAction->getUiUserInputData().toStart);
    EXPECT_FALSE(pCustomAction->getUiUserInputData().toApply);
    ASSERT_EQ(pCustomAction->getUiUserInputData().inputPoints.size(), 2ULL);
    EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(pCustomAction->getUiUserInputData().inputPoints.back(), continuePoint);
}

TEST_F(TestViewControllerCustomAction, input_mouseCombinationChangeAction)
{
    const auto mouseCombination1 = static_cast<unsigned int>(cRightButton) | static_cast<unsigned int>(cCtrlKey) |
                                   static_cast<unsigned int>(cShiftKey);
    const auto mouseCombination2 = static_cast<unsigned int>(cRightButton) | static_cast<unsigned int>(cCtrlKey) |
                                   static_cast<unsigned int>(cAltKey);

    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    auto pCustomAction1 = Handle(CustomMouseAction){
        new CustomMouseAction(userActionInput, std::vector{mouseCombination1}, m_pMockInputHandler)};

    auto pMockAnotherInputHandler = Handle(MockInputHandler){new MockInputHandler()};
    auto pCustomAction2 = Handle(CustomMouseAction){
        new CustomMouseAction(m_userActionInput, std::vector{mouseCombination2}, pMockAnotherInputHandler)};

    getViewController().setCustomMouseActions({pCustomAction1, pCustomAction2});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    const auto startPoint = Graphic3d_Vec2i{14, 12};
    const auto continuePoint = Graphic3d_Vec2i{13, 11};
    const auto stopPoint = Graphic3d_Vec2i{13, 11};

    getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    getViewController().UpdateMouseButtons(stopPoint, cRightButton, cCtrlKey | cAltKey, false);

    EXPECT_FALSE(pCustomAction1->getUiUserInputData().toStart);
    EXPECT_TRUE(pCustomAction1->getUiUserInputData().toApply);
    ASSERT_EQ(pCustomAction1->getUiUserInputData().inputPoints.size(), 3ULL);
    EXPECT_EQ(pCustomAction1->getUiUserInputData().inputPoints.front(), startPoint);
    EXPECT_EQ(pCustomAction1->getUiUserInputData().inputPoints.at(1ULL), continuePoint);
    EXPECT_EQ(pCustomAction1->getUiUserInputData().inputPoints.back(), stopPoint);

    EXPECT_TRUE(pCustomAction2->getUiUserInputData().toStart);
    EXPECT_FALSE(pCustomAction2->getUiUserInputData().toApply);
    ASSERT_EQ(pCustomAction2->getUiUserInputData().inputPoints.size(), 1ULL);
    EXPECT_EQ(pCustomAction2->getUiUserInputData().inputPoints.front(), stopPoint);
}

TEST_F(TestViewControllerCustomAction, handle_mouseCombinationStartContinueStop)
{
    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    Handle(CustomMouseAction) pCustomAction =
        new CustomMouseAction(userActionInput, std::vector{m_defaultMouseCombination}, m_pMockInputHandler);

    getViewController().setCustomMouseActions({pCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    const auto startPoint = Graphic3d_Vec2i{14, 12};
    const auto continuePoint = Graphic3d_Vec2i{13, 11};
    const auto stopPoint = Graphic3d_Vec2i{13, 11};

    getViewController().UpdateMouseButtons(startPoint, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    UserActionInputData userActionInputData;

    EXPECT_CALL(*m_pMockInputHandler, handle(_)).WillOnce(::testing::SaveArg<0ULL>(&userActionInputData));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(userActionInputData.toStart);
    EXPECT_FALSE(userActionInputData.toApply);
    ASSERT_EQ(userActionInputData.inputPoints.size(), 1ULL);
    EXPECT_EQ(userActionInputData.inputPoints.front(), startPoint);

    getViewController().UpdateMousePosition(continuePoint, cRightButton, cCtrlKey | cShiftKey, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockInputHandler, handle(_)).WillOnce(::testing::SaveArg<0ULL>(&userActionInputData));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_FALSE(userActionInputData.toStart);
    EXPECT_FALSE(userActionInputData.toApply);
    ASSERT_EQ(userActionInputData.inputPoints.size(), 2ULL);
    EXPECT_EQ(userActionInputData.inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInputData.inputPoints.back(), continuePoint);

    getViewController().UpdateMouseButtons(stopPoint, cRightButton, cNoneKey, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockInputHandler, handle(_)).WillOnce(::testing::SaveArg<0ULL>(&userActionInputData));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_FALSE(userActionInputData.toStart);
    EXPECT_TRUE(userActionInputData.toApply);
    ASSERT_EQ(userActionInputData.inputPoints.size(), 3ULL);
    EXPECT_EQ(userActionInputData.inputPoints.front(), startPoint);
    EXPECT_EQ(userActionInputData.inputPoints.at(1ULL), continuePoint);
    EXPECT_EQ(userActionInputData.inputPoints.back(), stopPoint);
}

TEST_F(TestViewControllerCustomAction, setNewAction_tearDownIsCalled)
{
    UserActionInput userActionInput{UserActionInput::ApplyOn::Stop, 3};
    Handle(testing::StrictMock<MockCustomMouseAction>) pCustomAction = new testing::StrictMock<MockCustomMouseAction>(
        userActionInput, std::vector{m_defaultMouseCombination}, m_pMockInputHandler);

    EXPECT_CALL(*m_pDefaultCustomAction, setUp());
    getViewController().setCustomMouseActions({m_pDefaultCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);

    ::testing::InSequence inSequence;
    EXPECT_CALL(*m_pDefaultCustomAction, tearDown());
    EXPECT_CALL(*pCustomAction, setUp());
    getViewController().setCustomMouseActions({pCustomAction});
    getViewController().FlushViewEvents(getContext(), getView(), false);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

} // namespace osis::test
