
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_KEY_HANDLER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_KEY_HANDLER_H

#include <OccSceneInteractionService/IKeyHandler.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockKeyHandler : public IKeyHandler
{
  public:
    MockKeyHandler() = default;
    ~MockKeyHandler() override = default;

    MockKeyHandler(const MockKeyHandler &) = delete;
    MockKeyHandler(MockKeyHandler &&) = delete;

    MockKeyHandler &operator=(const MockKeyHandler &) = delete;
    MockKeyHandler &operator=(MockKeyHandler &&) = delete;

    MOCK_METHOD(void, handleKeyPressed, (Aspect_VKey), (override));
    MOCK_METHOD(void, handleKeyReleased, (Aspect_VKey), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_KEY_HANDLER_H
