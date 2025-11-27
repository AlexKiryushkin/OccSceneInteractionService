
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

using ::testing::_;

class TestViewControllerMouseHoverListener : public TestViewControllerBase
{
};

TEST_F(TestViewControllerMouseHoverListener, updateMousePosition_noHandler)
{
    getViewController().UpdateMousePosition(Graphic3d_Vec2i{100, 200}, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_NONE,
                                            false);

    // just check that we don't crash if we don't have a mouse click handler
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

TEST_F(TestViewControllerMouseHoverListener, updateMousePosition_callbackIsCalled)
{
    const auto point1 = Graphic3d_Vec2i{100, 200};
    const auto point2 = Graphic3d_Vec2i{110, 210};
    const auto point3 = Graphic3d_Vec2i{120, 220};

    getViewController().setMouseHoverListener(getMockMouseHoverListener());

    std::vector<std::tuple<Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags>> testData{
        std::make_tuple(point1, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_SHIFT),
        std::make_tuple(point2, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_CTRL),
        std::make_tuple(point3, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_CTRL | Aspect_VKeyFlags_ALT)};

    for(auto [point, button, modifiers] : testData)
    {
        getViewController().ResetViewInput();

        getViewController().UpdateMousePosition(point, button, modifiers, false);

        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockMouseHoverListener(), onHover(point));
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerMouseHoverListener, updateMousePositionMoveToDisallowed_returnsTrueForListener)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    getViewController().setMouseHoverListener(getMockMouseHoverListener());

    getViewController().SetAllowHighlight(false);

    const auto toUpdateView = getViewController().UpdateMousePosition(point, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_SHIFT, false);
    EXPECT_TRUE(toUpdateView);
}

TEST_F(TestViewControllerMouseHoverListener, updateMousePosition_callbackIsCalledOnlyOnce)
{
    const auto point = Graphic3d_Vec2i{100, 200};

    getViewController().setMouseHoverListener(getMockMouseHoverListener());

    getViewController().UpdateMousePosition(point, Aspect_VKeyMouse_NONE, Aspect_VKeyFlags_SHIFT, false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockMouseHoverListener(), onHover(point));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*getMockMouseHoverListener(), onHover(_)).Times(0ULL);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
