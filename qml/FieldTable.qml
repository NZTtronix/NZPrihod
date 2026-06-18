import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: window
    width: 1300
    height: 600
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

                TableView {
                    id: tableView
                    anchors.fill: parent
                    anchors.margins: 1

                    model: fieldTableModel
                    clip: true
                    columnSpacing: 1
                    rowSpacing: 1
                    selectionModel: ItemSelectionModel {}

                    delegate: Rectangle {
                        required property bool selected
                        required property bool current
                        implicitHeight: 40

                        border.color: "#e0e0e0"
                        border.width: 1
                        color: current ? "white" : "lightgray"

                        property int columnIndex: column

                        TextInput {
                            id: cellInput
                            anchors.fill: parent
                            anchors.margins: 6
                            readOnly: false
                            text: {
                                var item = fieldTableModel.getItem(row);
                                if (!item)
                                    return "";
                                if (columnIndex < 4) {
                                    var key = ["X", "Y", "width", "height"][columnIndex];
                                    var v = item[key];
                                    return (typeof v === "number") ? v.toFixed(2) : (v !== undefined ? v : "0.00");
                                } else {
                                    var key = (columnIndex === 4) ? "content" : "codeType";
                                    return item[key] !== undefined ? item[key] : "";
                                }
                            }

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: columnIndex < 4 ? Text.AlignHCenter : Text.AlignLeft
                            // readOnly: true
                            color: "#333333"
                            font.pixelSize: 12

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                                onDoubleClicked: {
                                    cellInput.readOnly = false;
                                    cellInput.selectAll();
                                    cellInput.forceActiveFocus();
                                    var idx = fieldTableModel.index(row, columnIndex);
                                    tableView.selectionModel.setCurrentIndex(idx, 1);
                                }
                            }

                            onEditingFinished: {
                                if (!readOnly) {
                                    var value = text;
                                    if (columnIndex < 4)
                                        value = parseFloat(text) || 0;
                                    var idx = fieldTableModel.index(row, columnIndex);
                                    fieldTableModel.setData(idx, value, Qt.EditRole);
                                    // readOnly = true;
                                }
                            }

                            Keys.onEscapePressed: {
                                // readOnly = true;
                                // revert text
                                cellInput.text = (function () {
                                        var it = fieldTableModel.getItem(row);
                                        if (!it)
                                            return "";
                                        if (columnIndex < 4) {
                                            var k = ["X", "Y", "width", "height"][columnIndex];
                                            var vv = it[k];
                                            return (typeof vv === "number") ? vv.toFixed(2) : vv;
                                        } else {
                                            var k = (columnIndex === 4) ? "content" : "codeType";
                                            return it[k] || "";
                                        }
                                    })();
                            }

                            onActiveFocusChanged: {
                                if (activeFocus) {
                                    var idx = tableView.model.index(row, columnIndex);
                                    console.log("index is:" + idx);
                                    tableView.selectionModel.select(idx, tableView.selectionModel.Select | tableView.selectionModel.Clear);
                                }
                            }
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: (tableView.selectionModel.currentIndex.row === row && tableView.selectionModel.currentIndex.column === columnIndex) ? '#9ec5e5' : "transparent"
                            border.width: 2
                        }
                    }
                }
            }

            // Rectangle {
            //     id: canvas
            //     Layout.fillWidth: true
            //     Layout.fillHeight: true
            //     color: "white"
            //     border.color: "#ccc"
            //     border.width: 1
            //     radius: 4

            //     property var canvasItems: []

            //     function createCanvasRect(map) {
            //         var obj = canvasItemComponent.createObject(canvas);

            //         //                     // var qml = 'import QtQuick 2.15; Rectangle { x:' + (map.X || 0) +  color:"#2196F3"; opacity:0.8; border.width:2; border.color:"#1976D2"; radius:4; ' + 'Text { anchors.centerIn: parent; text: "' + (map.content || "") + '"; color:"white"; font.pixelSize:10 } }';
            //         // var qml =
            //         // 'import QtQuick 2.15; Rectangle { ' +
            //         // ' x:' + (map.X || 0) + '; y:' + (map.Y || 0) + '; width:' + (map.width || 20) + '; height:' + (map.height || 20) + '; color:"#2196F3"; opacity:0.8; border.width:2; border.color:"#1976D2"; radius:4; ' +
            //         // ' property int modelIndex: ' + (map._modelIndex !== undefined ? map._modelIndex : -1) + ';' +
            //         // ' MouseArea { anchors.fill: parent; cursorShape: Qt.OpenHandCursor; onPressed: cursorShape = Qt.ClosedHandCursor; onReleased: cursorShape = Qt.OpenHandCursor; drag.target: parent; onPositionChanged: { if (parent.modelIndex >= 0) { try { fieldTableModel.set(parent.modelIndex, { X: Math.round(parent.x), Y: Math.round(parent.y) }); } catch(e) {} } } onDragged: { /* optional continuous drag hook */ } onDragFinished: { if (parent.modelIndex >= 0) { try { fieldTableModel.set(parent.modelIndex, { X: Math.round(parent.x), Y: Math.round(parent.y) }); } catch(e) {} } } } ' +
            //         // ' Text { anchors.centerIn: parent; text: "' + (map.content || "") + '"; color:"white"; font.pixelSize:10 }' +
            //         // '}';
            //         //                     var obj = Qt.createQmlObject(qml, canvas, "dynamicCanvasItem");
            //         // var comp = Qt.createComponent("CanvasItem.qml");

            //         return obj;
            //     }

            //     Component.onCompleted: {
            //         // init existing items
            //         for (var r = 0; r < fieldTableModel.rowCount(); ++r) {
            //             var m = fieldTableModel.getItem(r);
            //             canvas.canvasItems.push(createCanvasRect(m));
            //         }

            //         fieldTableModel.rowsInserted.connect(function (parent, first, last) {
            //             for (var i = first; i <= last; ++i) {
            //                 var m = fieldTableModel.getItem(i);
            //                 var obj = createCanvasRect(m);
            //                 canvas.canvasItems.splice(i, 0, obj);
            //             }
            //         });

            //         fieldTableModel.rowsRemoved.connect(function (parent, first, last) {
            //             var count = last - first + 1;
            //             var removed = canvas.canvasItems.splice(first, count);
            //             for (var j = 0; j < removed.length; ++j)
            //                 removed[j].destroy();
            //         });

            //         fieldTableModel.dataChanged.connect(function (topLeft, bottomRight, roles) {
            //             var r0 = topLeft.row;
            //             var r1 = bottomRight.row;
            //             for (var rr = r0; rr <= r1; ++rr) {
            //                 var m = fieldTableModel.getItem(rr);
            //                 var obj = canvas.canvasItems[rr];
            //                 if (obj) {
            //                     obj.x = m.X;
            //                     obj.y = m.width !== undefined ? m.Y : m.Y;
            //                     obj.width = m.width;
            //                     obj.height = m.height;
            //                     if (obj.children && obj.children.length > 0)
            //                         obj.children[0].text = m.content;
            //                 }
            //             }
            //         });
            //     }
            // }
            Rectangle {
                id: canvas
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "white"
                border.color: "#ccc"
                border.width: 1
                radius: 4

                property var canvasItems: []

                Component.onCompleted: {
                    // Инициализация по существующим строкам
                    for (var r = 0; r < fieldTableModel.rowCount(); ++r) {
                        var m = fieldTableModel.getItem(r);
                        var obj = canvasItemComponent.createObject(canvas, {
                            x: m.X || m.XRole || 0,
                            y: m.Y || m.YRole || 0,
                            width: m.width || m.WidthRole || 20,
                            height: m.height || m.HeightRole || 20,
                            modelIndex: r
                        });
                        canvas.canvasItems.push(obj);
                    }

                    // Обработка вставки строк
                    fieldTableModel.rowsInserted.connect(function (parent, first, last) {
                        for (var i = first; i <= last; ++i) {
                            var m = fieldTableModel.getItem(i);
                            var obj = canvasItemComponent.createObject(canvas, {
                                x: m.X || m.XRole || 0,
                                y: m.Y || m.YRole || 0,
                                width: m.width || m.WidthRole || 20,
                                height: m.height || m.HeightRole || 20,
                                modelIndex: i
                            });
                            canvas.canvasItems.splice(i, 0, obj);
                        }
                        // скорректировать modelIndex для последующих объектов
                        for (var k = 0; k < canvas.canvasItems.length; ++k)
                            canvas.canvasItems[k].modelIndex = k;
                    });

                    // Удаление строк
                    fieldTableModel.rowsRemoved.connect(function (parent, first, last) {
                        var count = last - first + 1;
                        var removed = canvas.canvasItems.splice(first, count);
                        for (var j = 0; j < removed.length; ++j)
                            removed[j].destroy();
                        // скорректировать modelIndex для последующих объектов
                        for (var k = 0; k < canvas.canvasItems.length; ++k)
                            canvas.canvasItems[k].modelIndex = k;
                    });

                    // Обновления данных в моделях (dataChanged сигнал)
                    fieldTableModel.dataChanged.connect(function (topLeft, bottomRight, roles) {
                        var r0 = topLeft.row;
                        var r1 = bottomRight.row;
                        for (var rr = r0; rr <= r1; ++rr) {
                            var m = fieldTableModel.getItem(rr);
                            var obj = canvas.canvasItems[rr];
                            if (obj && m) {
                                if (m.X !== undefined)
                                    obj.x = m.X;
                                if (m.Y !== undefined)
                                    obj.y = m.Y;
                                if (m.width !== undefined)
                                    obj.width = m.width;
                                if (m.height !== undefined)
                                    obj.height = m.height;
                                if (m.content !== undefined && obj.children && obj.children.length > 0)
                                    obj.children[0].text = m.content;
                            }
                        }
                    });
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "➕ Add Row"
                Layout.preferredWidth: 150
                onClicked: fieldTableModel.addRow()
            }

            Button {
                text: "🗑️ Remove Selected"
                Layout.preferredWidth: 200
                enabled: tableView.currentIndex.isValid
                onClicked: {
                    fieldTableModel.removeRow(tableView.selectionModel.currentIndex.row);

                    // if (tableView.selectionModel.currentIndex.isValid()) {
                    //     var r = tableView.selectionModel.currentIndex;
                    //     console.log("tableView.selectionModel.currentIndex is "+ tableView.selectionModel.currentIndex)

                    //     fieldTableModel.removeRow(r);

                    // }
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
                text: "Total rows: " + fieldTableModel.rowCount()
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
                        text: "Selected Row: " + (tableView.selectionModel.currentIndex.row + 1)
                        font.pixelSize: 12
                    }

                    Text {
                        text: {
                            var columns = ["X", "Y", "Width", "Height", "Content", "Code Type"];
                            return "Selected Column: " + (columns[tableView.selectionModel.currentIndex.column]);
                        }
                        font.pixelSize: 12
                    }

                    Text {
                        text: "Double-click to edit cell"
                        font.pixelSize: 12
                        color: "#666666"
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
                    fieldTableModel.clear();
                    clearDialog.close();
                }
            }
        }
    }

    Component {
        id: canvasItemComponent

        Rectangle {
            id: rect
            property int modelIndex: -1

            width: (rect.modelIndex >= 0 ? fieldTableModel.getItem(rect.modelIndex).WidthRole || fieldTableModel.getItem(rect.modelIndex).width || fieldTableModel.getItem(rect.modelIndex).width : 20)
            height: (rect.modelIndex >= 0 ? fieldTableModel.getItem(rect.modelIndex).HeightRole || fieldTableModel.getItem(rect.modelIndex).height || fieldTableModel.getItem(rect.modelIndex).height : 20)
            x: (rect.modelIndex >= 0 ? fieldTableModel.getItem(rect.modelIndex).XRole || fieldTableModel.getItem(rect.modelIndex).X || fieldTableModel.getItem(rect.modelIndex).x : 10)
            y: (rect.modelIndex >= 0 ? fieldTableModel.getItem(rect.modelIndex).YRole || fieldTableModel.getItem(rect.modelIndex).Y || fieldTableModel.getItem(rect.modelIndex).y : 10)

            color: "#242578"
            radius: 4
            border.width: 2
            border.color: "#1e2250"
            opacity: 0.9

            Text {
                anchors.centerIn: parent
                text: (rect.modelIndex >= 0 ? fieldTableModel.getItem(rect.modelIndex).ContentRole || fieldTableModel.getItem(rect.modelIndex).content || "" : "")
                color: "white"
                font.pixelSize: 12
            }

            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: parent
                cursorShape: Qt.OpenHandCursor
                onPressed: cursorShape = Qt.ClosedHandCursor

                // Обновляем модель при движении (live) — можно убрать для записи только по завершению
                onPositionChanged: {

                    // if (rect.modelIndex >= 0) {

                    //     var idx = rect.modelIndex;
                    //     // формируем QModelIndex на стороне QML через стандартный вызов setData(model.index(row, column), value, role)
                    //     // В QML у модели есть метод setData(index, value, role) — доступен, если модель предоставляет интерфейс QAbstractItemModel.
                    //     // Здесь создаём индекс через model.index(row, 0)
                    //     try {
                    //         var modelIndex = fieldTableModel.index(idx, 0);
                    //         fieldTableModel.setFieldXY(rect.modelIndex, Math.round(rect.x), Math.round(rect.y));
                    //         fieldTableModel.setFieldXY(rect.modelIndex, Math.round(rect.x), Math.round(rect.y));

                    //     } catch (e) {
                    //         // если setData/index недоступны напрямую, можно вызвать Q\_INVOKABLE wrapper в модели или использовать getItem+custom setter
                    //         console.log("Model update failed:", e);
                    //     }
                    // }
                }

                // Гарантированная финальная запись после перетаскивания
                onReleased: {
                    console.log("onReleasedCalled");

                    cursorShape = Qt.OpenHandCursor;

                    if (rect.modelIndex >= 0) {
                        try {
                            var idx = fieldTableModel.index(rect.modelIndex, 0);
                            fieldTableModel.setFieldXY(rect.modelIndex, Math.round(rect.x), Math.round(rect.y));
                            fieldTableModel.setFieldXY(rect.modelIndex, Math.round(rect.x), Math.round(rect.y));

                            console.log(idx);
                            console.log(Math.round(rect.x), Math.round(rect.y));
                        } catch (e) {
                            console.log("final update failed:", e);
                        }
                    }
                }
            }

            // Drag.hotSpot.y: 10
            // Drag.hotSpot.x: 10
        }
    }
}
