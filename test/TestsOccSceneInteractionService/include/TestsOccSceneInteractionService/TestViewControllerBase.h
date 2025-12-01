
#ifndef TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H
#define TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H

#include "TestsOccSceneInteractionService/MockCameraListener.h"
#include "TestsOccSceneInteractionService/MockKeyHandler.h"
#include "TestsOccSceneInteractionService/MockMouseClickHandler.h"
#include "TestsOccSceneInteractionService/MockMouseHoverListener.h"
#include "TestsOccSceneInteractionService/MockOwnerHoverListener.h"

#include <OccSceneInteractionService/ViewController.h>

#include <gtest/gtest.h>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

namespace osis::test
{

class TestViewControllerBase : public testing::Test
{
  protected:
    template <class T>
    using StrictHandle = Handle(testing::StrictMock<T>);

    TestViewControllerBase();

    Handle(V3d_View) getView() const { return m_view; }

    Handle(AIS_InteractiveContext) getContext() const { return m_interactiveContext; }

    StrictHandle<MockCameraListener> getMockCameraListener() const { return m_pMockCameraListener; }

    StrictHandle<MockMouseClickHandler> getMockMouseClickHandler() const { return m_pMockMouseClickHandler; }

    StrictHandle<MockOwnerHoverListener> getMockOwnerHoverListener() const { return m_pMockOwnerHoverListener; }

    StrictHandle<MockMouseHoverListener> getMockMouseHoverListener() const { return m_pMockMouseHoverListener; }

    StrictHandle<MockKeyHandler> getMockKeyHandler() const { return m_pMockKeyHandler; }

    osis::ViewController &getViewController() { return m_aisViewController; }

  private:
    osis::ViewController m_aisViewController;

    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_interactiveContext;

    StrictHandle<MockCameraListener> m_pMockCameraListener = new testing::StrictMock<MockCameraListener>();
    StrictHandle<MockMouseClickHandler> m_pMockMouseClickHandler = new testing::StrictMock<MockMouseClickHandler>();
    StrictHandle<MockOwnerHoverListener> m_pMockOwnerHoverListener = new testing::StrictMock<MockOwnerHoverListener>();
    StrictHandle<MockMouseHoverListener> m_pMockMouseHoverListener = new testing::StrictMock<MockMouseHoverListener>();
    StrictHandle<MockKeyHandler> m_pMockKeyHandler = new testing::StrictMock<MockKeyHandler>();
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H
