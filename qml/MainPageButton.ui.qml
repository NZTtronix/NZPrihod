import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl

Rectangle {
    property alias iconSource: colorImage.source
    property alias imageSideLength: colorImage.width
    property alias imageColor: colorImage.color
    property alias textlabel: mytext.text
    property alias marea: mouseArea
    property var rectangleWidthHeight: 80

    width: rectangleWidthHeight
    height: rectangleWidthHeight
    color:"transparent"
    border.width:1
    border.color: "transparent"

        Rectangle {
            color: "transparent"
            height: 0.6*rectangleWidthHeight
            id: rect1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            ColorImage {
                anchors.centerIn: parent
                id: colorImage
                width: 32
                height: width
                source: ""
                color: '#ffffff'
            }
        }
        Rectangle {
            color: "transparent"
            height: 0.3*rectangleWidthHeight
            anchors.top: rect1.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                id: mytext
                anchors.centerIn: parent
                text: ""
                font.pixelSize: 14
                width: parent.width
                height: parent.height
                color: colorImage.color
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter  
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill:parent
            
        }
}

