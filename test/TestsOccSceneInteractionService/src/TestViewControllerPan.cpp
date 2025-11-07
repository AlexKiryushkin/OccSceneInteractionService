
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

using ::testing::_;

/**
 * @brief Tests mouse pan scenarios.
 */
class TestViewControllerPan : public TestViewControllerBase
{
  protected:
    Graphic3d_Vec2i m_defaultPoint{100, 200};
};

///
/// Tests for input for simple panning actions
///

/**
 * @brief Checks that the UI and Render inputs are correct for start of the pan
 */
TEST_F(TestViewControllerPan, input_panStart)
{
    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    const auto needToUpdate = getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton,
                                                                     Aspect_VKeyFlags_SHIFT, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_TRUE(guiInputBuffer.IsNewGesture);
    EXPECT_FALSE(guiInputBuffer.Panning.ToPan);
    EXPECT_FALSE(guiInputBuffer.Panning.ToStart);

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_TRUE(renderInputBuffer.IsNewGesture);
    EXPECT_FALSE(renderInputBuffer.Panning.ToPan);
    EXPECT_FALSE(renderInputBuffer.Panning.ToStart);
}

/**
 * @brief Checks that the UI and Render inputs are correct when panning happens in one frame
 */
TEST_F(TestViewControllerPan, input_pan_sameFrame)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_TRUE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.Panning.ToPan);
    EXPECT_TRUE(guiInputBuffer.Panning.ToStart);
    EXPECT_EQ(guiInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(guiInputBuffer.Panning.Delta.x(), delta.x());
    EXPECT_EQ(guiInputBuffer.Panning.Delta.y(), -delta.y());

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_TRUE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.Panning.ToPan);
    EXPECT_TRUE(renderInputBuffer.Panning.ToStart);
    EXPECT_EQ(renderInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(renderInputBuffer.Panning.Delta.x(), delta.x());
    EXPECT_EQ(renderInputBuffer.Panning.Delta.y(), -delta.y());
}

/**
 * @brief Checks that the UI and Render inputs are correct when panning happens in different frames
 */
TEST_F(TestViewControllerPan, input_pan_differentFrames)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.Panning.ToPan);
    EXPECT_TRUE(guiInputBuffer.Panning.ToStart);
    EXPECT_EQ(guiInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(guiInputBuffer.Panning.Delta.x(), delta.x());
    EXPECT_EQ(guiInputBuffer.Panning.Delta.y(), -delta.y());

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.Panning.ToPan);
    EXPECT_TRUE(renderInputBuffer.Panning.ToStart);
    EXPECT_EQ(renderInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(renderInputBuffer.Panning.Delta.x(), delta.x());
    EXPECT_EQ(renderInputBuffer.Panning.Delta.y(), -delta.y());
}

/**
 * @brief Checks that the UI and Render inputs are correct (empty) when panning is disallowed
 */
TEST_F(TestViewControllerPan, input_panDisallowed)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().SetAllowPanning(false);

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    EXPECT_FALSE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_FALSE(guiInputBuffer.Panning.ToPan);
    EXPECT_FALSE(guiInputBuffer.Panning.ToStart);

    getViewController().FlushViewEvents(getContext(), getView());
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_FALSE(renderInputBuffer.Panning.ToPan);
    EXPECT_FALSE(renderInputBuffer.Panning.ToStart);
}

/**
 * @brief Checks that the UI and Render inputs are correct when two panning actions happen in different frames
 */
TEST_F(TestViewControllerPan, input_panTwice_differentFrames)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{15, 30};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_LeftButton,
                                            Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta2, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.Panning.ToPan);
    EXPECT_FALSE(guiInputBuffer.Panning.ToStart);
    EXPECT_EQ(guiInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(guiInputBuffer.Panning.Delta.x(), delta2.x() - delta1.x());
    EXPECT_EQ(guiInputBuffer.Panning.Delta.y(), -delta2.y() + delta1.y());

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.Panning.ToPan);
    EXPECT_FALSE(renderInputBuffer.Panning.ToStart);
    EXPECT_EQ(renderInputBuffer.Panning.PointStart, m_defaultPoint);
    EXPECT_EQ(renderInputBuffer.Panning.Delta.x(), delta2.x() - delta1.x());
    EXPECT_EQ(renderInputBuffer.Panning.Delta.y(), -delta2.y() + delta1.y());
}

///
/// Tests for events for simple panning actions
///

/**
 * @brief Checks that that the event handling is correct when pan is disallowed. I.e. onCameraCenterChanged is not
 * called.
 */
TEST_F(TestViewControllerPan, event_panDisallowed_noNotify)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().SetAllowPanning(false);
    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT,
                                            false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).Times(0);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when panning happens in one frame. I.e. onCameraCenterChanged
 * callback is called.
 */
TEST_F(TestViewControllerPan, event_pan_sameFrame)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT,
                                            false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    gp_Pnt newCenter;
    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).WillOnce(::testing::SaveArg<0>(&newCenter));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(newCenter.IsEqual(getView()->Camera()->Center(), 1e-7));
}

/**
 * @brief Checks that the event handling is correct when panning happens in different frames. I.e. onCameraCenterChanged
 * callback is called.
 */
TEST_F(TestViewControllerPan, event_pan_differentFrames)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT,
                                            false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    gp_Pnt newCenter;
    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).WillOnce(::testing::SaveArg<0>(&newCenter));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(newCenter.IsEqual(getView()->Camera()->Center(), 1e-7));
}

/**
 * @brief Checks that the event handling is correct when two panning actions happen in one frame. I.e.
 * onCameraCenterChanged callback is called only once.
 */
TEST_F(TestViewControllerPan, event_panTwice_oneCallback)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_LeftButton,
                                            Aspect_VKeyFlags_SHIFT, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_LeftButton,
                                            Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    gp_Pnt newCenter;
    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).WillOnce(::testing::SaveArg<0>(&newCenter));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(newCenter.IsEqual(getView()->Camera()->Center(), 1e-7));
}

/**
 * @brief Checks that the event handling is correct when two panning actions happen in different frames. I.e.
 * onCameraCenterChanged callback is called every frame.
 */
TEST_F(TestViewControllerPan, event_panTwice_differentFrames)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_LeftButton,
                                            Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    gp_Pnt newCenter;
    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).WillOnce(::testing::SaveArg<0>(&newCenter));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(newCenter.IsEqual(getView()->Camera()->Center(), 1e-7));

    getViewController().UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_LeftButton,
                                            Aspect_VKeyFlags_SHIFT, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraCenterChanged(_)).WillOnce(::testing::SaveArg<0>(&newCenter));
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_TRUE(newCenter.IsEqual(getView()->Camera()->Center(), 1e-7));
}

} // namespace osis::test
