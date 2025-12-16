
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_INPUT_HANDLER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_INPUT_HANDLER_H

#include <OccSceneInteractionService/IInputHandler.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockInputHandler : public IInputHandler
{
  public:
    MockInputHandler() = default;
    ~MockInputHandler() override = default;

    MockInputHandler(const MockInputHandler &) = delete;
    MockInputHandler(MockInputHandler &&) = delete;

    MockInputHandler &operator=(const MockInputHandler &) = delete;
    MockInputHandler &operator=(MockInputHandler &&) = delete;

    MOCK_METHOD(void, handle, (const UserActionInputData &), (override));
};


} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_INPUT_HANDLER_H
