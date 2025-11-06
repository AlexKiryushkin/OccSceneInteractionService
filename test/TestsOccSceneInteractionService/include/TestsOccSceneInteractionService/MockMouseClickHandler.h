
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_CLICK_HANDLER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_CLICK_HANDLER_H

#include <OccSceneInteractionService/IMouseClickHandler.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockMouseClickHandler : public IMouseClickHandler
{
  public:
    MockMouseClickHandler() = default;
    ~MockMouseClickHandler() override = default;

    MockMouseClickHandler(const MockMouseClickHandler &) = delete;
    MockMouseClickHandler(MockMouseClickHandler &&) = delete;

    MockMouseClickHandler &operator=(const MockMouseClickHandler &) = delete;
    MockMouseClickHandler &operator=(MockMouseClickHandler &&) = delete;

    MOCK_METHOD(bool, actsOn, (Aspect_VKeyMouse, Aspect_VKeyFlags, bool), (const, override));
    MOCK_METHOD(void, handleMouseClicked, (Graphic3d_Vec2i, Aspect_VKeyMouse, Aspect_VKeyFlags, bool), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_MOUSE_CLICK_HANDLER_H
