

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick

GridView {
    // property alias gridWidth: width
    // property alias gridHeight: height
    width: 700
    height: 150
    
    cellWidth: 110
    cellHeight: 110

    children: [
        Rectangle {
            color: "#1d1d1d"
            anchors.fill: parent
            z: -1
        }
    ]

    model: MainPageToolBarModel {}

    delegate: MainPageToolBarDelegate {}
}
