import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import OpenCascade 7.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
	title: "OpenCascade Qt Viewer Demo"

	QmlOsisView {
		anchors.fill: parent
		focus: true
	}
}
