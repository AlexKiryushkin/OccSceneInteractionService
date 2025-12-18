
#ifndef TESTS_USER_INTERACTION_SERVICE_GLFW_OCCT_WINDOWS_H
#define TESTS_USER_INTERACTION_SERVICE_GLFW_OCCT_WINDOWS_H

#include <OccSceneInteractionService/IOwnerHoverListener.h>

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_RenderingContext.hxx>
#include <Aspect_Window.hxx>
#include <Graphic3d_Vec.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Standard_Type.hxx>
#include <TCollection_AsciiString.hxx>

struct GLFWwindow;

namespace osis::test
{

class GlfwOcctWindow : public Aspect_Window
{
  public:
    DEFINE_STANDARD_RTTI_INLINE(GlfwOcctWindow, Aspect_Window)

  public:
    GlfwOcctWindow(int width, int height, const TCollection_AsciiString &title);

    ~GlfwOcctWindow() override { close(); }

    void close();

    const Handle(Aspect_DisplayConnection) & getDisplay() const { return m_display; }

    Aspect_RenderingContext nativeGlContext() const;

    Aspect_Drawable NativeHandle() const override;

    Aspect_Drawable NativeParentHandle() const override { return 0; }

    Aspect_TypeOfResize DoResize() override;

    Standard_Boolean IsMapped() const override;

    Standard_Boolean DoMapping() const override { return true; }

    void Map() const override;

    void Unmap() const override;

    void Position(Standard_Integer &x1, Standard_Integer &y1, Standard_Integer &x2,
                  Standard_Integer &y2) const override;

    Standard_Real Ratio() const override;

    void Size(Standard_Integer &width, Standard_Integer &height) const override;

    Aspect_FBConfig NativeFBConfig() const override;

  protected:
    Handle(Aspect_DisplayConnection) m_display;
    GLFWwindow *m_glfwWindow;
    Standard_Integer m_xLeft;
    Standard_Integer m_yTop;
    Standard_Integer m_xRight;
    Standard_Integer m_yBottom;
};

} // namespace osis::test

#endif // TESTS_USER_INTERACTION_SERVICE_MOCK_OWNER_HOVER_LISTENER_H
