import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import MyComponents 1.0
import FieldListModel 1.0

Page {
    id: root
    function onBackClicked() {
        console.log("back clicked:");
        root.stackViewId.popCurrentItem();
    }
    function onItemClicked(itemName) {
        console.log("Clicked at ", itemName);
        chosenLabelTemplate = itemName;
        fieldListModel.loadFromJson(root.printLabelsPath + root.chosenLabelTemplate + ".json");

        fieldListAndCanvasRect.visible = true;

        var params = {
            fieldListModelID: fieldListModel,
            chosenLabelTemplate: root.chosenLabelTemplate,
            printLabelsPath: root.printLabelsPath
        };
        var obj = fieldsAndCanvasComponent.createObject(stackViewfieldListAndCanvasRect, params);
        if (!obj)
            console.log("creation failed:", fieldsAndCanvasComponent.errorString());
        else {
            console.log("created:", obj);
            stackViewfieldListAndCanvasRect.replaceCurrentItem(obj); // или push/insert в StackView
        }
    }

    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId
    property var keywordsPath: "C:/projects/qt/QmlAppTest/appdata/keywords/keywords.json"
    property var templatePath: "C:/projects/qt/QmlAppTest/appdata/templates/"
    property var printLabelsPath: "C:/projects/qt/QmlAppTest/appdata/printlabels/"
    property var chosenLabelTemplate
    anchors.fill: parent
    background: Rectangle {
        color: "#4f8f8f"
        opacity: 0.1
    }
    header: CustomPageHeader {
        id: myHeader
    }

    palette {
        // основное текстовое значение
        dark: "#101019"

        text: "white"
        // дополнительные цвета (при необходимости)
        buttonText: "white"
        windowText: "white"
        base: "transparent"
        window: "transparent"
        // фон окна (если нужно)
        button: "transparent"
    }

    // FIELDLISTMODEL
    FieldListModel {
        id: fieldListModel
    }
    //LABEL TEMPLATES LIST SUBHEADER
    ListModel {
        id: labelsList
    }
    Rectangle {
        id: subheader
        width: 300
        height: 100
        x: 50
        color: "transparent"
        Column {
            Row {
                height: 65
                width: 200
                ItemDelegate {
                    id: control
                    icon.source: "Resources/icons/print.svg"
                    icon.width: 40
                    icon.height: 40
                    icon.color: control.highlighted ? "white" : "white"
                    display: ItemDelegate.IconOnly
                    palette.windowText: "white"
                    background: Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        height: 0.6 * parent.height
                        width: 5
                        color: '#7fffff00'
                        opacity: highlighted ? 0.5 : 0

                        Rectangle {
                            height: 80
                            anchors.verticalCenter: parent.verticalCenter
                            width: root.menuWidth
                            color: "white"
                            opacity: highlighted ? 0.1 : 0
                        }
                    }
                }
                Text {
                    text: "Label templates"
                    width: 250
                    wrapMode: Text.WordWrap
                    font.pointSize: 20
                    font.bold: true
                    color: "white"
                }
            }
            Text {
                width: 300
                color: "white"
                wrapMode: Text.WordWrap
                text: "Add a new label template or edit existing label templates"
            }
        }
    }

    //LABEL TEMPLATES LIST

    Rectangle {
        id: labelTemplatesListRect
        width: 300
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 100
        color: "transparent"

        anchors.bottomMargin: 100
        x: 50
        y: 10
        CustomListView {
            id: customListView
            marginsTop: 0
            margins: 1
            itemHeight: 40
            itemWidth: 300
            textColor: "white"
            customListItemNames: root.barcodeAnalyzerId.getFolderJsonBasenames(root.printLabelsPath)
        }

        Column {
            anchors.top: loadImageButton.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            height: customListView.itemHeight
            width: parent.width
            spacing: 1
            Button {
                id: addNewButton
                height: 40
                width: parent.width

                background: Rectangle {
                    width: customListView.itemWidth
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "AddNew"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    console.log("clicked at AddNew");
                    addNewDialog.open();
                    // root.barcodeAnalyzerId.addNewEmptyPrintLabelJson(root.printLabelsPath);
                    // fieldListModelID.addEmptyItem();
                }
            }
            Button {
                id: renameButton
                height: 40

                background: Rectangle {
                    width: customListView.itemWidth
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "Rename"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    console.log("clicked at renameButton");
                }
            }

            Button {
                id: removeButton
                height: 40

                background: Rectangle {
                    width: customListView.itemWidth
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "remove"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    console.log("clicked at remove");
                }
            }
        }
    }

    // FIELDTABLEWINDOW
    Component {
        id: fieldsAndCanvasComponent
        FieldTable {}
    }
    Rectangle {
        id: fieldListAndCanvasRect
        color: 'transparent'
        anchors.topMargin: 0

        anchors.left: labelTemplatesListRect.right
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        StackView {
            id: stackViewfieldListAndCanvasRect
            anchors.fill: parent
        }
    }
    Dialog {
        id: addNewDialog
        title: "Создать новый шаблон этикетки"
        modal: true
        anchors.centerIn: parent
        width: 320
        property var name: "newLabel"
        contentItem: Column {
            Text {
                text: "Введите название в поле ниже"
                wrapMode: Text.WordWrap
                padding: 20
            }
             
            TextField {
                text: addNewDialog.name
                wrapMode: Text.WordWrap
                padding: 20
                onEditingFinished: addNewDialog.name = text
            }
        }

        footer: RowLayout {
            spacing: 10
            anchors.margins: 10

            Button {
                text: "Cancel"
                Layout.fillWidth: true
                onClicked: addNewDialog.close()
            }

            Button {
                text: "OK"
                Layout.fillWidth: true
                background: Rectangle {
                    color: '#101019'
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    if (fieldListModel.saveToJson(root.printLabelsPath + addNewDialog.name + ".json") === true) {
                        labelsList.append(addNewDialog.name);

                    };
                    addNewDialog.close()
                    
                }
            }
        }
    }
    Component.onCompleted: {
        // keywordsModel.loadFromJson(keywordsPath);
        // enabledKeywords.sourceModel = keywordsModel;
        // keywordsModel.loadFromJson(keywordsPath);

        customListView.itemClicked.connect(onItemClicked);
        myHeader.backClicked.connect(onBackClicked);

        var LabelNames = barcodeAnalyzerId.getFolderJsonBasenames(printLabelsPath);
        console.log("LabelNames.length:", LabelNames.length);
        // listViewBarcodes.currentIndex = -1;
        for (let i = 0; i < LabelNames.length; ++i) {
            labelsList.append(LabelNames[i]);
        }
        console.log("labelsList.count:", labelsList.count);
        // console.log("labelsListImage.source = ", Qt.resolvedUrl("C:/projects/qt/QmlAppTest/templates/NoImage.bmp"));

        //templateImage.source = "file:///C:/projects/qt/QmlAppTest/templates/NoImage.bmp";

    }
}
