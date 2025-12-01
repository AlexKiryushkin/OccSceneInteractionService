
#ifndef OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
#define OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H

#include <OccSceneInteractionService/KeyboardListener.h>
#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>
#include <OccSceneInteractionService/UiRenderSyncObject.h>

#include <AIS_ViewController.hxx>
#include <Graphic3d_Vec2.hxx>
#include <Standard_Handle.hxx>

#include <optional>

namespace osis
{

class ICameraListener;
class IKeyHandler;
class IMouseClickHandler;
class IMouseHoverListener;
class IOwnerHoverListener;

class ViewController : public AIS_ViewController
{
  public: //! @name public methods
    OSIS_API ViewController() = default;

    /**
     * @brief Sets camera listener. Can be NULL, if no camera listener is needed. Is called from UI thread.
     * @param pCameraListener camera listener.
     */
    void setCameraListener(Handle(ICameraListener) pCameraListener);

    /**
     * @brief Sets mouse click handler. Can be NULL, if no mouse click handler is needed. Is called from UI thread.
     * @param pMouseClickHandler mouse click handler.
     */
    void setMouseClickHandler(Handle(IMouseClickHandler) pMouseClickHandler);

    /**
     * @brief Sets owner hover listener. Can be NULL, if no owner hover listener is needed. Is called from UI thread.
     * @param pOwnerHoverListener owner hover listener.
     */
    void setOwnerHoverListener(Handle(IOwnerHoverListener) pOwnerHoverListener);

    /**
     * @brief Sets owner hover listener. Can be NULL, if no owner hover listener is needed. Is called from UI thread.
     * @param pOwnerHoverListener owner hover listener.
     */
    void setMouseHoverListener(Handle(IMouseHoverListener) pMouseHoverListener);

    /**
     * @brief Sets key handler. Can be NULL, if no key handler is needed. Is called from UI thread.
     * @param pKeyHandler key handler.
     */
    void setKeyHandler(Handle(IKeyHandler) pKeyHandler);

  public: //! @name public overridden methods
    /**
     * @brief Overridden method of HandleViewEvents. Is called from Render thread.
     */
    void HandleViewEvents(const Handle(AIS_InteractiveContext) & pContext, const Handle(V3d_View) & pView) override;
    
    /**
     * @brief Overridden method of KeyDown. Is called from UI thread.
     */
    void KeyDown(Aspect_VKey key, double time, double pressure = 1.0) override;

    /**
     * @brief Overridden method of KeyUp. Is called from UI thread.
     */
    void KeyUp(Aspect_VKey key, double time) override;

    /**
     * @brief Overridden method of UpdateMouseClick. Is called from UI thread.
     */
    bool UpdateMouseClick(const Graphic3d_Vec2i &point, Aspect_VKeyMouse button, Aspect_VKeyFlags modifiers,
                          bool isDoubleClick) override;

    /**
     * @brief Overridden method of UpdateMousePosition. Is called from UI thread.
     */
    bool UpdateMousePosition(const Graphic3d_Vec2i &point, Aspect_VKeyMouse buttons, Aspect_VKeyFlags modifiers,
                             bool isEmulated) override;

  protected: //! @name protected overridden methods
    /**
     * @brief Overridden method of flushBuffers. Is called at sync stage between UI and Render threads.
     */
    void flushBuffers(const Handle(AIS_InteractiveContext) & pContext, const Handle(V3d_View) & pView) override;

    /**
     * @brief Overridden method of handlePanning. Is called from Render thread.
     */
    void handlePanning(const Handle(V3d_View) & view) override;

    /**
     * @brief Overridden method of handleZoom. Is called from Render thread.
     */
    void handleZoom(const Handle(V3d_View) & view, const Aspect_ScrollDelta &params, const gp_Pnt *point) override;

    /**
     * @brief Overridden method of handleOrbitRotation. Is called from Render thread.
     */
    void handleOrbitRotation(const Handle(V3d_View) & view, const gp_Pnt &point, bool toLockZUp) override;

    /**
     * @brief Overridden method of handleViewRotation. Is called from Render thread.
     */
    void handleViewRotation(const Handle(V3d_View) & view, double yawExtra, double pitchExtra, double roll,
                            bool toRestartOnIncrement) override;

    /**
     * @brief Overridden method of contextLazyMoveTo. Is called from Render thread.
     */
    void contextLazyMoveTo(const Handle(AIS_InteractiveContext) &context, const Handle(V3d_View) &view,
                           const Graphic3d_Vec2i &thePnt) override;

  private:
    struct MouseClickData
    {
        [[nodiscard]] bool isValid() const { return button != Aspect_VKeyMouse_NONE; }

        bool operator==(const MouseClickData &rhs) const
        {
            return std::tie(point, button, modifiers, isDoubleClick) ==
                   std::tie(rhs.point, rhs.button, rhs.modifiers, rhs.isDoubleClick);
        }

        Graphic3d_Vec2i point;
        Aspect_VKeyMouse button{};
        Aspect_VKeyFlags modifiers{};
        bool isDoubleClick{};
    };

  private:
    UiRenderSyncObject<Handle(ICameraListener)> m_pCameraListenerSyncObject;
    bool m_isAnimationInProgress = false;

    UiRenderSyncObject<Handle(IMouseClickHandler)> m_pMouseClickHandlerSyncObject;
    UiRenderSyncObject<MouseClickData> m_mouseClickDataSyncObject;

    UiRenderSyncObject<Handle(IOwnerHoverListener)> m_pOwnerHoverListenerSyncObject;

    UiRenderSyncObject<Handle(IMouseHoverListener)> m_pMouseHoverListenerSyncObject;
    UiRenderSyncObject<std::optional<Graphic3d_Vec2i>> m_mouseHoverPositionSyncObject;
    
    KeyboardListener m_keyboardListener;
    UiRenderSyncObject<Handle(IKeyHandler)> m_pKeyHandlerSyncObject;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
