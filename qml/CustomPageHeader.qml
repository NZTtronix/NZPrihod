import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Rectangle {
        id: headerRect
        height: 90
        signal backClicked();
        Column {

            anchors.fill: parent
            spacing: 2
            Button {
                id: backbutton
                icon.source: "Resources/icons/back.svg"
                icon.color: "white"
                display: Button.TextUnderIcon
                width: 50
                palette.buttonText: "white"
                icon.width: 48
                icon.height: 48
                height: width
                background: Rectangle {
                    opacity: backbutton.highlighted ? 0.1 : 0
                }
                onClicked: {
                    headerRect.backClicked(); 
                }
            }
            Rectangle {
                width: headerRect.width
                height: headerRect.height - backbutton.height
                anchors.left: backbutton.right
                anchors.verticalCenter: backbutton.verticalCenter
                anchors.right: parent.right
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