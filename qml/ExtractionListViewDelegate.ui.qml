import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: delegate
    width: ListView.view.width
    height: 50
    implicitHeight: 50
    implicitWidth: 200
    Rectangle {
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#cc4d4700"
            }

            GradientStop {
                position: 1
                color: "#ae000000"
            }
            orientation: Gradient.Vertical
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: delegate.GridView.view.currentIndex = index
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            id: row1
            color: "#00ffffff"

            Layout.preferredHeight: 20

            Layout.fillWidth: true

            Text {
                color: "white"
                id: textl
                x: 8
                y: 4
                text: model.name
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            id: row2
            color: "#1df5f10e"
            radius: 6
            border.color: "#534c00"
            anchors.top: row1.bottom
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Layout.minimumHeight: 20
            Layout.alignment: top

            TextEdit {
                text: model.valueRole
                id: textEdit
                color: "yellow"
                opacity: 1
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.bottomMargin: 0
                font.pixelSize: 12
                selectionColor: "#00000080"
            }
        }
    }
    states: [
        State {
            name: "Highlighted"

            when: delegate.GridView.isCurrentItem
        }
    ]
}
