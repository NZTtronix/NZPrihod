import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import Backend 1.0

Item {
    id: root
    property var barcodeAnalyzerId
    property var stackViewId
    property var templateName
    property var templatePath
    anchors.fill: parent

    Component {
        id: templatePropertyRecognition
        TemplatePropertyRecognition {}
    }
    Component {
        id: templatePropertyParse
        TemplatePropertyParse {}
    }

    TemplatesSideBar {
        id: templateProperties
        barcodeAnalyzerId: root.barcodeAnalyzerId
        stackViewId: root.stackViewId
        headerText: "Параметры " + root.templateName
        menuItemNames: ["Recognition", "Keyword", "Parse", "Print"]

        onItemClicked: {
            console.log("Clicked at ", itemName);

            if (itemName == "Recognition") {
                var params = {
                    barcodeAnalyzerId: root.barcodeAnalyzerId,
                    stackViewId: root.stackViewId,
                    stackOfTemplateId: stackOfTemplate,
                    templateName: root.templateName,
                    templatePath: root.templatePath
                };
                var obj = templatePropertyRecognition.createObject(stackOfTemplate, params);
                if (!obj)
                    console.log("creation failed:", templatePropertyRecognition.errorString());
                else {
                    console.log("created:", obj);
                    stackOfTemplate.replaceCurrentItem(obj); // или push/insert в StackView
                }
            }
            if (itemName == "Parse") {
                var params = {
                    barcodeAnalyzerId: root.barcodeAnalyzerId,
                    stackViewId: root.stackViewId,
                    stackOfTemplateId: stackOfTemplate,
                    templateName: root.templateName,
                    templatePath: root.templatePath
                };
                var obj = templatePropertyParse.createObject(stackOfTemplate, params);
                if (!obj)
                    console.log("creation failed:", templatePropertyParse.errorString());
                else {
                    console.log("created:", obj);
                    stackOfTemplate.replaceCurrentItem(obj); // или push/insert в StackView
                }
            }
        }
        onBackClicked: {
            console.log("Clicked at back");
            root.stackViewId.popCurrentItem();
        }
    }

    StackView {
        id: stackOfTemplate
        anchors.left: templateProperties.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}

// Item {
//     id: root
//     property var barcodeAnalyzerId
//     property var stackViewId
//     property string materialSelectedName
//     implicitWidth: 1440
//     implicitHeight: 900
//     signal materialSelected(string name)
//     function populateModelFromJson(obj) {
//         templateContentListModel.clear()
//         if (!obj || !obj.barcodes)
//             return
//         for (var i = 0; i < obj.barcodes.length; ++i) {
//             var entry = obj.barcodes[i]
//             templateContentListModel.append({
//                                                 "content": entry.content ? entry.content : ""
//                                             })
//             console.log("appended model with entry.content = " + entry.content)
//         }
//     }
//     function onMaterialSelectedHandler(name) {
//         materialSelectedName = name
//         console.log("Material selected:", materialSelectedName)
//         var currentTemplateJsonString = barcodeAnalyzerId.loadJsonStringFromTemplateFile(
//                     name)

//         console.log("currentTemplateJsonString:", currentTemplateJsonString)
//         var obj = JSON.parse(currentTemplateJsonString)
//         populateModelFromJson(obj)
//         console.log(templateContentListModel)
//     }

//     anchors.fill: parent

//     RowLayout {
//         anchors.fill: parent

//         spacing: 0
//         Rectangle {
//             Layout.preferredWidth: 240
//             Layout.fillHeight: true

//             color: "#101019"

//             visible: true
//             ColumnLayout {
//                 width: 240

//                 Layout.preferredWidth: 240
//                 anchors.bottom: parent.bottom
//                 anchors.top: parent.top
//                 anchors.margins: 8
//                 anchors.topMargin: 0
//                 spacing: 8
//                 Text {
//                     text: "Material template"
//                     color: "white"
//                     anchors.top: parent.top
//                     font.bold: true
//                     font.pointSize: 12
//                     horizontalAlignment: Text.AlignLeft
//                 }

//                 ListView {
//                     id: listView
//                     anchors.top: parent.top - 100

//                     anchors.bottom: parent.bottom
//                     width: 240

//                     model: barcodeAnalyzerId.materialTemplatesNames // QStringList из C++
//                     clip: true

//                     delegate: ItemDelegate {
//                         width: parent.width
//                         text: modelData // имя файла
//                         checked: ListView.isCurrentItem
//                         highlighted: ListView.isCurrentItem
//                         onClicked: {
//                             listView.currentIndex = index

//                             // сигнал о выборе: можно отправить имя в корень приложения
//                             root.onMaterialSelectedHandler(
//                                         barcodeAnalyzerId.materialTemplates[index])
//                         }
//                     }
//                 }
//                 Rectangle {
//                     id: addnewtemplate
//                     color: "#102010"
//                     width: parent.width
//                     Text {
//                         text: "ADD NEW"
//                         anchors.verticalCenter: parent.verticalCenter
//                         anchors.horizontalCenter: parent.horizontalCenter

//                         color: "white"
//                     }
//                     MouseArea {
//                         anchors.fill: parent
//                         onClicked: {

//                         }
//                     }

//                     height: 70
//                     anchors.bottom: parent.bottom
//                 }
//             }
//         }
//         Rectangle {
//             Layout.fillWidth: true
//             Layout.fillHeight: true

//             color: "#14141E"

//             visible: true
//             id: columnLayout

//             width: parent.width
//             height: parent.height
//             Rectangle {
//                 color: "#101019"
//                 height: 50
//                 Text {
//                     text: "СhosenMaterial: " + materialSelectedName
//                     color: "white"
//                     font.bold: true
//                     font.pointSize: 12
//                 }
//             }
//             ListModel {
//                 id: templateContentListModel
//             }
//             ListView {

//                 id: listViewBarcodes
//                 x: 0
//                 y: 0
//                 height: 900
//                 width: 1000
//                 model: templateContentListModel
//                 clip: true
//                 delegate: ItemDelegate {

//                     width: parent.width
//                     height: 50
//                     text: content // имя файла
//                     checked: ListView.isCurrentItem
//                     highlighted: ListView.isCurrentItem
//                     onClicked: {

//                     }
//                 }
//             }

//             Popup {
//                 id: barcodeProperties
//                 focus: true
//                 closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
//                 width: parent.width * 0.8
//             }
//         }
//     }
// }

