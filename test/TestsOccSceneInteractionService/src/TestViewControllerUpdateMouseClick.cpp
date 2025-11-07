
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

class TestViewControllerUpdateMouseClick : public TestViewControllerBase
{
  protected:
    TestViewControllerUpdateMouseClick() = default;
};

/**
 * Check that there is no crash when we don't have a mouse click handler
 */
TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_noHandler)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    getViewController().UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * Checks that \a UpdateMouseClick returns true on selection, i.e. clicking left mouse button
 */
TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_updateViewNoHandler)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    auto toUpdateView =
        getViewController().UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_NONE, false);
    EXPECT_TRUE(toUpdateView);

    toUpdateView =
        getViewController().UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    EXPECT_TRUE(toUpdateView);

    toUpdateView =
        getViewController().UpdateMouseClick(point, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_NONE, false);
    EXPECT_FALSE(toUpdateView);
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackActsOn_callbackIsCalled)
{
    const auto point1 = Graphic3d_Vec2i{100, 200};
    const auto point2 = Graphic3d_Vec2i{110, 210};
    const auto point3 = Graphic3d_Vec2i{120, 220};

    getViewController().setMouseClickHandler(getMockMouseClickHandler());

    std::vector<std::tuple<Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags, bool>> testData{
        std::make_tuple(point1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false),
        std::make_tuple(point2, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, true),
        std::make_tuple(point3, Aspect_VKeyMouse_MiddleButton, Aspect_VKeyFlags_CTRL | Aspect_VKeyFlags_ALT, true)};

    for(auto [point, button, modifiers, isDoubleClick] : testData)
    {
        getViewController().ResetViewInput();

        EXPECT_CALL(*getMockMouseClickHandler(), actsOn(button, modifiers, isDoubleClick))
            .WillOnce(::testing::Return(true));
        getViewController().UpdateMouseClick(point, button, modifiers, isDoubleClick);

        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockMouseClickHandler(), handleMouseClicked(point, button, modifiers, isDoubleClick));
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackDoesNotActOn_callbackIsNotCalled)
{
    const auto point1 = Graphic3d_Vec2i{100, 200};
    const auto point2 = Graphic3d_Vec2i{110, 210};
    const auto point3 = Graphic3d_Vec2i{120, 220};

    getViewController().setMouseClickHandler(getMockMouseClickHandler());

    std::vector<std::tuple<Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags, bool>> testData{
        std::make_tuple(point1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false),
        std::make_tuple(point2, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, true),
        std::make_tuple(point3, Aspect_VKeyMouse_MiddleButton, Aspect_VKeyFlags_CTRL | Aspect_VKeyFlags_ALT, true)};

    for(auto [point, button, modifiers, isDoubleClick] : testData)
    {
        getViewController().ResetViewInput();

        EXPECT_CALL(*getMockMouseClickHandler(), actsOn(button, modifiers, isDoubleClick))
            .WillOnce(::testing::Return(false));
        getViewController().UpdateMouseClick(point, button, modifiers, isDoubleClick);

        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockMouseClickHandler(), handleMouseClicked(point, button, modifiers, isDoubleClick)).Times(0);
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_updateViewWithHandler)
{
    const auto point = Graphic3d_Vec2i{100, 200};
    const auto button = Aspect_VKeyMouse_RightButton;
    const auto modifier = Aspect_VKeyFlags_SHIFT;

    getViewController().setMouseClickHandler(getMockMouseClickHandler());

    EXPECT_CALL(*getMockMouseClickHandler(), actsOn(button, modifier, false)).WillOnce(::testing::Return(true));
    auto toUpdateView = getViewController().UpdateMouseClick(point, button, modifier, false);
    EXPECT_TRUE(toUpdateView);

    EXPECT_CALL(*getMockMouseClickHandler(), actsOn(button, modifier, false)).WillOnce(::testing::Return(false));
    toUpdateView = getViewController().UpdateMouseClick(point, button, modifier, false);
    EXPECT_FALSE(toUpdateView);
}

TEST_F(TestViewControllerUpdateMouseClick, updateMouseClick_callbackIsCalledOnlyOnce)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    getViewController().setMouseClickHandler(getMockMouseClickHandler());

    EXPECT_CALL(*getMockMouseClickHandler(), actsOn(Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false))
        .WillOnce(::testing::Return(true));
    getViewController().UpdateMouseClick(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockMouseClickHandler(),
                handleMouseClicked(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*getMockMouseClickHandler(),
                handleMouseClicked(point, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false))
        .Times(0ULL);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
