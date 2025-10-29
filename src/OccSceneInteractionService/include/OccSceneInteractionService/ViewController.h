
#ifndef OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
#define OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H

#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>

#include <AIS_ViewController.hxx>

namespace osis
{

class ViewController : public AIS_ViewController
{
  public:
    OSIS_API ViewController();
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
