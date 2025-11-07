
#include "OccSceneInteractionService/ViewController.h"

#include "OccSceneInteractionService/ICameraListener.h"
#include "OccSceneInteractionService/IMouseClickHandler.h"

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

void ViewController::setMouseClickHandler(Handle(IMouseClickHandler) pMouseClickHandler)
{
    m_pMouseClickHandlerSyncObject.setUiData(std::move(pMouseClickHandler));
}

void ViewController::HandleViewEvents(const Handle(AIS_InteractiveContext) & pContext, const Handle(V3d_View) & pView)
{
    AIS_ViewController::HandleViewEvents(pContext, pView);

    if(myViewAnimation)
    {
        const auto isAnimationCurrentlyStopped = myViewAnimation->IsStopped();

        auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
        if(pCameraListener)
        {
            if(m_isAnimationInProgress && isAnimationCurrentlyStopped)
            {
                pCameraListener->onAnimationStopped();
            }

            if(!m_isAnimationInProgress && !isAnimationCurrentlyStopped)
            {
                pCameraListener->onAnimationStarted();
            }
        }

        m_isAnimationInProgress = !isAnimationCurrentlyStopped;
    }

    if(auto pMouseClickHandler = m_pMouseClickHandlerSyncObject.getRenderData();
       pMouseClickHandler && m_mouseClickDataSyncObject.getRenderData().isValid())
    {
        auto &&mouseClickData = m_mouseClickDataSyncObject.getRenderData();
        pMouseClickHandler->handleMouseClicked(mouseClickData.point, mouseClickData.button, mouseClickData.modifiers,
                                               mouseClickData.isDoubleClick);

        m_mouseClickDataSyncObject.resetRenderData();
    }
}

bool ViewController::UpdateMouseClick(const Graphic3d_Vec2i &point, Aspect_VKeyMouse button, Aspect_VKeyFlags modifiers,
                                      bool isDoubleClick)
{
    auto toUpdateView = AIS_ViewController::UpdateMouseClick(point, button, modifiers, isDoubleClick);

    if(auto pMouseClickHandler = m_pMouseClickHandlerSyncObject.getUiData();
       pMouseClickHandler && pMouseClickHandler->actsOn(button, modifiers, isDoubleClick))
    {
        m_mouseClickDataSyncObject.setUiData(MouseClickData{point, button, modifiers, isDoubleClick});
        toUpdateView = true;
    }

    return toUpdateView;
}

void ViewController::flushBuffers(const Handle(AIS_InteractiveContext) & pContext, const Handle(V3d_View) & pView)
{
    AIS_ViewController::flushBuffers(pContext, pView);
    m_pCameraListenerSyncObject.sync();

    m_pMouseClickHandlerSyncObject.sync();
    m_mouseClickDataSyncObject.sync();
}

void ViewController::handlePanning(const Handle(V3d_View) & view)
{
    AIS_ViewController::handlePanning(view);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowPanning && myGL.Panning.ToPan && pCameraListener)
    {
        pCameraListener->onCameraCenterChanged(view->Camera()->Center());
    }
}

void ViewController::handleZoom(const Handle(V3d_View) & view, const Aspect_ScrollDelta &params, const gp_Pnt *point)
{
    AIS_ViewController::handleZoom(view, params, point);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowZooming && pCameraListener)
    {
        pCameraListener->onCameraScaleChanged(params.Delta);
    }
}

void ViewController::handleOrbitRotation(const Handle(V3d_View) & view, const gp_Pnt &point, bool toLockZUp)
{

    AIS_ViewController::handleOrbitRotation(view, point, toLockZUp);

    auto pCameraListener = m_pCameraListenerSyncObject.getRenderData();
    if(myToAllowRotation && myGL.OrbitRotation.ToRotate && pCameraListener)
    {
        pCameraListener->onCameraRotation();
    }
}

void ViewController::handleViewRotation(const Handle(V3d_View) & view, double yawExtra, double pitchExtra, double roll,
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
