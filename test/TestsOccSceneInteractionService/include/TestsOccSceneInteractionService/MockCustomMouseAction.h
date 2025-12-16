
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_CUSTOM_MOUSE_ACTION_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_CUSTOM_MOUSE_ACTION_H

#include <OccSceneInteractionService/CustomMouseAction.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockCustomMouseAction : public CustomMouseAction
{
  public:
    MockCustomMouseAction(UserActionInput userInput, std::vector<unsigned int> mouseCombinations,
                          Handle(IInputHandler) pInputHandler)
        : CustomMouseAction{std::move(userInput), std::move(mouseCombinations), std::move(pInputHandler)}
    {
    }

    ~MockCustomMouseAction() override = default;

    MockCustomMouseAction(const MockCustomMouseAction &) = delete;
    MockCustomMouseAction(MockCustomMouseAction &&) = delete;

    MockCustomMouseAction &operator=(const MockCustomMouseAction &) = delete;
    MockCustomMouseAction &operator=(MockCustomMouseAction &&) = delete;

    MOCK_METHOD(void, setUp, (), (override));
    MOCK_METHOD(void, tearDown, (), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_CUSTOM_MOUSE_ACTION_H
