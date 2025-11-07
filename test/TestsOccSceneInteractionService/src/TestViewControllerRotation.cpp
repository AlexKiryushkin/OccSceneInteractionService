
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

/**
 * @brief Tests mouse rotation scenarios.
 */
class TestViewControllerRotation : public TestViewControllerBase
{
  protected:
    Graphic3d_Vec2i m_defaultPoint{100, 200};
};

///
/// Tests for input for simple rotation actions
///

/**
 * @brief Checks that the UI and Render inputs are correct for start of the rotation
 */
TEST_F(TestViewControllerRotation, input_rotationStart)
{
    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    const auto needToUpdate = getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton,
                                                                     Aspect_VKeyFlags_CTRL, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_TRUE(guiInputBuffer.IsNewGesture);
    EXPECT_FALSE(guiInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(guiInputBuffer.OrbitRotation.ToStart);

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_TRUE(renderInputBuffer.IsNewGesture);
    EXPECT_FALSE(renderInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(renderInputBuffer.OrbitRotation.ToStart);
}

/**
 * @brief Checks that the UI and Render inputs are correct when rotation happens in one frame
 */
TEST_F(TestViewControllerRotation, input_rotation_sameFrame)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_TRUE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.OrbitRotation.ToRotate);
    EXPECT_TRUE(guiInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta});

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_TRUE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.OrbitRotation.ToRotate);
    EXPECT_TRUE(renderInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta});
}

/**
 * @brief Checks that the UI and Render inputs are correct when rotation happens in different frames
 */
TEST_F(TestViewControllerRotation, input_rotation_differentFrames)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.OrbitRotation.ToRotate);
    EXPECT_TRUE(guiInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta});

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.OrbitRotation.ToRotate);
    EXPECT_TRUE(renderInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta});
}

/**
 * @brief Checks that the UI and Render inputs are correct (empty) when rotation is disallowed
 */
TEST_F(TestViewControllerRotation, input_rotationDisallowed)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().SetAllowRotation(false);

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);

    EXPECT_FALSE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_FALSE(guiInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(guiInputBuffer.OrbitRotation.ToStart);

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_FALSE(renderInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(renderInputBuffer.OrbitRotation.ToStart);
}

/**
 * @brief Checks that the UI and Render inputs are correct when two rotation actions happen in different frames
 */
TEST_F(TestViewControllerRotation, input_rotationTwice_differentFrames)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{15, 30};

    auto &&guiInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = getViewController().InputBuffer(AIS_ViewInputBufferType_GL);

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_RightButton,
                                            Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    const auto needToUpdate = getViewController().UpdateMousePosition(
        m_defaultPoint + delta2, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(guiInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(guiInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta2});

    getViewController().FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.OrbitRotation.ToRotate);
    EXPECT_FALSE(renderInputBuffer.OrbitRotation.ToStart);
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointStart, Graphic3d_Vec2d{m_defaultPoint});
    EXPECT_EQ(renderInputBuffer.OrbitRotation.PointTo, Graphic3d_Vec2d{m_defaultPoint + delta2});
}

///
/// Tests for events for simple rotation actions
///

/**
 * @brief Checks that that the event handling is correct when rotation is disallowed. I.e. onCameraRotation is not
 * called.
 */
TEST_F(TestViewControllerRotation, event_rotationDisallowed_noNotify)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().SetAllowRotation(false);
    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL,
                                            false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation()).Times(0);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when rotation happens in one frame. I.e. onCameraRotation
 * callback is called.
 */
TEST_F(TestViewControllerRotation, event_rotation_sameFrame)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL,
                                            false);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation());
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when rotation happens in different frames. I.e. onCameraRotation
 * callback is called.
 */
TEST_F(TestViewControllerRotation, event_rotation_differentFrames)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL,
                                            false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation());
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when two rotation actions happen in one frame. I.e.
 * onCameraRotation callback is called only once.
 */
TEST_F(TestViewControllerRotation, event_rotationTwice_oneCallback)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_RightButton,
                                            Aspect_VKeyFlags_CTRL, false);
    getViewController().UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_RightButton,
                                            Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation());
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when two rotation actions happen in different frames. I.e.
 * onCameraRotation callback is called every frame.
 */
TEST_F(TestViewControllerRotation, event_rotationTwice_differentFrames)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};

    getViewController().setCameraListener(getMockCameraListener());

    getViewController().UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_RightButton, Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_RightButton,
                                            Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation());
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_RightButton,
                                            Aspect_VKeyFlags_CTRL, false);
    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onCameraRotation());
    getViewController().HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
