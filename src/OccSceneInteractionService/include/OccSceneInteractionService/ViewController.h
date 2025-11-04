
#ifndef OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
#define OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H

#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>
#include <OccSceneInteractionService/UiRenderSyncObject.h>

#include <AIS_ViewController.hxx>
#include <Standard_Handle.hxx>

namespace osis
{

class ICameraListener;

class ViewController : public AIS_ViewController
{
  public:
    OSIS_API ViewController();

    /**
     * @brief Sets camera listener. Can be NULL, if no camera listener is needed. Is called from UI thread.
     * @param pCameraListener camera listener.
     */
    void setCameraListener(Handle(ICameraListener) pCameraListener);

  public: //! @name public overridden methods
    /**
     * @brief Overridden method of HandleViewEvents. Is called from Render thread.
     */
    void HandleViewEvents(const Handle(AIS_InteractiveContext) &pContext,
                          const Handle(V3d_View) &pView) override;

  protected: //! @name protected overridden methods
    /**
     * @brief Overridden method of flushBuffers. Is called at sync stage between UI and Render threads.
     */
    void flushBuffers(const Handle(AIS_InteractiveContext) &pContext,
                      const Handle(V3d_View) &pView) override;

    /**
     * @brief Overridden method of handlePanning. Is called from Render thread.
     */
    void handlePanning(const Handle(V3d_View) &view) override;

    /**
     * @brief Overridden method of handleZoom. Is called from Render thread.
     */
    void handleZoom(const Handle(V3d_View) &view, const Aspect_ScrollDelta &params, const gp_Pnt *point) override;

    /**
     * @brief Overridden method of handleOrbitRotation. Is called from Render thread.
     */
    void handleOrbitRotation(const Handle(V3d_View) &view, const gp_Pnt &point, bool toLockZUp) override;

    /**
     * @brief Overridden method of handleViewRotation. Is called from Render thread.
     */
    void handleViewRotation(const Handle(V3d_View) &view, double yawExtra, double pitchExtra, double roll,
                            bool toRestartOnIncrement) override;

  private:
    UiRenderSyncObject<Handle(ICameraListener)> m_pCameraListenerSyncObject;
    bool m_isAnimationInProgress = false;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
