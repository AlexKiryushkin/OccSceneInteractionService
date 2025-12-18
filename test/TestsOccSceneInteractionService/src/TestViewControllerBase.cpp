
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <OpenGl_GraphicDriver.hxx>

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_RenderingContext.hxx>
#include <Aspect_Window.hxx>
#include <Graphic3d_Vec.hxx>
#include <TCollection_AsciiString.hxx>

#if defined(__APPLE__)
#include <Cocoa_Window.hxx>
#elif defined(_WIN32)
#include <WNT_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

#if defined(__APPLE__)
#undef Handle // avoid name collisions in macOS headers
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace osis::test
{

namespace
{

//! GLFWwindow wrapper implementing Aspect_Window interface.
class GlfwOcctWindow : public Aspect_Window
{
  public:
    DEFINE_STANDARD_RTTI_INLINE(GlfwOcctWindow, Aspect_Window)

  public:
    //! Main constructor.
    GlfwOcctWindow(int width, int height, const TCollection_AsciiString &title)
        : m_glfwWindow(glfwCreateWindow(width, height, title.ToCString(), nullptr, nullptr))
        , m_xLeft(0)
        , m_yTop(0)
        , m_xRight(0)
        , m_yBottom(0)
    {
        if(m_glfwWindow != nullptr)
        {
            int aWidth = 0, aHeight = 0;
            glfwGetWindowPos(m_glfwWindow, &m_xLeft, &m_yTop);
            glfwGetWindowSize(m_glfwWindow, &aWidth, &aHeight);

            m_xRight = m_xLeft + aWidth;
            m_yBottom = m_yTop + aHeight;

#if !defined(_WIN32) && !defined(__APPLE__)
            m_display = new Aspect_DisplayConnection((Aspect_XDisplay *)glfwGetX11Display());
#endif
        }
    }

    //! Close the window.
    ~GlfwOcctWindow() override { Close(); }

    //! Close the window.
    void Close()
    {
        if(m_glfwWindow != nullptr)
        {
            glfwDestroyWindow(m_glfwWindow);
            m_glfwWindow = nullptr;
        }
    }

    //! Return X Display connection.
    const Handle(Aspect_DisplayConnection) & GetDisplay() const { return m_display; }

    //! Return native OpenGL context.
    Aspect_RenderingContext NativeGlContext() const
    {
#if defined(__APPLE__)
        return (NSOpenGLContext *)glfwGetNSGLContext(myGlfwWindow);
#elif defined(_WIN32)
        return glfwGetWGLContext(m_glfwWindow);
#else
        return glfwGetGLXContext(myGlfwWindow);
#endif
    }

  public:
    Aspect_Drawable NativeHandle() const override
    {
#if defined(__APPLE__)
        return (Aspect_Drawable)glfwGetCocoaWindow(myGlfwWindow);
#elif defined(_WIN32)
        return (Aspect_Drawable)glfwGetWin32Window(m_glfwWindow);
#else
        return (Aspect_Drawable)glfwGetX11Window(myGlfwWindow);
#endif
    }

    //! Returns parent of native Window handle.
    Aspect_Drawable NativeParentHandle() const override { return 0; }

    Aspect_TypeOfResize DoResize() override
    {
        if(glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE) == 1)
        {
            int xPos = 0;
            int yPos = 0;
            int width = 0;
            int height = 0;

            glfwGetWindowPos(m_glfwWindow, &xPos, &yPos);
            glfwGetWindowSize(m_glfwWindow, &width, &height);

            m_xLeft = xPos;
            m_xRight = xPos + width;
            m_yTop = yPos;
            m_yBottom = yPos + height;
        }

        return Aspect_TOR_UNKNOWN;
    }

    Standard_Boolean IsMapped() const override { return glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE) != 0; }

    Standard_Boolean DoMapping() const override { return true; }

    void Map() const override { glfwShowWindow(m_glfwWindow); }

    void Unmap() const override { glfwHideWindow(m_glfwWindow); }

    void Position(Standard_Integer &x1, Standard_Integer &y1, Standard_Integer &x2, Standard_Integer &y2) const override
    {
        x1 = m_xLeft;
        x2 = m_xRight;
        y1 = m_yTop;
        y2 = m_yBottom;
    }

    Standard_Real Ratio() const override
    {
        return Standard_Real(m_xRight - m_xLeft) / Standard_Real(m_yBottom - m_yTop);
    }

    void Size(Standard_Integer &width, Standard_Integer &height) const override
    {
        width = m_xRight - m_xLeft;
        height = m_yBottom - m_yTop;
    }

    Aspect_FBConfig NativeFBConfig() const override { return nullptr; }

  protected:
    Handle(Aspect_DisplayConnection) m_display;
    GLFWwindow *m_glfwWindow;
    Standard_Integer m_xLeft;
    Standard_Integer m_yTop;
    Standard_Integer m_xRight;
    Standard_Integer m_yBottom;
};

} // namespace

TestViewControllerBase::TestViewControllerBase()
{
    glfwInit();

#if !defined(_WIN32) && !defined(__APPLE__)
    Handle(Aspect_DisplayConnection) pDisplayConnection =
        new Aspect_DisplayConnection((Aspect_XDisplay *)glfwGetX11Display());
#else
    Handle(Aspect_DisplayConnection) pDisplayConnection = new Aspect_DisplayConnection();
#endif

    Handle(OpenGl_GraphicDriver) pGraphicDriver = new OpenGl_GraphicDriver(pDisplayConnection, false);

    Handle(V3d_Viewer) pViewer = new V3d_Viewer(pGraphicDriver);

    m_view = new V3d_View(pViewer);

    m_interactiveContext = new AIS_InteractiveContext(pViewer);

    Handle(GlfwOcctWindow) pWindow = new GlfwOcctWindow(640, 480, "Window");

    m_view->SetWindow(pWindow, pWindow->NativeGlContext());
}

} // namespace osis::test
