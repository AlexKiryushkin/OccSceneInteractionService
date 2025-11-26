
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

namespace osis::test
{

using ::testing::_;

/**
 * @brief Tests owner hover listener scenarios.
 */
class TestOwnerHoverListener : public TestViewControllerBase
{
  protected:
    static Handle(AIS_Shape) makeAisBox(const gp_Pnt &startPoint = gp_Pnt{})
    {
        auto shape = BRepPrimAPI_MakeBox{startPoint, 1.0, 1.0, 1.0};
        return Handle(AIS_Shape){new AIS_Shape(shape)};
    }
};

/**
 * @brief Checks that the event handling is correct when user hovers over an AIS object.
 */
TEST_F(TestOwnerHoverListener, event_moveTo)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point = Graphic3d_Vec2i{windowSizeX / 2, windowSizeY / 2};

    const auto pAisShape = makeAisBox();
    getContext()->Display(pAisShape, true);
    getView()->FitAll();

    getViewController().UpdateMousePosition(point, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(getContext()->HasDetected());
    EXPECT_EQ(getContext()->DetectedInteractive(), pAisShape);
}

/**
 * @brief Checks that the callback is called when object is hovered.
 */
TEST_F(TestOwnerHoverListener, event_moveTo_startHoveringCallbackIsCalled)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point = Graphic3d_Vec2i{windowSizeX / 2, windowSizeY / 2};

    const auto pAisShape = makeAisBox();
    getContext()->Display(pAisShape, true);
    getView()->FitAll();

    getViewController().setOwnerHoverListener(getMockOwnerHoverListener());

    getViewController().UpdateMousePosition(point, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(pAisShape->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(getContext()->HasDetected());
    EXPECT_EQ(getContext()->DetectedInteractive(), pAisShape);
}

/**
 * @brief Checks that the callback is called when object is no longer hovered.
 */
TEST_F(TestOwnerHoverListener, event_moveTo_stopHoveringCallbackIsCalled)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point1 = Graphic3d_Vec2i{windowSizeX / 2, windowSizeY / 2};
    const auto point2 = Graphic3d_Vec2i{0, 0};

    const auto pAisShape = makeAisBox();
    getContext()->Display(pAisShape, true);
    getView()->FitAll();

    getViewController().setOwnerHoverListener(getMockOwnerHoverListener());

    getViewController().UpdateMousePosition(point1, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(pAisShape->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(point2, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStopHovered(pAisShape->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_FALSE(getContext()->HasDetected());
}

/**
 * @brief Checks that the callback is not called when the same object is hovered in the next frame.
 */
TEST_F(TestOwnerHoverListener, event_moveToSameObjectSecondFrame_noCallback)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point = Graphic3d_Vec2i{windowSizeX / 2, windowSizeY / 2};

    const auto pAisShape = makeAisBox();
    getContext()->Display(pAisShape, true);
    getView()->FitAll();

    getViewController().setOwnerHoverListener(getMockOwnerHoverListener());

    getViewController().UpdateMousePosition(point, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(pAisShape->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(point + Graphic3d_Vec2i{1, 1}, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(_)).Times(0);
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(getContext()->HasDetected());
    EXPECT_EQ(getContext()->DetectedInteractive(), pAisShape);
}

/**
 * @brief Checks that two callback are called when hovering objects changes
 */
TEST_F(TestOwnerHoverListener, event_moveTo_twoCallbacksOnHoveredObjectChange)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point1 = Graphic3d_Vec2i{windowSizeX / 4, windowSizeY / 2};
    const auto point2 = Graphic3d_Vec2i{3 * windowSizeX / 4, windowSizeY / 2};

    const auto pAisShape1 = makeAisBox();
    const auto pAisShape2 = makeAisBox(gp_Pnt{1.5, 0.0, 0.0});

    getContext()->Display(pAisShape1, true);
    getContext()->Display(pAisShape2, true);
    getView()->SetProj(V3d_TypeOfOrientation_Yup_Front);
    getView()->FitAll();

    getViewController().setOwnerHoverListener(getMockOwnerHoverListener());

    getViewController().UpdateMousePosition(point1, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(pAisShape1->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(point2, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStopHovered(pAisShape1->GlobalSelOwner()));
    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(pAisShape2->GlobalSelOwner()));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(getContext()->HasDetected());
    EXPECT_EQ(getContext()->DetectedInteractive(), pAisShape2);
}

/**
 * @brief Checks that the event handling is correct when user hovers over an AIS object and dynamic highlighting is
 * disallowed.
 */
TEST_F(TestOwnerHoverListener, event_moveTo_dynamicHighlightingDisallowed)
{
    int windowSizeX{};
    int windowSizeY{};
    getView()->Window()->Size(windowSizeX, windowSizeY);

    const auto point = Graphic3d_Vec2i{windowSizeX / 2, windowSizeY / 2};

    const auto pAisShape = makeAisBox();
    getContext()->Display(pAisShape, true);
    getView()->FitAll();

    getViewController().setOwnerHoverListener(getMockOwnerHoverListener());

    getViewController().SetAllowHighlight(false);
    getViewController().UpdateMousePosition(point, 0, 0, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockOwnerHoverListener(), handleOwnerStartHovered(_)).Times(0);
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_FALSE(getContext()->HasDetected());
}

} // namespace osis::test
