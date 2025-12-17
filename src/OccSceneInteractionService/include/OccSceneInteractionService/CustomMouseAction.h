
#ifndef USER_INTERACTION_SERVICE_CUSTOM_MOUSE_ACTION_H
#define USER_INTERACTION_SERVICE_CUSTOM_MOUSE_ACTION_H

#include <OccSceneInteractionService/IInputHandler.h>
#include <OccSceneInteractionService/UserActionInput.h>
#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>

#include <Graphic3d_Vec2.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>

#include <memory>
#include <vector>

namespace osis
{

/**
 * @brief This is an abstract class for different actions. By action we mean some graphics scene action that starts
 * for some particular mouse combination (mouse + modifier) and ends when this combination changes to the one action
 * does not accept. This class provides interface for both handling it in UI and Render threads, and at sync between
 * UI and Render thread.
 */
class OSIS_API CustomMouseAction : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(CustomMouseAction, Standard_Transient)

  public:
    /**
     * @brief Initializes class.
     * @param userInput Class that accumulates user input data.
     * @param mouseCombinations Mouse combinations that activates the action.
     * @param pInputHandler Handler that performs the logic with user input.
     */
    CustomMouseAction(UserActionInput userInput, std::vector<unsigned int> mouseCombinations,
                      Handle(IInputHandler) pInputHandler)
        : m_userInput{std::move(userInput)}
        , m_mouseCombinations{std::move(mouseCombinations)}
        , m_pInputHandler{std::move(pInputHandler)}
    {
    }

    CustomMouseAction(const CustomMouseAction &) = default;
    CustomMouseAction(CustomMouseAction &&) noexcept = default;

    CustomMouseAction &operator=(const CustomMouseAction &) = default;
    CustomMouseAction &operator=(CustomMouseAction &&) noexcept = default;

    ~CustomMouseAction() override = default;

    /**
     * @brief A combination of Mouse + Modifiers that activates a custom gesture. This method should be called in GUI
     * thread. Multiple combinations are provided for cases when user can press certain modifiers while doing an action.
     * For example: left mouse click activates the gesture, but user can press SHIFT to change its behavior slightly. In
     * case we will add both left mouse button and SHIFT + left mouse button to mouse combinations, so that action
     * continues after user presses SHIFT.
     * @note For more information see Aspect_VKeyMouse_* and Aspect_VKeyFlags_*.
     * @return a combination of Mouse + Modifiers that activates a custom gesture.
     */
    const std::vector<unsigned int> &getMouseCombinations() const { return m_mouseCombinations; }

    /**
     * @brief Returns user input data gathered in UI thread. Should be called in UI thread.
     * @return User input data gathered in UI thread.
     */
    const UserActionInputData &getUiUserInputData() const { return m_userInput.getUiUserInputData(); }

    /**
     * @brief Returns user input data in Render thread that was synced from UI thread. Should be called in Render thread
     * @return User input data in Render thread that was synced from UI thread.
     */
    const UserActionInputData &getRenderUserInputData() const { return m_userInput.getRenderUserInputData(); }

    /**
     * @brief This method should called once before any other action methods. This method should be called from render thread.
     */
    virtual void setUp() {}

    /**
     * @brief This method should called once after all action methods are called. This method should be called from render thread.
     */
    virtual void tearDown() {}

    /**
     * @brief This method is called once the user mouse input matches \a getMouseCombinations value. This method should
     * be called in GUI thread.
     * @param point Mouse position at which action was started.
     */
    void startAction(const Graphic3d_Vec2i &point) { m_userInput.startGatherInput(point); }

    /**
     * @brief This method is called every time the user mouse input matches \a getMouseCombinations value and mouse
     * position is changed. This method should be called in GUI thread.
     * @param point Current mouse position.
     */
    void continueAction(const Graphic3d_Vec2i &point) { m_userInput.continueGatherInput(point); }

    /**
     * @brief This method is called once the user mouse input does not match \a getMouseCombinations value anymore. This
     * method should be called in GUI thread.
     * @param point Mouse position at which action was stopped.
     */
    void stopAction(const Graphic3d_Vec2i &point) { m_userInput.stopGatherInput(point); }

    /**
     * @brief Synchronization between GUI user input data and Render data. Should be called at sync time between GUI and
     * Render threads.
     */
    void sync() { m_userInput.sync(); }

    /**
     * @brief Handles user input by performing actions with graphics scene. This method should be called in Render
     * thread.
     */
    void handle() { m_pInputHandler->handle(m_userInput.getRenderUserInputData()); }

    /**
     * @brief Clears all UI input. This method should be called in GUI thread.
     */
    void clearUiInput() {m_userInput.resetUiInput(); }

    /**
     * @brief Clears all Render input. Should be called at sync time between GUI and Render threads.
     */
    void clearRenderInput() { m_userInput.resetRenderInput(); }

  private:
    UserActionInput m_userInput;
    std::vector<unsigned int> m_mouseCombinations;

    Handle(IInputHandler) m_pInputHandler;
};

} // namespace osis

#endif // USER_INTERACTION_SERVICE_CUSTOM_MOUSE_ACTION_H
