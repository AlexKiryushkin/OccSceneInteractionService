
#ifndef TESTS_USER_INTERACTION_SERVICE_MOCK_OWNER_HOVER_LISTENER_H
#define TESTS_USER_INTERACTION_SERVICE_MOCK_OWNER_HOVER_LISTENER_H

#include <OccSceneInteractionService/IOwnerHoverListener.h>

#include <gmock/gmock.h>

namespace osis::test
{

class MockOwnerHoverListener : public IOwnerHoverListener
{
  public:
    MockOwnerHoverListener() = default;
    ~MockOwnerHoverListener() override = default;

    MockOwnerHoverListener(const MockOwnerHoverListener &) = delete;
    MockOwnerHoverListener(MockOwnerHoverListener &&) = delete;

    MockOwnerHoverListener &operator=(const MockOwnerHoverListener &) = delete;
    MockOwnerHoverListener &operator=(MockOwnerHoverListener &&) = delete;

    MOCK_METHOD(void, handleOwnerStartHovered, (Handle(SelectMgr_EntityOwner)), (override));
    MOCK_METHOD(void, handleOwnerStopHovered, (Handle(SelectMgr_EntityOwner)), (override));
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_OWNER_HOVER_LISTENER_H
