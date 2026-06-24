import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Window 2.15
import FieldListModel 1.0

Item {
    id: root
    anchors.fill: parent
    visible: true

    property var fieldListModelID
    property var chosenLabelTemplate
    property var printLabelsPath

    function printCanvas() {
        console.log("printCanvas() called");

        canvas.grabToImage(function(result) {
                
                console.log("result is ", result );
        
        fieldListModelID.print(result); // см. C++ ниже
    });
}


    Column {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
        spacing: 10

        // Layout.alignment: Qt.AlignTop
        // Layout.fillHeight: false   // важно

        // Header + List (строго сверху, высота 150px)
        Column {
            spacing: 10

            // Layout.fillHeight: false

            // Layout.fillWidth: true
            // Layout.preferredHeight: 150 + 24 + 10 // грубо: заголовок + отступы; можно убрать/подобрать

            Text {
                text: "Field Items Table"
                color: palette.text
                font.pixelSize: 24
                font.bold: true
                // Layout.alignment: Qt.AlignTop
                // Layout.fillHeight: false
            }
            Rectangle {
                height: 15
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                color: "transparent"
                GridLayout {
                    columns: 4
                    // columnSpacing: 10
                    rowSpacing: 10
                    RowLayout {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 15

                        Text {
                            text: "X"
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 15
                            color: "white"
                        }

                        Text {
                            text: "Y"
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 15
                            color: "white"
                        }

                        Text {
                            text: "Ширина"
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 15
                            color: "white"
                        }

                        Text {
                            text: "Высота"
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 15
                            color: "white"
                        }

                        Text {
                            text: "Тип"
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 15
                            color: "white"
                        }

                        Text {
                            text: "Содержание"
                            Layout.preferredWidth: 200
                            Layout.preferredHeight: 15
                            color: "white"
                        }
                    }
                }
            }
            Rectangle {
                // Layout.fillWidth: true
                // Layout.preferredHeight: 150
                // Layout.fillHeight: false

                color: "transparent"
                border.color: "#cccccc"
                border.width: 1
                radius: 4
                width: 600
                height: 150

                ListView {
                    id: listView
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        bottom: parent.bottom
                        margins: 1
                    }
                    clip: true

                    property int sbW: 12

                    model: fieldListModelID
                    delegate: Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 25
                        color: ListView.isCurrentItem ? "#203030" : "#101019"
                        border.width: 1
                        border.color: "grey"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: listView.currentIndex = index
                        }

                        GridLayout {
                            columns: 4
                            // columnSpacing: 10
                            rowSpacing: 10
                            RowLayout {
                                Layout.preferredWidth: 50
                                Layout.preferredHeight: 25

                                TextField {
                                    text: X
                                    Layout.preferredWidth: 50
                                    Layout.preferredHeight: 25
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                    onEditingFinished: X = text
                                }

                                TextField {
                                    text: Y
                                    Layout.preferredWidth: 50
                                    Layout.preferredHeight: 25
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                    onEditingFinished: Y = text
                                }

                                TextField {
                                    text: model.width
                                    Layout.preferredWidth: 50
                                    Layout.preferredHeight: 25
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"

                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                    onEditingFinished: model.width = text
                                }

                                TextField {
                                    text: model.height
                                    Layout.preferredWidth: 50
                                    Layout.preferredHeight: 25
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                    onEditingFinished: model.height = text
                                }

                                ComboBox {
                                    id: codeTypeCombo
                                    Layout.preferredWidth: 150
                                    Layout.preferredHeight: 25
                                    property var codeTypeStrings: ["None", "QRCode", "Code39"]
                                    model: codeTypeStrings
                                    currentIndex: (codeType === "QRCode") ? 1 : (codeType === "Code39") ? 2 : 0
                                    onCurrentIndexChanged: {
                                        codeType = codeTypeStrings[currentIndex];
                                    }
                                    onActivated: codeType = codeTypeStrings[currentIndex]
                                    contentItem: Text {
                                        text: codeTypeCombo.currentText
                                        color: "white"
                                        verticalAlignment: Text.AlignVCenter
                                        elide: Text.ElideRight
                                    }
                                    background: Rectangle {
                                        color: "transparent"
                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                }

                                TextField {
                                    text: content
                                    Layout.preferredWidth: 200
                                    Layout.preferredHeight: 25
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        // border.width: 1
                                        // border.color: "grey"
                                    }
                                    onEditingFinished: content = text
                                }
                            }
                        }
                    }

                    // место под scrollbar справа
                    contentWidth: width - sbW
                    ScrollBar.vertical: ScrollBar {
                        width: listView.sbW
                        policy: ScrollBar.AlwaysOn
                        anchors {
                            top: listView.top
                            right: listView.right
                            bottom: listView.bottom
                        }
                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Add Row"
                Layout.preferredWidth: 150
                onClicked: {
                    fieldListModelID.addEmptyItem();

                    rowCounter.text = "Total rows: " + fieldListModelID.rowCount();
                }
                background: Rectangle {
                    color: "#101019"
                }
            }

            Button {
                text: "Remove Selected"
                Layout.preferredWidth: 150
                onClicked: {
                    fieldListModelID.deleteItem(listView.currentIndex);
                    rowCounter.text = "Total rows: " + fieldListModelID.rowCount();
                }
                background: Rectangle {
                    color: "#101019"
                }
            }

            Button {
                text: "Save"
                Layout.preferredWidth: 150
                onClicked: {
                    let res = fieldListModelID.saveToJson(root.printLabelsPath + root.chosenLabelTemplate + ".json");
                    console.log("Save clicked, saveToJson returned", res);
                }
                background: Rectangle {
                    color: "#101019"
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                id: rowCounter
                text: "Total rows: " + fieldListModelID.rowCount()
                font.pixelSize: 14
            }
        }

        Button {
            width: 200
            height: 25
            text: "Test print"
            onClicked: {
                console.log("Test print clicked");
                printCanvas();
            }
            background: Rectangle {
                color: "#101019"
            }
        }

        // Preview edit + Canvas (минимум места, выровнено по верхней границе)
        ColumnLayout {

            spacing: 10
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignTop

            Text {
                text: ""
                color: palette.text
                font.pixelSize: 24
                font.bold: true
                height: 25
                Layout.alignment: Qt.AlignTop
            }

            Rectangle {
                color: "transparent"
                width: 300
                Layout.preferredHeight: 25

                PrintSizeForm {
                    id: printSizeForm
                    height: 25
                }
                Layout.alignment: Qt.AlignTop
            }
            Rectangle {
                id: canvas
                Layout.fillWidth: true
                color: "white"
                border.color: "#ccc"
                border.width: 1
                radius: 4

                implicitWidth: printSizeForm.widthMM * pixelDensity
                implicitHeight: printSizeForm.heightMM * pixelDensity

                property var pixelDensity: Screen.pixelDensity
                property var canvasItems: []

                Repeater {
                    id: repeater
                    model: fieldListModelID
                    delegate: Item {
                        id: marker
                        property int modelRow: index
                        width: model.width
                        height: model.height
                        x: X
                        y: Y

                        Image {
                            id: svgImage
                            anchors.fill: parent
                            source: fieldListModelID.GenerateBarcode(modelRow, content, codeType)
                            fillMode: Image.PreserveAspectFit
                            smooth: false
                        }

                        Text {
                            width: parent.width
                            height: parent.height
                            text: codeType == "None" ? content : ""
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 10
                            font.pixelSize: parent.height
                        }

                        MouseArea {
                            anchors.fill: parent
                            drag.target: marker
                            drag.axis: Drag.XAndYAxis

                            onClicked: listView.currentIndex = modelRow
                            onReleased: fieldListModelID.updatePosition(modelRow, marker.x, marker.y)
                        }

                        onXChanged: {
                            if (x < 0)
                                x = 0;
                            if (x + width > canvas.width)
                                x = canvas.width - width;
                        }
                        onYChanged: {
                            if (y < 0)
                                y = 0;
                            if (y + height > canvas.height)
                                y = canvas.height - height;
                        }
                    }
                }
            }
        }
    }

    // Диалоги/нижние кнопки у тебя дальше — оставляй как есть,
    // но если хочешь чтобы всё было "сверху и минимум по высоте",
    // их лучше тоже перенести внутрь ColumnLayout и задать Layout.alignment: Qt.AlignTop.
    Dialog {
        id: clearDialog
        title: "Clear All Data"
        modal: true
        anchors.centerIn: parent
        width: 320

        contentItem: Text {
            text: "Are you sure you want to delete all rows?\nThis action cannot be undone."
            wrapMode: Text.WordWrap
            padding: 20
        }

        footer: RowLayout {
            spacing: 10
            anchors.margins: 10

            Button {
                text: "Cancel"
                Layout.fillWidth: true
                onClicked: clearDialog.close()
            }

            Button {
                text: "Delete All"
                Layout.fillWidth: true
                background: Rectangle {
                    color: "#f44336"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: clearDialog.close()
            }
        }
    }
}
