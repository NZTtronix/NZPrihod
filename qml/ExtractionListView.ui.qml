

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick

ListView {
    property  var modelid;
    id: view
    width: 250
    height: 300

    highlightMoveDuration: 0

    children: [
        Rectangle {
            color: "#1d1d1d"
            anchors.fill: parent
            z: -1
        }
    ]
     model: modelid;
    

    delegate: ExtractionListViewDelegate {}
}
