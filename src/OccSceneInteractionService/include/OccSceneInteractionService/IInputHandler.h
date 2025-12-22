
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_INPUT_HANDLER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_INPUT_HANDLER_H

#include <OccSceneInteractionService/UserActionInputData.h>

#include <Standard_Handle.hxx>

namespace osis
{

/** @ingroup UserInteractionService
 * @brief Performs custom logic in graphics scene for given user input
 */
class IInputHandler : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(IInputHandler, Standard_Transient)

  public:
    IInputHandler() = default;

    IInputHandler(const IInputHandler &) = default;
    IInputHandler(IInputHandler &&) = default;

    IInputHandler &operator=(const IInputHandler &) = default;
    IInputHandler &operator=(IInputHandler &&) = default;

    ~IInputHandler() override = default;

    /**
     * @brief Performs custom logic with user input. Is called from Render thread.
     * @param userInput Input with which the custom logic is performed.
     */
    virtual void handle(const UserActionInputData &userInput) = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_INPUT_HANDLER_H
