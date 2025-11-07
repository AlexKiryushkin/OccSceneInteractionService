
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_CLICK_HANDLER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_CLICK_HANDLER_H

#include <Aspect_VKeyFlags.hxx>
#include <Graphic3d_Vec2.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

namespace osis
{

/**
 * @brief An interface that allow to perform additional actions on mouse clicks in render thread. By mouse clicks we
 * mean pressing and releasing a mouse button in the same place on the screen within some tolerance.
 */
class IMouseClickHandler : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(IMouseClickHandler, Standard_Transient)

  public:
    IMouseClickHandler() = default;

    IMouseClickHandler(const IMouseClickHandler &) = default;
    IMouseClickHandler(IMouseClickHandler &&) = default;

    IMouseClickHandler &operator=(const IMouseClickHandler &) = default;
    IMouseClickHandler &operator=(IMouseClickHandler &&) = default;

    ~IMouseClickHandler() override = default;

    /**
     * @brief Returns true if given handler has some action to perform for given input. This method should be
     * thread-safe.
     * @param button Clicked button.
     * @param modifiers Pressed modifier while button is clicked.
     * @param isDoubleClick True if it is a double-click.
     * @return true if given handler has some action to perform.
     */
    virtual bool actsOn(Aspect_VKeyMouse button, Aspect_VKeyFlags modifiers, bool isDoubleClick) const = 0;

    /**
     * @brief Performs an action on mouse click. Is called from render thread.
     * @note This method is only called if \a actsOn returns true. So there is no need in additional check inside.
     * @param point Point where mouse was clicked.
     * @param button Clicked button.
     * @param modifiers Pressed modifier while button is clicked.
     * @param isDoubleClick True if it is a double-click.
     */
    virtual void handleMouseClicked(Graphic3d_Vec2i point, Aspect_VKeyMouse button, Aspect_VKeyFlags modifiers,
                                    bool isDoubleClick) = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_MOUSE_CLICK_HANDLER_H
