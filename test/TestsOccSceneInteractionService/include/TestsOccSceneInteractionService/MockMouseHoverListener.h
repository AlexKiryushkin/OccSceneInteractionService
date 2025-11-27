
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_HOVER_LISTENER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_HOVER_LISTENER_H

#include <OccSceneInteractionService/IMouseHoverListener.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockMouseHoverListener : public IMouseHoverListener
{
  public:
    MockMouseHoverListener() = default;
    ~MockMouseHoverListener() override = default;

    MockMouseHoverListener(const MockMouseHoverListener &) = delete;
    MockMouseHoverListener(MockMouseHoverListener &&) = delete;

    MockMouseHoverListener &operator=(const MockMouseHoverListener &) = delete;
    MockMouseHoverListener &operator=(MockMouseHoverListener &&) = delete;

    MOCK_METHOD(void, onHover, (Graphic3d_Vec2i), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_HOVER_LISTENER_H
