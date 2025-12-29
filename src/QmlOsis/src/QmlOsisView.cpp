
#include "QmlOsis/QmlOsisView.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>

#include <OpenGl_GraphicDriver.hxx>
#ifdef _WIN32
#include <WNT_Window.hxx>
#elif (__APPLE__)
#include <Cocoa_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

#include <QOpenGLContext>

Aspect_VKeyFlags toOcc(Qt::KeyboardModifiers modifiers)
{
    Aspect_VKeyFlags occFlags{};
    if(modifiers.testFlag(Qt::KeyboardModifier::AltModifier))
    {
        occFlags |= Aspect_VKeyFlags_ALT;
    }
    if(modifiers.testFlag(Qt::KeyboardModifier::ControlModifier))
    {
        occFlags |= Aspect_VKeyFlags_CTRL;
    }
    if(modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier))
    {
        occFlags |= Aspect_VKeyFlags_SHIFT;
    }
    if(modifiers.testFlag(Qt::KeyboardModifier::MetaModifier))
    {
        occFlags |= Aspect_VKeyFlags_META;
    }

    return occFlags;
}

Aspect_VKeyMouse toOcc(Qt::MouseButtons buttons)
{
    Aspect_VKeyMouse occButtons{};
    if(buttons.testFlag(Qt::MouseButton::RightButton))
    {
        occButtons |= Aspect_VKeyMouse_RightButton;
    }
    if(buttons.testFlag(Qt::MouseButton::LeftButton))
    {
        occButtons |= Aspect_VKeyMouse_LeftButton;
    }
    if(buttons.testFlag(Qt::MouseButton::MiddleButton))
    {
        occButtons |= Aspect_VKeyMouse_MiddleButton;
    }

    return occButtons;
}

Aspect_VKey toOccVirtualKey(std::uint32_t nativeKey)
{
#ifdef _WIN32
    return WNT_Window::VirtualKeyFromNative(static_cast<int>(nativeKey));
#elif (__APPLE__)
    return Cocoa_Window::VirtualKeyFromNative(static_cast<int>(nativeKey));
#endif
}

Aspect_RenderingContext getNativeRenderingContext(QOpenGLContext *context)
{
#ifdef _WIN32
    return reinterpret_cast<Aspect_RenderingContext>(
        context->nativeInterface<QNativeInterface::QWGLContext>()->nativeContext());
#elif defined __APPLE__
    return reinterpret_cast<Aspect_RenderingContext>(
        context->nativeInterface<QNativeInterface::QCocoaGLContext>()->nativeContext());
#else
    return reinterpret_cast<Aspect_RenderingContext>(
        context->nativeInterface<QNativeInterface::QGLXContext>()->nativeContext());
#endif
}

Handle(Aspect_Window) makeWindow(Handle(Aspect_DisplayConnection) displayConnection, std::uintptr_t windowId)
{
#ifdef _WIN32
    return {new WNT_Window(Aspect_Drawable(windowId))};
#elif (__APPLE__)
    return {new Cocoa_Window(reinterpret_cast<NSView *>(windowId))};
#else
    return {new Xw_Window(displayConnection, Aspect_Drawable(windowId))};
#endif
}

namespace osis::qml
{

QmlOsisView::QmlOsisView(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    QObject::connect(this, &QQuickItem::windowChanged, this, &QmlOsisView::onWindowChanged, Qt::DirectConnection);
}

void QmlOsisView::onWindowChanged(QQuickWindow *window)
{
    if(!window)
        return;

    m_devicePixelRatio = window->effectiveDevicePixelRatio();
    updateOccDisplay(window);

    QObject::connect(window, &QQuickWindow::screenChanged, this,
                     [this, window]()
                     {
                         if(window->effectiveDevicePixelRatio() != m_devicePixelRatio)
                         {
                             m_devicePixelRatio = window->effectiveDevicePixelRatio();
                             updateOccDisplay(window);
                             update();
                         }
                     });

    QObject::connect(window, &QQuickWindow::beforeSynchronizing, this, &QmlOsisView::onSynchronizing,
                     Qt::DirectConnection);
    QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, this, &QmlOsisView::onInvalidating,
                     Qt::DirectConnection);
}

void QmlOsisView::hoverMoveEvent(QHoverEvent *event)
{
    if(const auto pos = toOccPoint(event);
       m_viewController.UpdateMousePosition(pos, toOcc(event->buttons()), toOcc(event->modifiers()), false))
    {
        window()->update();
    }
}

void QmlOsisView::mouseMoveEvent(QMouseEvent *event)
{
    if(const auto pos = toOccPoint(event);
       m_viewController.UpdateMousePosition(pos, toOcc(event->buttons()), toOcc(event->modifiers()), false))
    {
        window()->update();
    }
}

void QmlOsisView::mousePressEvent(QMouseEvent *event)
{
    if(const auto pos = toOccPoint(event);
       m_viewController.UpdateMouseButtons(pos, toOcc(event->buttons()), toOcc(event->modifiers()), false))
    {
        window()->update();
    }
}

void QmlOsisView::mouseReleaseEvent(QMouseEvent *event)
{
    if(const auto pos = toOccPoint(event);
       m_viewController.UpdateMouseButtons(pos, toOcc(event->buttons()), toOcc(event->modifiers()), false))
    {
        window()->update();
    }
}

void QmlOsisView::keyPressEvent(QKeyEvent *event)
{
    m_viewController.KeyDown(toOccVirtualKey(event->nativeVirtualKey()), event->timestamp(), 0.0);
    window()->update();
}

void QmlOsisView::keyReleaseEvent(QKeyEvent *event)
{
    m_viewController.KeyUp(toOccVirtualKey(event->nativeVirtualKey()), event->timestamp());
    window()->update();
}

void QmlOsisView::wheelEvent(QWheelEvent *event)
{
    const auto delta = std::min(event->angleDelta().x(), event->angleDelta().y()) < 0 ? -1.0 : 1.0;
    Aspect_ScrollDelta scrollDelta{NCollection_Vec2{-1, -1}, delta, toOcc(event->modifiers())};
    if(m_viewController.UpdateMouseScroll(scrollDelta))
    {
        window()->update();
    }
}

void QmlOsisView::onSynchronizing()
{
    if(!m_viewer)
    {
        initializeViewer(window()->winId());
        QObject::connect(this->window(), &QQuickWindow::beforeRendering, this,
                         &QmlOsisView::onBeforeRendering, Qt::DirectConnection);
        QObject::connect(this->window(), &QQuickWindow::beforeRenderPassRecording, this,
                         &QmlOsisView::onBeforeRenderPassRecording,
                         Qt::DirectConnection);
    }

    QPoint viewportPos = mapToGlobal(QPointF(0, 0)).toPoint();
    QSize viewportSize = size().toSize();

    if (viewportPos.x() != m_viewportPos.x() || viewportPos.y() != m_viewportPos.y())
    {
        m_view->MustBeResized();
    }

    if(viewportSize.width() != m_viewportSize.width() || viewportSize.height() != m_viewportSize.height())
    {
        m_view->MustBeResized();
        m_view->Invalidate();
    }

    m_viewportPos = viewportPos;
    m_viewportSize = viewportSize;

    m_viewController.FlushViewEvents(m_context, m_view);
}

void QmlOsisView::onInvalidating()
{
    m_view.Nullify();
    m_context.Nullify();
    m_viewer.Nullify();
}

void QmlOsisView::onBeforeRenderPassRecording()
{
    if(m_view.IsNull())
        return;

    // Redraw the view.
    // NOTE: MustBeResized is called here, due to a bug that causes the background to
    //       go white, when hovering a MenuBar (and possibly other top-level QML items).

    this->window()->beginExternalCommands();

    m_view->Redraw();
    m_view->MustBeResized();

    // End begin update screen
    this->window()->endExternalCommands();
    //
}

void QmlOsisView::initializeViewer(std::uintptr_t drawable)
{
    Q_ASSERT(m_viewer.IsNull());

    const auto context = QOpenGLContext::currentContext();
    if(!context)
    {
        return;
    }

    auto renderContext = getNativeRenderingContext(context);
    if(renderContext == nullptr)
    {
        return;
    }

    Handle(OpenGl_GraphicDriver) driver{new OpenGl_GraphicDriver(m_display, false)};
    driver->ChangeOptions().buffersNoSwap = true;

    m_viewer = {new V3d_Viewer(driver)};
    m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    // Create and setup interactivity context.
    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded, false);

    // Create and setup view.
    auto window = makeWindow(m_display, drawable);
    if(!window->IsMapped())
    {
        window->Map();
    }

    m_view = m_viewer->CreateView();

    m_view->SetImmediateUpdate(Standard_False);
    m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(renderContext));
    m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.1, V3d_ZBUFFER);

    // Create a demo scene.
    createDemoScene();
}

void QmlOsisView::createDemoScene()
{
    // Create a bisque cone at [0, 10, 0].
    gp_Ax2 axis;
    axis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape bisqueCone = BRepPrimAPI_MakeCone(axis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) bisqueConeShape = new AIS_Shape(bisqueCone);
    bisqueConeShape->SetColor(Quantity_NOC_BISQUE);

    // Create a chocolate cone at [8, 10, 0].
    axis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape chocoCone = BRepPrimAPI_MakeCone(axis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) chocoConeShape = new AIS_Shape(chocoCone);
    chocoConeShape->SetColor(Quantity_NOC_CHOCOLATE);

    m_context->Display(bisqueConeShape, Standard_True);
    m_context->Display(chocoConeShape, Standard_True);

    // Fit all into the view.
    m_view->FitAll();
}

void QmlOsisView::onBeforeRendering()
{
    if(!window())
    {
        return;
    }

    if (!m_view)
    {
        return;
    }

    m_viewController.HandleViewEvents(m_context, m_view);
}

Graphic3d_Vec2i QmlOsisView::toOccPoint(QSinglePointEvent *event) const
{
    const auto point = m_devicePixelRatio * event->position().toPoint();
    return Graphic3d_Vec2i{point.x(), point.y()};
}

void QmlOsisView::updateOccDisplay(QQuickWindow *window) {
    
#if !defined(_WIN32) && !defined(__APPLE__)
    const auto nativeInterface = window->screen()->nativeInterface<QNativeInterface::QX11Application>();
    if (!m_display)
    {
        m_display = {new Aspect_DisplayConnection((Aspect_XDisplay *)nativeInterface->display())};
    }
    else
    {
        m_display->Init((Aspect_XDisplay *)nativeInterface->display());
    }
#endif
}

} // namespace osis::qml
