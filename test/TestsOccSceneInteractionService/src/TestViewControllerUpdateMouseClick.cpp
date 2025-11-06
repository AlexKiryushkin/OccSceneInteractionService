
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

class TestViewControllerUpdateMouseClick : public TestViewControllerBase
{
  protected:
    TestViewControllerUpdateMouseClick()
        : TestViewControllerBase{}
    {
    }
};

/**
 * Check that there is no crash when we don't have a mouse click handler
 */
TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_noHandler)
{
    m_aisViewController.UpdateMouseClick(
        Graphic3d_Vec2i{100, 200}, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

/**
 * Checks that \a UpdateMouseClick returns true on selection, i.e. clicking left mouse button
 */
TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_updateViewNoHandler)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    auto toUpdateView =
        m_aisViewController.UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_NONE, false);
    EXPECT_TRUE(toUpdateView);

    toUpdateView =
        m_aisViewController.UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    EXPECT_TRUE(toUpdateView);

    toUpdateView =
        m_aisViewController.UpdateMouseClick(point, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_NONE, false);
    EXPECT_FALSE(toUpdateView);
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackActsOn_callbackIsCalled)
{
    const auto point1 = Graphic3d_Vec2i{100, 200};
    const auto point2 = Graphic3d_Vec2i{110, 210};
    const auto point3 = Graphic3d_Vec2i{120, 220};

    m_aisViewController.setMouseClickHandler(m_pMockMouseClickHandler);

    std::vector<std::tuple<Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags, bool>> testData{
        std::make_tuple(point1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false),
        std::make_tuple(point2, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, true),
        std::make_tuple(point3, Aspect_VKeyMouse_MiddleButton, Aspect_VKeyFlags_CTRL | Aspect_VKeyFlags_ALT, true)};

    for(auto [point, button, modifiers, isDoubleClick] : testData)
    {
        m_aisViewController.ResetViewInput();

        EXPECT_CALL(*m_pMockMouseClickHandler, actsOn(button, modifiers, isDoubleClick))
            .WillOnce(::testing::Return(true));
        m_aisViewController.UpdateMouseClick(point, button, modifiers, isDoubleClick);

        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*m_pMockMouseClickHandler, handleMouseClicked(point, button, modifiers, isDoubleClick));
        m_aisViewController.HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackDoesNotActOn_callbackIsNotCalled)
{
    const auto point1 = Graphic3d_Vec2i{100, 200};
    const auto point2 = Graphic3d_Vec2i{110, 210};
    const auto point3 = Graphic3d_Vec2i{120, 220};

    m_aisViewController.setMouseClickHandler(m_pMockMouseClickHandler);

    std::vector<std::tuple<Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags, bool>> testData{
        std::make_tuple(point1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false),
        std::make_tuple(point2, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, true),
        std::make_tuple(point3, Aspect_VKeyMouse_MiddleButton, Aspect_VKeyFlags_CTRL | Aspect_VKeyFlags_ALT, true)};

    for(auto [point, button, modifiers, isDoubleClick] : testData)
    {
        m_aisViewController.ResetViewInput();

        EXPECT_CALL(*m_pMockMouseClickHandler, actsOn(button, modifiers, isDoubleClick))
            .WillOnce(::testing::Return(false));
        m_aisViewController.UpdateMouseClick(point, button, modifiers, isDoubleClick);

        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*m_pMockMouseClickHandler, handleMouseClicked(point, button, modifiers, isDoubleClick)).Times(0);
        m_aisViewController.HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_updateViewWithHandler)
{
    const auto point = Graphic3d_Vec2i{100, 200};
    const auto button = Aspect_VKeyMouse_RightButton;
    const auto modifier = Aspect_VKeyFlags_SHIFT;

    m_aisViewController.setMouseClickHandler(m_pMockMouseClickHandler);

    EXPECT_CALL(*m_pMockMouseClickHandler, actsOn(button, modifier, false)).WillOnce(::testing::Return(true));
    auto toUpdateView = m_aisViewController.UpdateMouseClick(point, button, modifier, false);
    EXPECT_TRUE(toUpdateView);

    EXPECT_CALL(*m_pMockMouseClickHandler, actsOn(button, modifier, false)).WillOnce(::testing::Return(false));
    toUpdateView = m_aisViewController.UpdateMouseClick(point, button, modifier, false);
    EXPECT_FALSE(toUpdateView);
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackIsCalledOnlyOnce)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    m_aisViewController.setMouseClickHandler(m_pMockMouseClickHandler);

    EXPECT_CALL(*m_pMockMouseClickHandler, actsOn(Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false))
        .WillOnce(::testing::Return(true));
    m_aisViewController.UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockMouseClickHandler,
                handleMouseClicked(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false));
    m_aisViewController.HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*m_pMockMouseClickHandler,
                handleMouseClicked(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false)).Times(0ULL);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
