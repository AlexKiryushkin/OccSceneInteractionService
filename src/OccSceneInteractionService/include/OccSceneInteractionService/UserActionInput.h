
#ifndef OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_H
#define OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_H

#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>
#include <OccSceneInteractionService/UserActionInputData.h>

#include <cstdint>
#include <optional>

namespace osis
{

/**
 * @brief This class is responsible for gathering user action input in GUI thread and copying it to render thread
 * without data races.
 */
class UserActionInput
{
  public:
    enum class ApplyOn : std::uint8_t
    {
        Continue = 0x02,
        Stop = 0x04
    };

    UserActionInput(ApplyOn applyOn, std::optional<std::size_t> maxInputPoints);

    /**
     * @brief Returns user input data gathered in UI thread. Should be called in UI thread.
     * @return User input data gathered in UI thread.
     */
    [[nodiscard]] const UserActionInputData &getUiUserInputData() const { return m_uiInputData; }

    /**
     * @brief Returns user input data in Render thread that was synced from UI thread. Should be called in Render thread
     * @return User input data in Render thread that was synced from UI thread.
     */
    [[nodiscard]] const UserActionInputData &getRenderUserInputData() const { return m_renderInputData; }

    /**
     * @brief This method is called once we start gathering user input. Should be called in UI thread.
     * @param point Mouse position at which we start gathering input.
     */
    void startGatherInput(const Graphic3d_Vec2i &point);

    /**
     * @brief This method is called while we continue gathering user input. Should be called in UI thread.
     * @param point Mouse position at which we continue gathering input.
     */
    void continueGatherInput(const Graphic3d_Vec2i &point);

    /**
     * @brief This method is called once we stop gathering user input. Should be called in UI thread.
     * @param point Mouse position at which we stop gathering input.
     */
    void stopGatherInput(const Graphic3d_Vec2i &point);

    /**
     * @brief Synchronizes UI and Render user inputs. Should be called when UI and Render threads are synced.
     */
    void sync();

    /**
     * @brief Reset user UI input. Usually called when window lost focus. Should be called from UI thread.
     */
    void resetUiInput();

    /**
     * @brief Resets user Render input. Should be called after render input was handled in order not to handle it again.
     * Should be called from Render thread.
     */
    void resetRenderInput();

  private:
    /**
     * @brief Adds point if current number plus added point does not exceed m_maxInputPoints. Otherwise overwrites
     * the last value.
     * @param point Point to be added.
     */
    void addPoint(const Graphic3d_Vec2i &point);

  private:
    enum class ActionState : std::uint8_t
    {
        NotEntered,
        Started,
        Stopped
    };

  private:
    ApplyOn m_applyOn;

    std::optional<std::size_t> m_maxInputPoints;

    ActionState m_actionState = ActionState::NotEntered;

    UserActionInputData m_uiInputData;
    UserActionInputData m_renderInputData;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_H
