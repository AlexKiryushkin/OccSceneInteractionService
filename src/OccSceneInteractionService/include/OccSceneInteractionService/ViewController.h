
#ifndef OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
#define OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H

#include <OccSceneInteractionService/CustomMouseAction.h>
#include <OccSceneInteractionService/KeyboardListener.h>
#include <OccSceneInteractionService/OccSceneInteractionServiceExports.h>
#include <OccSceneInteractionService/UiRenderSyncObject.h>

#include <AIS_ViewController.hxx>
#include <Graphic3d_Vec2.hxx>
#include <Standard_Handle.hxx>

#include <map>
#include <optional>
#include <unordered_set>

class SelectMgr_Filter;

namespace osis
{

class ICameraListener;
class IKeyHandler;
class IMouseClickHandler;
class IMouseHoverListener;
class IOwnerHoverListener;

struct CameraEnvironment;
struct SceneInteractionEnvironment;
struct SelectionEnvironment;

/** @defgroup OccSceneInteractionService OpenCASCADE scene interaction service module
 * @brief OpenCASCADE scene interaction service module
 * @details This module provides flexible interfaces for solving the problem of accepting user input in UI thread and
 * processing it in Render thread. Typical use case is when we have a UI framework (QT) that has a dedicated thread for
 * rendering.
 *
 * Prerequisite knowledge:
 *
 * 1. It is built on top of AIS_ViewController class. For more information, see the link below.
 *
 * 2. Also, to understand what problem we are solving here, one should be aware of QML Threaded Render Loop (also see link
 * below.)
 *
 * For each method of each class in this module there should be a clear distinction where (or when) it should be called.
 * Possible options are: UI thread, Render thread, sync time between GUI and Render threads or any thread. The general
 * rule is:
 *
 * 1. Accumulate all user input UI thread
 *
 * 2. Pass accumulated input to render thread data structure at sync time
 *
 * 3. Process passed input in render thread by changing the graphics scene and/or view
 *
 * Generally, one should try to minimize operations at sync time as it blocks the UI thread and can cause performance
 * issues. That's why we have 3 stages instead of just 2 when we process passed input at sync time, as processing input
 * is much more expensive operation then passing the UI input to render data structures.
 *
 * To abstract the user of this module from implementation details of internal data synchronization and input processing
 * we provide a series of interfaces for interaction with the scene. The interfaces are:
 *
 * 1. ICameraListener - Helps when user wants to do an action on any camera event.
 *
 * 2. IKeyHandler - Handles key events in Render thread.
 *
 * 3. IMouseClickHandler - Adds additional action on mouse clicks.
 *
 * 4. IMouseHoverListener - Adds additional actions on mouse hover (mouse move without any buttons pressed).
 *
 * 5. IOwnerHoverListener - Adds additional actions when owner starts/stops being hovered.
 *
 * 6. ISelectionHandler - Provides opportunity to handle selection differently from default behavior.
 *
 * 7. CustomAction - Allows user to add additional custom actions on specific Mouse + Modifier combinations.
 *
 * The main class of this module is ViewController. It provides all necessary API for accumulating input in UI thread,
 * synchronizing UI and Render data, and process input in Render thread. It also provides API for setting all necessary
 * additional handlers/listeners mentioned above.
 *
 * @see
 * [Small AIS_ViewController article](https://unlimited3d.wordpress.com/2019/11/06/ais_viewcontroller-in-occt-7-4-0/)
 *
 * [QML Threaded Render Loop](https://doc.qt.io/qt-6/qtquick-visualcanvas-scenegraph.html#threaded-render-loop-threaded)
 */

/** @ingroup UserInteractionService
 * @brief This class is responsible for:
 * 1. Accumulating user input (mouse clicks, mouse movements, etc.) on UI side in GUI thread;
 * 2. Passing user input from UI to Render at sync between GUI and Render thread;
 * 3. Handling user input in Render thread.
 *
 * If one wants to modify the default behavior, they can set custom SceneInteractionEnvironment.
 */
class ViewController : public AIS_ViewController
{
  public: //! @name public methods
    OSIS_API ViewController() = default;

    /**
     * @brief Sets scene interaction environment which consists of camera environment, selection environment and custom
     * actions. Is called from UI thread.
     * @param sceneInteractionEnvironment Scene interaction environment.
     */
    void setSceneInteractionEnvironment(const SceneInteractionEnvironment &sceneInteractionEnvironment);

    /**
     * @brief Sets camera environment for the graphics scene. Is called from UI thread.
     * @param cameraEnvironment Camera environment.
     */
    void setCameraEnvironment(const CameraEnvironment &cameraEnvironment);

    /**
     * @brief Sets selection environment for the graphics scene. Is called from UI thread.
     * @param selectionEnvironment Selection environment.
     */
    void setSelectionEnvironment(const SelectionEnvironment &selectionEnvironment);

    /**
     * @brief Sets custom actions for the graphics scene. Is called from UI thread.
     * @param customMouseActions Custom actions.
     */
    void setCustomMouseActions(std::vector<Handle(CustomMouseAction)> customMouseActions);

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

    /**
     * @brief Sets selection filter. Can be NULL, if no selection filter needed. Is called from UI thread.
     * @param pSelectionFilter Selection filter.
     */
    void setSelectionFilter(Handle(SelectMgr_Filter) pSelectionFilter);

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
     * @brief Overridden method of UpdateMouseButtons. Is called from UI thread.
     */
    bool UpdateMouseButtons(const Graphic3d_Vec2i &point, Aspect_VKeyMouse buttons, Aspect_VKeyFlags modifiers,
                            bool isEmulated) override;

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

    
    /**
     * @brief Returns camera listener. Is called from Render thread.
     * @return Camera listener.
     */
    Handle(ICameraListener) getCameraListener() const { return m_pCameraListenerSyncObject.getRenderData(); }

    /**
     * @brief Returns owner hover listener. Is called from Render thread.
     * @return Owner hover listener.
     */
    Handle(IOwnerHoverListener) getOwnerHoverListener() const;

    /**
     * @brief Returns custom actions of the graphics scene. Is called from Render thread.
     * @return Custom actions of the graphics scene.
     */
    const std::vector<Handle(CustomMouseAction)> &getCustomActions() const;

    /**
     * @brief Returns key handler. Is called from Render thread.
     * @return Key handler.
     */
    Handle(IKeyHandler) getKeyHandler() const;

    /**
     * @brief Returns mouse click handler. Is called from Render thread.
     * @return Mouse click handler.
     */
    Handle(IMouseClickHandler) getMouseClickHandler() const;

    /**
     * @brief Returns mouse hover listener. Is called from Render thread.
     * @return Mouse hover listener.
     */
    Handle(IMouseHoverListener) getMouseHoverListener() const;

    /**
     * @brief Sets if rubber band selection is allowed. Is called from UI thread.
     * @param toAllow if true, rubber band selection is allowed.
     */
    void setAllowRubberBandSelection(bool toAllow);

    /**
     * @brief Returns if rubber band selection is allowed. Is called from UI thread.
     * @return true if rubber band selection is allowed.
     */
    bool toAllowRubberBandSelection() const;

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

  private: //! @name private methods
    /**
     * @brief Flushes actions input and/or setting new actions. Is called at sync stage between UI and Render threads.
     */
    void flushActions();

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
    std::unordered_set<unsigned> m_rubberBandModifiers;

    UiRenderSyncObject<Handle(ICameraListener)> m_pCameraListenerSyncObject;
    bool m_isAnimationInProgress = false;

    UiRenderSyncObject<Handle(IMouseClickHandler)> m_pMouseClickHandlerSyncObject;
    UiRenderSyncObject<MouseClickData> m_mouseClickDataSyncObject;

    UiRenderSyncObject<Handle(IOwnerHoverListener)> m_pOwnerHoverListenerSyncObject;

    UiRenderSyncObject<Handle(IMouseHoverListener)> m_pMouseHoverListenerSyncObject;
    UiRenderSyncObject<std::optional<Graphic3d_Vec2i>> m_mouseHoverPositionSyncObject;
    
    KeyboardListener m_keyboardListener;
    UiRenderSyncObject<Handle(IKeyHandler)> m_pKeyHandlerSyncObject;

    UiRenderSyncObject<std::vector<Handle(CustomMouseAction)>> m_customMouseActionsSyncObject;
    std::map<unsigned int, Handle(CustomMouseAction)> m_customActionValues;

    UiRenderSyncObject<Handle(SelectMgr_Filter)> m_pSelectionFilterSyncObject;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_VIEW_CONTROLLER_H
