
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_HOVER_LISTENER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_HOVER_LISTENER_H

#include <Graphic3d_Vec2.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

namespace osis
{

/**
 * @brief An interface that allow to perform additional actions on mouse movement without any buttons pressed.
 */
class IMouseHoverListener : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(IMouseHoverListener, Standard_Transient)

  public:
    IMouseHoverListener() = default;

    IMouseHoverListener(const IMouseHoverListener &) = default;
    IMouseHoverListener(IMouseHoverListener &&) = default;

    IMouseHoverListener &operator=(const IMouseHoverListener &) = default;
    IMouseHoverListener &operator=(IMouseHoverListener &&) = default;

    ~IMouseHoverListener() override = default;

    /**
     * @brief Performs an action on mouse movement without any buttons pressed. Is called from Render thread.
     * @param point Current position of mouse.
     */
    virtual void onHover(Graphic3d_Vec2i point) = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_HOVER_LISTENER_H
