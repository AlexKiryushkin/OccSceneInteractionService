
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include "TestsOccSceneInteractionService/GlfwOcctWindow.h"

#include <GLFW/glfw3.h>

namespace osis::test
{

TestViewControllerBase::TestViewControllerBase()
{
    glfwInit();

    Handle(GlfwOcctWindow) pWindow = new GlfwOcctWindow(640, 480, "Window");

    Handle(OpenGl_GraphicDriver) pGraphicDriver = new OpenGl_GraphicDriver(pWindow->getDisplay());

    Handle(V3d_Viewer) pViewer = new V3d_Viewer(pGraphicDriver);

    m_view = new V3d_View(pViewer);

    m_interactiveContext = new AIS_InteractiveContext(pViewer);


    m_view->SetWindow(pWindow, pWindow->nativeGlContext());
}

} // namespace osis::test
