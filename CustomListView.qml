import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore

Rectangle {
    id: root
    property var customListItemNames
    property var itemWidth: 200 
    property var itemHeight : 30
    property var fontsize : 16
    property var margins: 0
    property var marginsTop: 0
    property var textColor : "white"

    signal itemClicked(string itemName)

    color: "transparent"
    // anchors.left: parent.left
    // anchors.top: parent.top
    // anchors.bottom: parent.bottom
    anchors.fill: parent
    anchors.margins: margins
    anchors.topMargin: marginsTop
    ListView {
       
        id: listView
         ScrollIndicator.vertical: ScrollIndicator {
            parent: listview.parent
            anchors.top: listview.top
            anchors.left: listview.right
            anchors.bottom: listview.bottom
        }
        anchors.fill: parent
        anchors.bottom: parent.bottom
        model: customListItemNames
        delegate: ItemDelegate {
            id: controlx
            width: itemWidth
            height: itemHeight
            // Text {
            //     text: modelData
            //     anchors.fill: parent
            //     leftPadding: 5
            //     font.pointSize: root.fontsize
            //     verticalAlignment: Text.AlignHCenter
            //     color: root.textColor
            // }
            text: modelData
            font.pointSize:root.fontsize
            highlighted: ListView.isCurrentItem
            // background: Rectangle {
                
            //     color:"black"
            //     opacity: 0.0
            // }
            onClicked: {
                listView.currentIndex = index;
                console.log("clicked:", modelData);
                root.itemClicked(modelData);
            }
        }



    }
    Component.onCompleted: {
        listView.currentIndex = -1;
    }
}
