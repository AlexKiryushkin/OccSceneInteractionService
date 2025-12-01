
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_KEY_HANDLER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_KEY_HANDLER_H

#include <Aspect_VKey.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

namespace osis
{

/**
 * @brief An interface to perform some actions in render thread on key pressing and releasing. AIS_ViewController has
 * callbacks for these events but only in UI thread. This class allows to perform actions in Render thread.
 */
class IKeyHandler : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(IKeyHandler, Standard_Transient)

  public:
    IKeyHandler() = default;

    IKeyHandler(const IKeyHandler &) = default;
    IKeyHandler(IKeyHandler &&) = default;

    IKeyHandler &operator=(const IKeyHandler &) = default;
    IKeyHandler &operator=(IKeyHandler &&) = default;

    ~IKeyHandler() override = default;

    /**
     * @brief Callback called on key pressed. Is called from Render thread.
     * @param key Key that was pressed.
     */
    virtual void handleKeyPressed(Aspect_VKey key) = 0;

    /**
     * @brief Callback called on key released. Is called from Render thread.
     * @param key Key that was released.
     */
    virtual void handleKeyReleased(Aspect_VKey key) = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_KEY_HANDLER_H
