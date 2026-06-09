import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore

Rectangle {
    property var barcodeAnalyzerId
    property var stackViewId
    property var menuItemNames: ["Main", "rules", "templates", "parsing", "keywords", "settings", "print", "history", "About"]
    property var menuIconSize: 40
    property var menuWidth: 80
    property var menuItemHeight

    property var iconpaths: ["Resources/icons/home.svg", "Resources/icons/rule.svg", "Resources/icons/label.svg", "Resources/icons/print.svg", 
        // "Resources/icons/templates.svg",
        //         "Resources/icons/parsing.svg",
        //         "Resources/icons/keywords.svg",
        //         "Resources/icons/print",

        "Resources/icons/history", "Resources/icons/settings.svg", "Resources/icons/info.svg"]
    id: root

    signal itemClicked(string itemName)
    height: parent.height
    width: menuWidth

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.leftMargin: 0
    anchors.topMargin: 0
    anchors.bottomMargin: 0

    color: "transparent"
    Component.onCompleted: listView.currentIndex = -1
    ListView {
        id: listView
        visible: true

        anchors.fill: parent
        anchors.topMargin: 50
        spacing: 2
        model: menuItemNames
        delegate: ItemDelegate {
            id: control
            icon.source: root.iconpaths[index]
            icon.color: control.highlighted ? "white" : "white"
            display: ItemDelegate.IconOnly
            width: root.width
            palette.windowText: "white"
            icon.width: menuIconSize
            icon.height: menuIconSize
            height: root.menuItemHeight
            text: modelData

            highlighted: ListView.isCurrentItem
            background: Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                height: 0.6 * parent.height
                width: 5
                color: '#7fffff00'
                opacity: highlighted ? 0.5: 0

                Rectangle {
                    height:   root.menuItemHeight
                                    anchors.verticalCenter: parent.verticalCenter
                    radius: 10
                    width: root.menuWidth
                    color: white
                    opacity: highlighted ? 0.1: 0
                }
            }

            onClicked: {
                listView.currentIndex = index;
                console.log("clicked:", modelData);
                root.itemClicked(modelData);
            }
        }
    }
    Button {
        id: button
        icon.source: root.iconpaths[root.iconpaths.length-1]
        icon.width: menuIconSize
        icon.height: menuIconSize
                    display: ItemDelegate.IconOnly
                    width: root.width

        height: root.menuItemHeight
        anchors.bottom: root.bottom
        icon.color: "white"
        
        background: Rectangle {
             height: 0.75 * parent.height
             anchors.verticalCenter: parent.verticalCenter

             width: 5
                color: "yellow"
                opacity: active? 0 : 0
            }
     
        
    }
}
