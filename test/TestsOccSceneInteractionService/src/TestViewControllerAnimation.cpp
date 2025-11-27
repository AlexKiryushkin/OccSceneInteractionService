
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <AIS_AnimationCamera.hxx>

namespace osis::test
{

using ::testing::_;

/**
 * @brief Tests animation callback scenarios.
 */
class TestViewControllerAnimation : public TestViewControllerBase
{
};

/**
 * @brief Just checks that we don't crash if there is no callback
 */
TEST_F(TestViewControllerAnimation, animation)
{
    getViewController().KeyDown(Aspect_VKey_ViewRight, 1.0);

    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the onAnimationStarted callback is called when animation starts. And not called on next frame.
 */
TEST_F(TestViewControllerAnimation, onAnimationStarted)
{
    getViewController().setCameraListener(getMockCameraListener());
    getViewController().KeyDown(Aspect_VKey_ViewRight, 1.0);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onAnimationStarted());
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*getMockCameraListener(), onAnimationStarted()).Times(0);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that callback is not called if we call abort animation when there was no animation in progress
 */
TEST_F(TestViewControllerAnimation, noCallbackOnAbortAnimationWhenNoAnimation)
{
    getViewController().setCameraListener(getMockCameraListener());

    EXPECT_CALL(*getMockCameraListener(), onAnimationStopped()).Times(0);
    getViewController().AbortViewAnimation();

    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

/**
 * @brief Checks that the onAnimationStopped callback is called when animation stops. And not called on next frame.
 */
TEST_F(TestViewControllerAnimation, onAnimationStopped)
{
    getViewController().setCameraListener(getMockCameraListener());
    getViewController().KeyDown(Aspect_VKey_ViewRight, 1.0);

    getViewController().FlushViewEvents(getContext(), getView(), false);

    EXPECT_CALL(*getMockCameraListener(), onAnimationStarted());
    getViewController().HandleViewEvents(getContext(), getView());

    getViewController().ViewAnimation()->Stop();

    EXPECT_CALL(*getMockCameraListener(), onAnimationStopped());
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());

    EXPECT_CALL(*getMockCameraListener(), onAnimationStopped()).Times(0);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

} // namespace osis::test
