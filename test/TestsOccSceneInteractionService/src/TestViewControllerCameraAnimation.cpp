
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

namespace osis::test
{

/**
 * @brief Tests camera animation scenarios.
 */
class TestViewControllerCameraAnimation : public TestViewControllerBase
{
};

/**
 * @brief Checks that no callback is called when camera listener is NULL
 */
TEST_F(TestViewControllerCameraAnimation, noCallback_NoCameraListener)
{
    m_aisViewController.KeyDown(Aspect_VKey_ViewTop, 0.0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    m_aisViewController.HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that callback is called when camera listener is not NULL.
 * And that callback is not called when animation is in progress on the subsequent frames.
 */
TEST_F(TestViewControllerCameraAnimation, callback_cameraListener)
{
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    m_aisViewController.KeyDown(Aspect_VKey_ViewTop, 0.0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStarted());
    m_aisViewController.HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStarted()).Times(0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that callback is not called if we call abort animation when there was no animation in progress
 */
TEST_F(TestViewControllerCameraAnimation, noCallbackOnAbortAnimationWhenNoAnimation)
{
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStopped()).Times(0);
    m_aisViewController.AbortViewAnimation();

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that callback is called if we call abort animation when there was animation in progress.
 * Also checks that a callback is not called on subsequent frames.
 */
TEST_F(TestViewControllerCameraAnimation, callbackOnAbortAnimation)
{
    m_aisViewController.setCameraListener(m_pMockCameraListener);

    m_aisViewController.KeyDown(Aspect_VKey_ViewTop, 0.0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStarted());
    m_aisViewController.HandleViewEvents(getContext(), getView());

    m_aisViewController.AbortViewAnimation();

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStopped());
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*m_pMockCameraListener, onAnimationStopped()).Times(0);
    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());

    m_aisViewController.FlushViewEvents(getContext(), getView(), false);
    m_aisViewController.HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
