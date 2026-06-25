/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.impl 2.15

Rectangle {
    id: root
    signal canvasSizeChanged(real widthMM, real heightMM)

    Layout.fillHeight: true
    Layout.fillWidth: true
    palette {

        // основное текстовое значение
        // text: "white"
        // дополнительные цвета (при необходимости)
        buttonText: "white"
        // windowText: "white"
        base: "transparent"
        window: "transparent"
        highlight: "#203030"

        // window: "transparent"
        // фон окна (если нужно)
        button: "#203030"
    }

    property alias widthMM: spinBoxWidth.value
    property alias heightMM: spinBoxHeight.value

    

    ListModel {
        id: standartSizesModel
        ListElement {
            w: 50.0
            h: 50.0
            format: "50x50"
        }
        ListElement {
            w: 60.0
            h: 40.0
            format: "60x40"
        }
        ListElement {
            w: 80.0
            h: 70.0
            format: "80x70"
        }
        ListElement {
            w: 200.0
            h: 300.0
            format: "A4"
        }
    }
    GridLayout {
        id: gridLayout
        x: 0
        y: 0
        Layout.preferredHeight: 20

        ComboBox {
            id: chooseStandartCustom
            Layout.preferredWidth: 150
            model: ["Кастомный размер", "Стандартный размер"]
            background: Rectangle {
                color: "transparent"
                border.width: 2
                radius: 4
                border.color: "#CCCCCC"
            }

            Connections {
                target: standartSizeBox
                function onActivated(index) {
                    spinBoxWidth.value = standartSizesModel.get(index).w;
                    spinBoxHeight.value = standartSizesModel.get(index).h;
                }
            }
        }

        ComboBox {
            id: standartSizeBox
            Layout.preferredWidth: 150
            model: standartSizesModel
            visible: chooseStandartCustom.currentIndex === 1
            textRole: "format"
            background: Rectangle {
                color: "transparent"
                border.width: 2
                radius: 4
                border.color: "#CCCCCC"
            }

            Connections {
                target: standartSizeBox
                function onActivated(index) {
                    spinBoxWidth.value = standartSizesModel.get(index).w;
                    spinBoxHeight.value = standartSizesModel.get(index).h;
                }
            }
        }
        SpinBox {
            id: spinBoxWidth
            Layout.preferredWidth: 50
            visible: chooseStandartCustom.currentIndex === 0
            value: 60.0
            wheelEnabled: true
            editable: true
            to: 999.0
            background: Rectangle {
                color: "transparent"
            }
            Connections {
                target: spinBoxWidth
                function onValueChanged() {
                    root.canvasSizeChanged(widthMM, heightMM)
                }
            }
        }
        SpinBox {
            id: spinBoxHeight
            Layout.preferredWidth: 50
            visible: chooseStandartCustom.currentIndex === 0
            value: 40.0
            wheelEnabled: true
            editable: true
            to: 999.0
            background: Rectangle {
                color: "transparent"
            }
            Connections {
                target: spinBoxHeight
                function onValueChanged() {
                    root.canvasSizeChanged(widthMM, heightMM)
                }
            }     
        }

    }
}
