#ifndef OCCTVIEW_H
#define OCCTVIEW_H

#include <OccSceneInteractionService/ViewController.h>


#include <QMutex.h>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow.h>

#include <AIS_InteractiveContext.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

namespace osis::qml
{

class QmlOsisView : public QQuickItem
{
    Q_OBJECT

  public:
    explicit QmlOsisView(QQuickItem *parent = nullptr);

    void hoverMoveEvent(QHoverEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

  public slots:
    void onSynchronizing();
    void onInvalidating();
    void onBeforeRendering();
    void onBeforeRenderPassRecording();

  private slots:
    void onWindowChanged(QQuickWindow *window);

  protected:
    void initializeViewer(std::uintptr_t drawable);

    // Private interface.
  private:
    void createDemoScene();

    void updateOccDisplay(QQuickWindow *window);
    Graphic3d_Vec2i toOccPoint(QSinglePointEvent *event) const;

  private:
    Handle(Aspect_DisplayConnection) m_display;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_context;

    ViewController m_viewController;

    qreal m_devicePixelRatio;
    QSize m_viewportSize;
    QPoint m_viewportPos;
};

} // namespace osis::qml

#endif // OCCTVIEW_H
