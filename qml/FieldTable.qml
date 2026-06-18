import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Window 2.15
// import FieldListModel 1.0
Window {
    id: root
    width: 1500
    height: 900
    visible: true
    title: "Field Table Model"

       



    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            text: "Field Items Table"
            font.pixelSize: 24
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            Rectangle {
                Layout.preferredWidth: 700
                Layout.fillHeight: true
                color: "#f5f5f5"
                border.color: "#cccccc"
                border.width: 1
                radius: 4

                ListView {
                    id: listView
                    anchors.fill: parent
                    anchors.margins: 1

                    model: fieldListModel
                    delegate: Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 25
                        color: ListView.isCurrentItem ? "#203030" : "#101019"
                        border.width: 1
                        border.color: "grey"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                listView.currentIndex = index;
                            }
                        }
                        GridLayout {
                            columns: 4
                            columnSpacing: 10
                            rowSpacing: 10
                            RowLayout {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                TextField {
                                    text: X
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: X = text
                                }
                                TextField {
                                    text: Y
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: Y = text
                                }
                                TextField {
                                    text: model.width
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: model.width = text
                                }
                                TextField {
                                    text: model.height
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: model.height = text
                                }
                                TextField {
                                    text: codeType
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: codeType = text
                                }
                                TextField {
                                    text: content
                                    Layout.preferredWidth: 100
                                    Layout.preferredHeight: 20
                                    color: "white"
                                    background: Rectangle {
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "grey"
                                    }
                                    onEditingFinished: content = text
                                }
                            }
                        }
                    }
                }
            }
            



    Rectangle {
        property var pixelDensity: Screen.pixelDensity

        PrintSizeForm{
            id: printSizeForm
            
        }
        
        id: canvas
        implicitWidth: printSizeForm.widthMM* pixelDensity
        implicitHeight: printSizeForm.heightMM * pixelDensity
        property var canvasItems: []
        color: "white"
        border.color: "#ccc"
        border.width: 1
        radius: 4

        Repeater {
            id: repeater
            model: fieldListModel
            delegate: Item {
                id: marker
                property int modelRow: index
                width: model.width    // роли width,height
                height: model.height
                x: model.X
                y: model.Y
                
                // отобразить SVG/контент
                Image {
                        anchors.fill: parent
                        id: svgImage
                        source: fieldListModel.GenerateBarcode(modelRow, model.content, model.codetype);   // либо сформируйте путь по codeType
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                }
                

                // Rectangle {
                //     anchors.fill: parent
                //     border.width: 1
                //     border.color: "green"
                //     color: (model.index.row() == modelRow)?'#c41dec8f': '#b75d8171';
                //     Label {
                //         text: model.content
                //         anchors.centerIn:parent

                //     }
                // }

                MouseArea {
                    anchors.fill: parent
                    drag.target: marker
                    drag.axis: Drag.XAndYAxis

                    onClicked: {
                        listView.currentIndex = modelRow;
                    }

                    onPositionChanged: {
                    // при текучем перемещении можно обновлять визуал без записи в модель
                    }

                    onReleased: {
                    // Обновляем модель через удобный инвокейбл
                    fieldListModel.updatePosition(modelRow, marker.x, marker.y)
                    }
                }

            // Ограничение движения внутри canvas
            onXChanged: { if (x < 0) x = 0; if (x + width > canvas.width) x = canvas.width - width }
            onYChanged: { if (y < 0) y = 0; if (y + height > canvas.height) y = canvas.height - height }

            
            }
        }
    }
            
    }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "➕ Add Row"
                Layout.preferredWidth: 150
                onClicked: {
                    fieldListModel.addEmptyItem();

                    rowCounter.text = "Total rows: " + fieldListModel.rowCount();
                }
            }

            Button {
                text: "🗑️ Remove Selected"
                Layout.preferredWidth: 200
                onClicked: {
                    fieldListModel.deleteItem(listView.currentIndex);
                    rowCounter.text = "Total rows: " + fieldListModel.rowCount();
                }
            }

            Button {
                text: "🧹 Clear All"
                Layout.preferredWidth: 150
                onClicked: clearDialog.open()
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                id: rowCounter
                text: "Total rows: " + fieldListModel.rowCount()
                font.pixelSize: 14
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 80
            color: "#e3f2fd"
            border.color: "#2196F3"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                Text {
                    text: "📊 Table Information"
                    font.pixelSize: 14
                    font.bold: true
                    color: "#1976D2"
                }

                RowLayout {
                    spacing: 30

                    Text {
                        text: "Selected Row: " + (listView.currentIndex + 1)
                        font.pixelSize: 12
                    }
                }
            }
        }
    }

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
                onClicked: {
                    // fieldListModel.clear();
                    clearDialog.close();
                }
            }
        }
    }

    
}
