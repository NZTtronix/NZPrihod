import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore

Rectangle {
    id: root
    property var customListItemNames
    property var itemWidth: 200
    property var itemHeight: 30
    property var fontsize: 16
    property var margins: 0
    property var marginsTop: 0
    property var textColor: "white"

    signal itemClicked(string itemName)

    color: "transparent"
    // anchors.left: parent.left
    // anchors.top: parent.top
    // anchors.bottom: parent.bottom
    anchors.topMargin: marginsTop
    anchors.fill: parent
    anchors.bottomMargin: 200

    ListView {
        id: listView
        ScrollIndicator.vertical: ScrollIndicator {
            parent: listview.parent
            anchors.top: listview.top
            anchors.left: listview.right
            anchors.bottom: listview.bottom
        }
        spacing: 1
        anchors.fill: parent
        anchors.bottom: parent.bottom
        model: customListItemNames
        delegate: Rectangle {
            id: delegateRect
            width: itemWidth
            height: itemHeight
            
            color: ListView.isCurrentItem ?  "#101019" : "#203030"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index;
                    console.log("templatePath ", templatePath);

                    root.itemClicked(modelData);
                }
            }
            Text {
                text: modelData
                anchors.fill: parent
                anchors.centerIn: parent
                leftPadding: 5
                font.pointSize: root.fontsize
                verticalAlignment: Text.AlignHCenter
                color: root.textColor
            }
        }
    }
    Component.onCompleted: {
        listView.currentIndex = -1;
    }
}
