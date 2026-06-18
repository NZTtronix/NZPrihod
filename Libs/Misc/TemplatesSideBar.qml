import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore

Rectangle {
    id: root
    property bool hasAddNewItem: true
    property bool hasDeleteItem: true
    property bool hasGoBackItem: true
    property var headerText: "Templates"
    property var barcodeAnalyzerId
    property var stackViewId
    property var menuItemNames: ["murata", "yageo", "samsung", "everlight"]
    signal itemClicked(string itemName)
    signal backClicked

    width: 225
    color: "transparent"
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.leftMargin: 0
    anchors.topMargin: 0
    anchors.bottomMargin: 0

    ListView {
        id: listView

        anchors.fill: parent
        anchors.leftMargin: 10
        header: Rectangle {
            id: headerRect
            height: 90


            Column {
                anchors.fill: parent
                spacing: 2
                Button {
                    id: control
                    icon.source: "Resources/icons/back.svg"
                    icon.color: control.highlighted ? "white" : "white"
                    display: ItemDelegate.TextUnderIcon
                    width: 30
                    palette.buttonText: "white"
                    icon.width: 24
                    icon.height: 24
                    height: width
                    // text: "back"

                    highlighted: ListView.isCurrentItem
                    background: Rectangle {
                        color: control.highlighted ? "#306060" : "transparent"
                        border.width: 1
                        border.color: control.highlighted ? "white" : "transparent"
                        radius: 40
                    }
                    onClicked: {
                        root.backClicked();
                    }
                }
                Rectangle {
                    width: headerRect.width
                    height: headerRect.height - control.height
                    anchors.left: parent.left
                    anchors.top:control.top
                    anchors.bottom:parent.bottom
                    anchors.right:parent.right
                    
                    color: "transparent"
                    Text {
                        
                        font.bold: true
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        color: "white"
                        fontSizeMode: Text.VerticalFit
                        minimumPixelSize: 10
                        font.pixelSize: 30
                        text: root.headerText
                        wrapMode: Text.WordWrap
                    }
                }
            }
            width: parent.width
            color: "transparent"
        }
        anchors.bottom: parent.bottom
        model: menuItemNames
        spacing: -1
        delegate: ItemDelegate {
            id: controlx
            width: root.width - 5
            height: 40
            Text {
                text: modelData
                anchors.fill: parent
                anchors.leftMargin: 40
                font.pointSize: 16
                verticalAlignment: Text.AlignHCenter
                color: "white"
            }

            highlighted: ListView.isCurrentItem
            background: Rectangle {
                border.width: 1
                
                border.color: controlx.highlighted ? "grey":"#202040"
                color: controlx.highlighted ? "#306060" : "transparent"
            }
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
