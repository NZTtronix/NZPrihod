import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import MyComponents 1.0
import PersonModel 1.0
import KeywordsModel 1.0
import EnabledProxyModel 1.0

ApplicationWindow {
    id: window
    property var barcodeAnalyzerId
    property var keywordModelId: "murata"
    property var keywordPath: "C:/projects/qt/QmlAppTest/keywords/keywords.json"

    property var templatePath: "C:/projects/qt/QmlAppTest/templates/murata/"
    property var chosenTemplate: "murata"
    visible: true
    width: 1000
    height: 1000
    title: "Match & extract params"
    palette {
        // основное текстовое значение

         text: "white"
        // // дополнительные цвета (при необходимости)
         buttonText: "white"
         windowText: "white"
         window: "#101019"      // фон окна (если нужно)
         button: "#101019"
         base: "#101019"
         dark: "#101019"
         brightText: "#101019"
         highlight: "#101019"
         alternateBase: "#101019"
         accent: "#101019"
         light: "#101019"
    }
    MatchModel {
        id: matchModel
    }
    KeywordsModel {
        id: keywordsModel
    }
    EnabledProxyModel {
        id: enabledKeywords
    }
    ListView {
        id: listView
        anchors.fill: parent
        anchors.bottomMargin: 100
        model: matchModel
        ScrollIndicator.vertical: ScrollIndicator {
            parent: listView.parent
            anchors.top: listView.top
            anchors.left: listView.right
            anchors.bottom: listView.bottom
        }
        delegate: Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            color: ListView.isCurrentItem ? "#203030" : "#101019"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index;
                }
            }
            Row {
                TextField {
                    color: "grey"
                    text: CodeIDRole
                    onEditingFinished: CodeIDRole = text
                }
             
                Rectangle {
                    id: comborect
                    color: "#101019"
                    width: 100
                    border.width: 1;
                    border.color: "grey" 
                    height: 20
                     ComboBox {
                        id: combobox
                        background: Rectangle { color: "transparent" } 
                        model: enabledKeywords
                        palette.windowText: white
                        displayText: "keyword: " + KeywordRole
                        textRole: "IDRole"
                        anchors.fill: parent
                        onActivated:{KeywordRole = currentText}
                    }

                }
                
                
            }
        }
    }
    Row {
        anchors.top: listView.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {

            text: "добавить"
            Layout.alignment: Qt.AlignRight
            anchors.top: listView.bottom
            onClicked: {
                matchModel.addEmptyItem();
            }
        }
        Button {

            text: "удалить"
            Layout.alignment: Qt.AlignRight
            anchors.top: listView.bottom
            onClicked: {
                matchModel.deleteItem(listView.currentIndex);
            }
        }
        Button {

            text: "сохранить"
            Layout.alignment: Qt.AlignRight
            anchors.top: listView.bottom
            onClicked: {
                matchModel.saveToJson(templatePath + chosenTemplate + ".json");
            }
        }
    }

    Component.onCompleted: {
        matchModel.loadFromJson(templatePath + chosenTemplate + ".json");
        keywordsModel.loadFromJson(keywordPath);
        enabledKeywords.sourceModel= keywordsModel;
    }

    // ApplicationWindow {
    //     id: window
    //     visible: true
    //     width: 1000
    //     height: 1000
    //     title: "Match & extract params"

    //     MatchModel {
    //         id: matchModel

    //         Component.onCompleted: {
    //             matchModel.loadFromJson("C:/projects/qt/QmlAppTest/templates/murata_match.json");
    //         }
    //     }

    //     ColumnLayout {
    //         anchors.fill: parent
    //         anchors.margins: 10
    //         spacing: 10
    //         anchors.bottomMargin: 10

    //         // Кнопка сохранения изменений

    //         // 2. Список элементов
    //         Rectangle {
    //             Layout.fillWidth: true
    //             Layout.fillHeight: true
    //             // model: matchModel
    //             clip: true
    //             // spacing: 8

    //             // 3. Делегат для отображения и редактирования полей
    //             // delegate:

    //              Frame {
    //                 width: 300

    //                 ColumnLayout {
    //                     anchors.fill: parent
    //                     spacing: 15
    //                     RowLayout {
    //                         Label {
    //                             text: "Barcode #" + matchModel.CodeIDRole
    //                             font.bold: true
    //                         }
    //                         TextField {
    //                             text: matchModel.CodeIDRole
    //                             Layout.preferredWidth: 30
    //                             onEditingFinished: matchModel.KeywordRole = text
    //                         }

    //                     }
    //                     RowLayout {
    //                     Label {
    //                             text: "Keyword"
    //                             font.bold: true
    //                         }
    //                         TextField {
    //                             text: matchModel.KeywordRole
    //                             Layout.preferredWidth: 100
    //                             onEditingFinished: matchModel.KeywordRole = text
    //                         }
    //                     }
    //                     RowLayout {
    //                         CheckBox {
    //                             text: "Starting characters"
    //                             checked: matchModel.MatchStartRole
    //                             onCheckedChanged: matchModel.MatchStartRole = checked
    //                         }
    //                         TextField {
    //                             visible: matchModel.MatchStartRole
    //                             text: matchModel.StartTextRole
    //                             Layout.preferredWidth: 100
    //                             onEditingFinished: matchModel.StartTextRole = text
    //                         }
    //                     }
    //                     RowLayout {
    //                         CheckBox {
    //                             text: "Ending characters"
    //                             checked: matchModel.MatchEndRole
    //                             onCheckedChanged: matchModel.MatchEndRole = checked
    //                         }
    //                         TextField {
    //                             visible: matchModel.MatchEndRole
    //                             text: matchModel.EndTextRole
    //                             Layout.preferredWidth: 100
    //                             onEditingFinished: matchModel.EndTextRole = text
    //                         }
    //                     }
    //                     RowLayout {
    //                         CheckBox {
    //                             text: "Match type"
    //                             checked: matchModel.CheckCodeTypeRole
    //                             onCheckedChanged: matchModel.CheckCodeTypeRole = checked
    //                         }
    //                         Label {
    //                             text: matchModel.CodeTypeRole
    //                             font.bold: true
    //                         }
    //                     }
    //                     RowLayout {
    //                         CheckBox {
    //                             text: "Case sensitive"
    //                             checked: matchModel.CaseSensitiveRole
    //                             onCheckedChanged: matchModel.CaseSensitiveRole = checked
    //                         }
    //                     }

    //                     // TextField {
    //                     //         visible: matchModel.MatchEndRole
    //                     //         text: matchModel.EndTextRole
    //                     //         Layout.preferredWidth: 100
    //                     //         onEditingFinished: matchModel.EndTextRole = text
    //                     //     }
    //                     // RowLayout {
    //                     //     CheckBox {
    //                     //         text: "Contains string"
    //                     //         checked: matchModel.MatchMiddleRole
    //                     //         onCheckedChanged: matchModel.MatchMiddleRole = checked
    //                     //     }
    //                     //     TextField {
    //                     //         visible: matchModel.MatchMiddleRole
    //                     //         text: matchModel.MiddleTextRole
    //                     //         Layout.preferredWidth: 100
    //                     //         onEditingFinished: matchModel.MatchMiddleRole = text
    //                     //     }
    //                     //     Label {
    //                     //         text: "N times"
    //                     //         font.bold: true
    //                     //         visible: matchModel.MatchMiddleRole

    //                     //     }
    //                     //     SpinBox {
    //                     //         from: 0
    //                     //         to: 99
    //                     //         value: matchModel.MiddleTextCountRole
    //                     //         Layout.preferredWidth: 100
    //                     //         onValueModified: matchModel.MinLengthRole = value
    //                     //                                     visible: matchModel.MatchMiddleRole

    //                     //     }

    //                     // }

    //                     // CheckBox {
    //                     //     text: "Регистрозависимый"
    //                     //     checked: matchModel.CaseSensitiveRole
    //                     //     onCheckedChanged: matchModel.CaseSensitiveRole = checked
    //                     // }
    //                 }
    //             }
    //         }
    //          Button {
    //             text: "Сохранить изменения в JSON"
    //             Layout.alignment: Qt.AlignRight
    //             onClicked: {
    //                 var success = matchModel.saveToJson("C:/projects/qt/QmlAppTest/templates/murata_match.json");
    //                 if (success) {
    //                     console.log("File Saved!");
    //                 } else {
    //                     console.error("FIle Save Error!");
    //                 }
    //             }
    //         }
    //     }

    // }

    // import QtQuick 2.15
    // import QtQuick.Controls 2.15
    // import QtQuick.Layouts 2.15
    // import MyComponents 1.0

    // Item {
    //     id: root
    //     property var barcodeAnalyzerId: analyzer
    //     property var stackViewId
    //     property var templateName: "murata.json"
    //     property var templatePath: "C:/projects/qt/QmlAppTest/templates/murata.json"
    //     implicitHeight: 800
    //     implicitWidth: 800
    //     anchors.fill:parent
    //     Rectangle{ anchors.fill:parent
    //     anchors.margins:200
    //     color:"lightGrey"}
    //         MatchModel {
    //             id: matchModel
    //         }
    //     Rectangle {
    //         id: rectBarcodes
    //         anchors.left: rectReco.right
    //         anchors.leftMargin: 50
    //         y: 0
    //         width: 300
    //         anchors.bottom: parent.bottom
    //         anchors.top: parent.top
    //         anchors.bottomMargin: 100
    //         color: "transparent"
    //         visible: true

    //         ListView {
    //             id: listViewBarcodes

    //             anchors.fill: parent

    //             model: barcodesListModel
    //             ScrollIndicator.vertical: ScrollIndicator {
    //                 parent: listview.parent
    //                 anchors.top: listview.top
    //                 anchors.left: listview.right
    //                 anchors.bottom: listview.bottom
    //             }
    //             spacing: 1
    //             delegate: Rectangle {
    //                 color: ListView.isCurrentItem ? "#203030" : "#101019"
    //                 width: parent.width
    //                 height: 60
    //                 MouseArea {
    //                     anchors.fill: parent
    //                     onClicked: {
    //                         listViewBarcodes.currentIndex = index;
    //                         root.onBarcodeClicked(index);
    //                     }
    //                 }
    //                 Column {
    //                     anchors.top: parent.top
    //                     anchors.left: parent.left
    //                     anchors.right: parent.right
    //                     height: 60

    //                     Label {
    //                         text: "[" + idx + "]: " + content
    //                         color: "white"
    //                         font.bold: true
    //                         font.pixelSize: 16
    //                     }
    //                     Label {
    //                         text: "type: " + type //+    "\t"+  "width:"+  "\t" +widthpx
    //                         color: "white"
    //                         font.pixelSize: 16
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //                 Component.onCompleted: {
    //                     console.log("loadFromJson")
    //                     matchModel.loadFromJson("C:/projects/qt/QmlAppTest/templates/murata_match.json")
    //                 }

    // }
}
