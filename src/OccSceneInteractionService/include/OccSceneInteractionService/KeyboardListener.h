
#ifndef OCC_SCENE_INTERACTION_SERVICE_KEYBOARD_LISTENER_H
#define OCC_SCENE_INTERACTION_SERVICE_KEYBOARD_LISTENER_H

#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>

#include <Aspect_VKey.hxx>

#include <cstdint>
#include <vector>

namespace osis
{

/**
 * @brief This class is needed to listen to keyboard events on UI side and pass them to Render side. Default OCC
 * implementation only has a callback on UI side.
 */
class KeyboardListener
{
  public:
    /**
     * @brief user action on key: whether it was pressed or released
     */
    enum class KeyAction : std::uint8_t
    {
        Pressed,
        Released
    };

    /**
     * @brief Key state after user action: contains a key and an action on this key
     */
    struct KeyState
    {
        Aspect_VKey key;
        KeyAction keyAction;
    };

    /**
     * @brief Returns key states gathered in UI thread. Should be called in UI thread.
     * @return Key states gathered in UI thread.
     */
    OSIS_API [[nodiscard]] const std::vector<KeyState> &getUiKeyStates() const;

    /**
     * @brief Returns key states in Render thread that were synced from UI thread. Should be called in Render thread.
     * @return Key states in Render thread that were synced from UI thread.
     */
    OSIS_API [[nodiscard]] const std::vector<KeyState> &getRenderKeyStates() const;

    /**
     * @brief Handling of a key press event. Should be called in UI thread.
     * @param key Key that was pressed.
     */
    OSIS_API void onKeyPressed(Aspect_VKey key);

    /**
     * @brief Handling of a key release event. Should be called in UI thread.
     * @param key Key that was released.
     */
    OSIS_API void onKeyReleased(Aspect_VKey key);

    /**
     * @brief Synchronizes UI and Render user inputs. Should be called when UI and Render threads are synced.
     */
    OSIS_API void sync();

    /**
     * @brief Reset user UI input. Usually called when window lost focus. Should be called from UI thread.
     */
    OSIS_API void clearUiInput();

    /**
     * @brief Resets user Render input. Should be called after render input was handled in order not to handle it again.
     * Should be called from Render thread.
     */
    OSIS_API void clearRenderInput();

  private:
    std::vector<KeyState> m_uiKeyStates;
    std::vector<KeyState> m_renderKeyStates;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_KEYBOARD_LISTENER_H
