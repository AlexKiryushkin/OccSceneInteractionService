
#ifndef OCC_SCENE_INTERACTION_SERVICE_SCENE_INTERACTION_ENVIRONMENT_H
#define OCC_SCENE_INTERACTION_SERVICE_SCENE_INTERACTION_ENVIRONMENT_H

#include <OccSceneInteractionService/CustomMouseAction.h>
#include <OccSceneInteractionService/ICameraListener.h>
#include <OccSceneInteractionService/IKeyHandler.h>
#include <OccSceneInteractionService/IMouseClickHandler.h>
#include <OccSceneInteractionService/IMouseHoverListener.h>
#include <OccSceneInteractionService/IOwnerHoverListener.h>

#include <SelectMgr_Filter.hxx>

namespace osis
{

/** @ingroup UserInteractionService
 * @brief A struct that contains all data that tells how selection should be handled in the scene.
 */
struct SelectionEnvironment
{
    bool allowRubberBandSelection = true;
    Handle(SelectMgr_Filter) pSelectionFilter;

    Handle(IOwnerHoverListener) pOwnerHoverListener;
};

/** @ingroup UserInteractionService
 * @brief A struct that contains all data that tells how camera should be handled in the scene.
 */
struct CameraEnvironment
{
    bool allowPan = true;
    bool allowRotation = true;
    bool allowZoom = true;

    Handle(ICameraListener) pCameraListener;
};

/** @ingroup UserInteractionService
 * @brief A full description of user interaction with scene.
 */
struct SceneInteractionEnvironment
{
    SelectionEnvironment selectionEnvironment;
    CameraEnvironment cameraEnvironment;

    Handle(IKeyHandler) pKeyHandler;
    Handle(IMouseClickHandler) pMouseClickHandler;
    Handle(IMouseHoverListener) pMouseHoverListener;
    std::vector<Handle(CustomMouseAction)> customMouseActions;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_SCENE_INTERACTION_ENVIRONMENT_H
