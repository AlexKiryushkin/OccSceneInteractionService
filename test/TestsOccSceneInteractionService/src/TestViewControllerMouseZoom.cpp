
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

using ::testing::_;

/**
 * @brief Tests mouse zoom scenarios.
 */
class TestViewControllerMouseZoom : public TestViewControllerBase
{
  protected:
    Graphic3d_Vec2i m_defaultPoint{100, 200};
};

///
/// Tests for input for simple zoom actions
///

/**
 * @brief Checks that the UI and Render inputs are correct for start of the zoom
 */
TEST_F(TestViewControllerMouseZoom, input_zoomStart)
{
    auto &&guiInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_GL);

    const auto needToUpdate = m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton,
                                                                     Aspect_VKeyFlags_CTRL, false);

    EXPECT_TRUE(needToUpdate);
    EXPECT_TRUE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.ZoomActions.IsEmpty());

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    EXPECT_TRUE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.ZoomActions.IsEmpty());
}

/**
 * @brief Checks that the UI and Render inputs are correct when zooming happens in one frame
 */
TEST_F(TestViewControllerMouseZoom, input_zoom_sameFrame)
{
    auto &&guiInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_GL);

    for(auto delta :
        {Graphic3d_Vec2i{10, 10}, Graphic3d_Vec2i{10, -10}, Graphic3d_Vec2i{-10, 10}, Graphic3d_Vec2i{-10, -10}})
    {
        m_aisViewController.ResetViewInput();

        m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                               false);
        const auto needToUpdate = m_aisViewController.UpdateMousePosition(
            m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);

        EXPECT_TRUE(needToUpdate);
        EXPECT_TRUE(guiInputBuffer.IsNewGesture);
        EXPECT_EQ(guiInputBuffer.ZoomActions.Size(), 1ULL);
        EXPECT_FALSE(guiInputBuffer.ZoomActions.First().HasPoint());

        // same signs
        EXPECT_GT(guiInputBuffer.ZoomActions.First().Delta * delta.x(), 0.0);

        m_aisViewController.FlushViewEvents(getContext(), getView(), false);
        EXPECT_TRUE(renderInputBuffer.IsNewGesture);
        EXPECT_EQ(renderInputBuffer.ZoomActions.Size(), 1ULL);
        EXPECT_FALSE(renderInputBuffer.ZoomActions.First().HasPoint());

        // same signs
        EXPECT_GT(renderInputBuffer.ZoomActions.First().Delta * delta.x(), 0.0);
    }
}

/**
 * @brief Checks that the UI and Render inputs are correct when zooming happens in different frames
 */
TEST_F(TestViewControllerMouseZoom, input_zoom_differentFrames)
{
    auto &&guiInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_GL);

    for(auto delta :
        {Graphic3d_Vec2i{10, 10}, Graphic3d_Vec2i{10, -10}, Graphic3d_Vec2i{-10, 10}, Graphic3d_Vec2i{-10, -10}})
    {
        m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                               false);
        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        const auto needToUpdate = m_aisViewController.UpdateMousePosition(
            m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);

        EXPECT_TRUE(needToUpdate);
        EXPECT_FALSE(guiInputBuffer.IsNewGesture);
        EXPECT_EQ(guiInputBuffer.ZoomActions.Size(), 1ULL);
        EXPECT_FALSE(guiInputBuffer.ZoomActions.First().HasPoint());

        // same signs
        EXPECT_GT(guiInputBuffer.ZoomActions.First().Delta * delta.x(), 0.0);

        m_aisViewController.FlushViewEvents(getContext(), getView(), false);
        EXPECT_FALSE(renderInputBuffer.IsNewGesture);
        EXPECT_EQ(renderInputBuffer.ZoomActions.Size(), 1ULL);
        EXPECT_FALSE(renderInputBuffer.ZoomActions.First().HasPoint());

        // same signs
        EXPECT_GT(renderInputBuffer.ZoomActions.First().Delta * delta.x(), 0.0);
    }
}

/**
 * @brief Checks that the UI and Render inputs are correct (empty) when zooming is disallowed
 */
TEST_F(TestViewControllerMouseZoom, input_zoomDisallowed)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    m_aisViewController.SetAllowZooming(false);

    auto &&guiInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_UI);
    auto &&renderInputBuffer = m_aisViewController.InputBuffer(AIS_ViewInputBufferType_GL);

    m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);

    const auto needToUpdate = m_aisViewController.UpdateMousePosition(
        m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);

    EXPECT_FALSE(needToUpdate);
    EXPECT_FALSE(guiInputBuffer.IsNewGesture);
    EXPECT_TRUE(guiInputBuffer.ZoomActions.IsEmpty());

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    EXPECT_FALSE(renderInputBuffer.IsNewGesture);
    EXPECT_TRUE(renderInputBuffer.ZoomActions.IsEmpty());
}

///
/// Tests for events for simple zoom actions
///

/**
 * @brief Checks that the event handling is correct when zooming happens in one frame. I.e. onCameraScaleChanged
 * callback is called with correct argument
 */
TEST_F(TestViewControllerMouseZoom, event_zoom_sameFrame)
{
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    for(auto delta :
        {Graphic3d_Vec2i{10, 10}, Graphic3d_Vec2i{10, -10}, Graphic3d_Vec2i{-10, 10}, Graphic3d_Vec2i{-10, -10}})
    {
        m_aisViewController.ResetViewInput();

        m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                               false);
        m_aisViewController.UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton,
                                                Aspect_VKeyFlags_CTRL, false);

        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        double zoomDelta{};
        EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).WillOnce(testing::SaveArg<0>(&zoomDelta));
        m_aisViewController.HandleViewEvents(getContext(), getView());

        // same signs
        EXPECT_GT(zoomDelta * delta.x(), 0.0);
    }
}

/**
 * @brief Checks that the event handling is correct when zooming happens in different frames. I.e. onCameraScaleChanged
 * callback is called with correct argument
 */
TEST_F(TestViewControllerMouseZoom, event_zoom_differentFrames)
{
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    for(auto delta :
        {Graphic3d_Vec2i{10, 10}, Graphic3d_Vec2i{10, -10}, Graphic3d_Vec2i{-10, 10}, Graphic3d_Vec2i{-10, -10}})
    {
        m_aisViewController.ResetViewInput();

        m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                               false);
        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        m_aisViewController.UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton,
                                                Aspect_VKeyFlags_CTRL, false);
        m_aisViewController.FlushViewEvents(getContext(), getView(), false);

        double zoomDelta{};
        EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).WillOnce(testing::SaveArg<0>(&zoomDelta));
        m_aisViewController.HandleViewEvents(getContext(), getView());

        // same signs
        EXPECT_GT(zoomDelta * delta.x(), 0.0);
    }
}

/**
 * @brief Checks that the event handling is correct when zoom is disallowed. I.e. onCameraScaleChanged is not called.
 */
TEST_F(TestViewControllerMouseZoom, event_zoomDisallowed_noNotify)
{
    const auto delta = Graphic3d_Vec2i{10, 10};

    m_aisViewController.SetAllowZooming(false);
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);
    m_aisViewController.UpdateMousePosition(m_defaultPoint + delta, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                            false);

    EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).Times(0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the event handling is correct when two zooming actions happen in one frame. I.e.
 * onCameraScaleChanged callback is called only once.
 */
TEST_F(TestViewControllerMouseZoom, event_zoomTwice_oneCallback)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);
    m_aisViewController.UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                            false);
    m_aisViewController.UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                            false);

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    double zoomDelta{};

    EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).WillOnce(testing::SaveArg<0>(&zoomDelta));
    m_aisViewController.HandleViewEvents(getContext(), getView());

    // same signs
    EXPECT_GT(zoomDelta, 0.0);
}

/**
 * @brief Checks that the event handling is correct when two zooming actions happen in different frames. I.e.
 * onCameraScaleChanged callback is called every frame.
 */
TEST_F(TestViewControllerMouseZoom, event_zoomTwiceInDifferentFrames)
{
    const auto delta1 = Graphic3d_Vec2i{10, 10};
    const auto delta2 = Graphic3d_Vec2i{20, 20};
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    m_aisViewController.UpdateMouseButtons(m_defaultPoint, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL, false);
    m_aisViewController.UpdateMousePosition(m_defaultPoint + delta1, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                            false);

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    double zoomDelta{};

    EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).WillOnce(testing::SaveArg<0>(&zoomDelta));
    m_aisViewController.HandleViewEvents(getContext(), getView());
    EXPECT_GT(zoomDelta, 0.0);

    m_aisViewController.UpdateMousePosition(m_defaultPoint + delta2, Aspect_VKeyMouse_LeftButton, Aspect_VKeyFlags_CTRL,
                                            false);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockCameraListener, onCameraScaleChanged(_)).WillOnce(testing::SaveArg<0>(&zoomDelta));
    m_aisViewController.HandleViewEvents(getContext(), getView());
    EXPECT_GT(zoomDelta, 0.0);
}

} // namespace osis::test
