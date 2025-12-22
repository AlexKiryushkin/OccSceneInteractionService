
#ifndef OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_DATA_H
#define OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_DATA_H

#include <Graphic3d_Vec2.hxx>

#include <vector>

namespace osis
{

/** @ingroup UserInteractionService
 * @brief All necessary data from user input to perform an action.
 */
struct UserActionInputData
{
    /**
     * @brief When set to true, means that gather of input just started this frame. Some additional logic might be
     * performed on start.
     */
    bool toStart = false;

    /**
     * @brief When set to true, means the action should be applied for given input.
     */
    bool toApply = false;

    /**
     * @brief Input points.
     */
    std::vector<Graphic3d_Vec2i> inputPoints;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_USER_ACTION_INPUT_DATA_H
