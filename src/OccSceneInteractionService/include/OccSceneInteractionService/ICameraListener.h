
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_CAMERA_LISTENER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_CAMERA_LISTENER_H

#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>
#include <gp_Pnt.hxx>

namespace osis
{

/**
 * @brief An interface for additional actions on camera events.
 * @details This class provides a series of callbacks on camera events such as: panning, rotating, zooming, starting
 * and stopping of animation, etc. This might be useful if we want to recalculate some values on camera changes or block
 * user input while camera is animating.
 */
class ICameraListener : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(ICameraListener, Standard_Transient)

  public:
    ICameraListener() = default;

    ICameraListener(const ICameraListener &) = default;
    ICameraListener(ICameraListener &&) = default;

    ICameraListener &operator=(const ICameraListener &) = default;
    ICameraListener &operator=(ICameraListener &&) = default;

    ~ICameraListener() override = default;

    /**
     * @brief Is called when camera animation started. Camera animation means changing camera position within each
     * frame. Is called from Render thread.
     */
    virtual void onAnimationStarted() = 0;

    /**
     * @brief Is called when camera animation stopped.
     */
    virtual void onAnimationStopped() = 0;

    /**
     * @brief This callback is called when camera scale was changed. Is called from Render thread.
     * @param delta Delta of scale. Positive means zoom in, negative - zoom out.
     */
    virtual void onCameraScaleChanged(double delta) = 0;

    /**
     * @brief This callback is called when camera center was changed. Is called from Render thread.
     * @param newCenter New center of a camera.
     */
    virtual void onCameraCenterChanged(gp_Pnt newCenter) = 0;

    /**
     * @brief This callback is called when camera performs rotation. Is called from Render thread.
     */
    virtual void onCameraRotation() = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_CAMERA_LISTENER_H
