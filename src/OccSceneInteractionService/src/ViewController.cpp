
#include "OccSceneInteractionService/ViewController.h"

#include "OccSceneInteractionService/ICameraListener.h"

#include <AIS_AnimationCamera.hxx>
#include <V3d_View.hxx>

namespace osis
{

ViewController::ViewController()
    : AIS_ViewController{}
{
}

void ViewController::setCameraListener(Handle(ICameraListener) pCameraListener)
{
    m_pCameraListenerSyncObject.setUiData(std::move(pCameraListener));
}

void ViewController::HandleViewEvents(const Handle(AIS_InteractiveContext) & pContext, const Handle(V3d_View) & pView)
{
    const auto wasAnimationStopped = myViewAnimation && myViewAnimation->IsStopped();
    AIS_ViewController::HandleViewEvents(pContext, pView);

    if (myViewAnimation)
    {
        const auto isAnimationStopped = myViewAnimation->IsStopped();

        auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
        if(!wasAnimationStopped && isAnimationStopped)
        {
            pCameraListener->onAnimationStopped();
        }

        if(wasAnimationStopped && !isAnimationStopped)
        {
            pCameraListener->onAnimationStarted();
        }
    }
}

void ViewController::flushBuffers(const Handle(AIS_InteractiveContext) &pContext, const Handle(V3d_View) &pView)
{
    m_pCameraListenerSyncObject.sync();
}

void ViewController::handlePanning(const Handle(V3d_View) &view)
{
    AIS_ViewController::handlePanning(view);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowPanning && myGL.Panning.ToPan && pCameraListener)
    {
        pCameraListener->onCameraCenterChanged(view->Camera()->Center());
    }
}

void ViewController::handleZoom(const Handle(V3d_View) &view, const Aspect_ScrollDelta &params, const gp_Pnt *point)
{
    AIS_ViewController::handleZoom(view, params, point);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowZooming && pCameraListener)
    {
        pCameraListener->onCameraScaleChanged(params.Delta);
    }
}

void ViewController::handleOrbitRotation(const Handle(V3d_View) &view, const gp_Pnt &point, bool toLockZUp) {

    AIS_ViewController::handleOrbitRotation(view, point, toLockZUp);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowRotation && myGL.OrbitRotation.ToRotate && pCameraListener)
    {
        pCameraListener->onCameraRotation();
    }
}

void ViewController::handleViewRotation(const Handle(V3d_View) &view, double yawExtra, double pitchExtra, double roll,
                                        bool toRestartOnIncrement)
{
    AIS_ViewController::handleViewRotation(view, yawExtra, pitchExtra, roll, toRestartOnIncrement);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowRotation && myGL.ViewRotation.ToRotate && pCameraListener)
    {
        pCameraListener->onCameraRotation();
    }
}

} // namespace osis
