
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace osis::test
{

TestViewControllerBase::TestViewControllerBase()
    : testing::Test{}
{
    Handle(Aspect_DisplayConnection) pDisplayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) pGraphicDriver = new OpenGl_GraphicDriver(pDisplayConnection, false);

    Handle(V3d_Viewer) pViewer = new V3d_Viewer(pGraphicDriver);

    m_view = new V3d_View(pViewer);

    m_interactiveContext = new AIS_InteractiveContext(pViewer);

    Handle(Aspect_NeutralWindow) pWindow = new Aspect_NeutralWindow();
    pWindow->SetSize(640, 480);

    m_view->SetWindow(pWindow);
}

} // namespace osis::test
