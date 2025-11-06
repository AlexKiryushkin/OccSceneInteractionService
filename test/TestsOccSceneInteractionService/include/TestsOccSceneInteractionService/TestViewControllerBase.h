
#ifndef TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H
#define TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H

#include "TestsOccSceneInteractionService/MockCameraListener.h"
#include "TestsOccSceneInteractionService/MockMouseClickHandler.h"

#include <OccSceneInteractionService/ViewController.h>

#include <gtest/gtest.h>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

namespace osis::test
{

class TestViewControllerBase : public testing::Test
{
  protected:
    TestViewControllerBase();

    Handle(V3d_View) getView() const { return m_view; }

    Handle(AIS_InteractiveContext) getContext() const { return m_interactiveContext; }

  protected:
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_interactiveContext;

    osis::ViewController m_aisViewController;
    Handle(testing::StrictMock<MockCameraListener>)
        m_pMockCameraListener = new testing::StrictMock<MockCameraListener>();
    Handle(testing::StrictMock<MockMouseClickHandler>)
        m_pMockMouseClickHandler = new testing::StrictMock<MockMouseClickHandler>();
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_TEST_VIEWER_CONTROLLER_BASE_H
