
#include "TestsOccSceneInteractionService/GlfwOcctWindow.h"

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

GlfwOcctWindow::GlfwOcctWindow(int width, int height, const TCollection_AsciiString &title)
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
    }

#if !defined(_WIN32) && !defined(__APPLE__)
    m_display = new Aspect_DisplayConnection((Aspect_XDisplay *)glfwGetX11Display());
#endif
}

void GlfwOcctWindow::close()
{
    if(m_glfwWindow != nullptr)
    {
        glfwDestroyWindow(m_glfwWindow);
        m_glfwWindow = nullptr;
    }
}

Aspect_RenderingContext GlfwOcctWindow::nativeGlContext() const
{
#if defined(__APPLE__)
    return (NSOpenGLContext *)glfwGetNSGLContext(m_glfwWindow);
#elif defined(_WIN32)
    return glfwGetWGLContext(m_glfwWindow);
#else
    return glfwGetGLXContext(m_glfwWindow);
#endif
}

Aspect_Drawable GlfwOcctWindow::NativeHandle() const
{
#if defined(__APPLE__)
    return (Aspect_Drawable)glfwGetCocoaWindow(m_glfwWindow);
#elif defined(_WIN32)
    return (Aspect_Drawable)glfwGetWin32Window(m_glfwWindow);
#else
    return (Aspect_Drawable)glfwGetX11Window(m_glfwWindow);
#endif
}

Aspect_TypeOfResize GlfwOcctWindow::DoResize()
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

Standard_Boolean GlfwOcctWindow::IsMapped() const
{
    return glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE) != 0;
}

void GlfwOcctWindow::Map() const
{
    glfwShowWindow(m_glfwWindow);
}

void GlfwOcctWindow::Unmap() const
{
    glfwHideWindow(m_glfwWindow);
}

void GlfwOcctWindow::Position(Standard_Integer &x1, Standard_Integer &y1, Standard_Integer &x2,
                              Standard_Integer &y2) const
{
    x1 = m_xLeft;
    x2 = m_xRight;
    y1 = m_yTop;
    y2 = m_yBottom;
}

Standard_Real GlfwOcctWindow::Ratio() const
{
    return Standard_Real(m_xRight - m_xLeft) / Standard_Real(m_yBottom - m_yTop);
}

void GlfwOcctWindow::Size(Standard_Integer &width, Standard_Integer &height) const
{
    width = m_xRight - m_xLeft;
    height = m_yBottom - m_yTop;
}

Aspect_FBConfig GlfwOcctWindow::NativeFBConfig() const
{
    return nullptr;
}

} // namespace osis::test
