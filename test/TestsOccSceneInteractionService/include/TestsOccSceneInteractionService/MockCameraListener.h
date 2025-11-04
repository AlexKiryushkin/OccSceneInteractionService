
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_CAMERA_LISTENER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_CAMERA_LISTENER_H

#include <OccSceneInteractionService/ICameraListener.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockCameraListener : public ICameraListener
{
  public:
    MockCameraListener() = default;
    ~MockCameraListener() override = default;

    MockCameraListener(const MockCameraListener &) = delete;
    MockCameraListener(MockCameraListener &&) = delete;

    MockCameraListener &operator=(const MockCameraListener &) = delete;
    MockCameraListener &operator=(MockCameraListener &&) = delete;

    MOCK_METHOD(void, onAnimationStarted, (), (override));
    MOCK_METHOD(void, onAnimationStopped, (), (override));

    MOCK_METHOD(void, onCameraScaleChanged, (double), (override));
    MOCK_METHOD(void, onCameraCenterChanged, (gp_Pnt), (override));
    MOCK_METHOD(void, onCameraRotation, (), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_CAMERA_LISTENER_H
