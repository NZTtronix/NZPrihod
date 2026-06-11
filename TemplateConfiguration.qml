import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Layouts 2.15
import Backend 1.0
import MyComponents 1.0

Page {
    id: root

    function onBackClicked() {
        console.log("back clicked:");
        root.stackViewId.popCurrentItem();
    }
    function onItemClicked(itemName) {
        console.log("Clicked at ", itemName);
        chosenTemplate = itemName;
        var jsonstringTemplate = root.barcodeAnalyzerId.loadJsonStringFromTemplateFile(templatePath + chosenTemplate + ".json");
        console.log(jsonstringTemplate);
        var obj = JSON.parse(jsonstringTemplate);
        console.log("templateImage.source old = " + root.barcodeAnalyzerId.imgPath);

        console.log("templateImage.source = " + templatePath + itemName + ".bmp");

        templateImage.source = "file:///" + templatePath + itemName + ".bmp";
        rectReco.visible = true;
        getBarcodesList();

        // var paramsReco = {
        //     barcodeAnalyzerId: root.barcodeAnalyzerId,
        //     stackViewId: stackReco,
        //     templateName: itemName,
        //     templatePath: root.templatePath
        // };
        // var objReco = templatePropertyRecognition.createObject(stackReco, paramsReco);
        // if (!objReco)
        //     console.log("creation failed:", templatePropertyRecognition.errorString());
        // else {
        //     console.log("created:", objReco);
        //     stackReco.push(objReco);
        // }

        // var paramsParse = {
        //     barcodeAnalyzerId: root.barcodeAnalyzerId,
        //     stackViewId: stackParse,
        //     templateName: itemName,
        //     templatePath: root.templatePath
        // };
        // var objParse = templatePropertyParse.createObject(stackParse, paramsParse);
        // if (!objParse)
        //     console.log("creation failed:", templatePropertyParse.errorString());
        // else {
        //     console.log("created:", objParse);
        //     stackParse.push(objParse);
        // }
    }
    function onLoadImage(selectedImage) {
        console.log("onLoadImage is called, image: " + selectedImage + "current template is: " + chosenTemplate);
        console.log("copyImageToTemplatesFolderAsChosenTemplateImage returned " + barcodeAnalyzerId.copyImageToTemplatesFolderAsChosenTemplateImage(chosenTemplate, selectedImage));
        console.log("templateImage.source = " + templatePath + chosenTemplate + ".bmp");
        templateImage.source = "file:///" + templatePath + "NoImage.bmp";

        templateImage.source = "file:///" + templatePath + chosenTemplate + ".bmp";
        var log = barcodeAnalyzerId.processTemplateImage(chosenTemplate);
        console.log();
        templateImage.source = "file:///" + templatePath + chosenTemplate + "_result" + ".bmp";
    }
    function onToggleResult() {
        console.log("Clicked at ToggleResult");
        console.log("templateImage.source is: " + templateImage.source);
        console.log("string to compare 1 is:" + String("file:///" + templatePath + chosenTemplate + ".bmp"));
        console.log("string to compare 2 is:" + String("file:///" + templatePath + chosenTemplate + "_result" + ".bmp"));

        if (String("file:///" + templatePath + chosenTemplate + ".bmp") == templateImage.source)
            templateImage.source = "file:///" + templatePath + chosenTemplate + "_result" + ".bmp";
        else if (String("file:///" + templatePath + chosenTemplate + "_result" + ".bmp") == templateImage.source)
            templateImage.source = "file:///" + templatePath + chosenTemplate + ".bmp";
    }
    function onCaptureImage() {
        console.log("Clicked at CaptureImage");
    }
    function getBarcodesList() {
        console.log("root.templatePath: ", templatePath, "root.templateName: ", chosenTemplate);
        var currentTemplateJsonString = barcodeAnalyzerId.loadJsonStringFromTemplateFile(templatePath + chosenTemplate + ".json");
        console.log("currentTemplateJsonString:", currentTemplateJsonString);

        var obj = JSON.parse(currentTemplateJsonString);
        barcodesListModel.clear();
        if (!obj || !obj.barcodes)
            return;
        for (var i = 0; i < obj.barcodes.length; ++i) {
            var entry = obj.barcodes[i];
            barcodesListModel.append({
                "idx": i,
                "content": entry.content,
                "type": entry.type,
                "widthpx": entry.width
            });
            console.log("appended model with entry.content = " + entry.content);
        }
        console.log();
    }
    function onBarcodeClicked(indexClicked) {
        console.log("Clicked at barcode #:  ", indexClicked);
    }

    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId
    property var templatePath: "C:/projects/qt/QmlAppTest/templates/"
    property var chosenTemplate
    anchors.fill: parent
    background: Rectangle {
        color: "#4f8f8f"
        opacity: 0.1
    }
    header: CustomPageHeader {
        id: myHeader
    }

    //TEMPLATES LIST SUBHEADER
    ListModel {
        id: templateList
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
                    icon.source: "Resources/icons/templates.svg"
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
                    text: "Registered list"
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
                text: "Add a new material template via capture or load image, redefine existing material templates"
            }
        }
    }

    //TEMPLATES LIST
    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 100
        color: "transparent"

        Rectangle {

            width: 300
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            x: 50
            y: 10
            color: "transparent"
            CustomListView {
                id: customListView
                marginsTop: 0
                margins: 1
                itemHeight: 40
                itemWidth: 300

                textColor: "white"
                customListItemNames: root.barcodeAnalyzerId.getFolderJsonBasenames(root.templatePath)
            }

            Column {
                anchors.bottom: parent.bottom
                spacing: 1
                Button {
                    id: addNewButton
                    height: 40

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
                        root.onAddNewItem();
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
                        root.onAddNewItem();
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
                        root.onAddNewItem();
                    }
                }
            }
        }
    }
    //IMAGE
    Rectangle {
        id: rectReco
        x: 400
        y: 0
        width: 800
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.bottomMargin: 100
        color: "transparent"
        visible: false

        Rectangle {

            width: 800
            height: 800
            color: "black"

            Image {
                id: templateImage
                cache: false
                anchors.fill: parent
                source: root.barcodeAnalyzerId.imgPath
                fillMode: Image.PreserveAspectFit
            }
        }
        //IMAGE BUTTONS
        Column {
            width: parent.width

            anchors.bottom: parent.bottom
            spacing: 1
            Button {
                id: loadImageButton
                height: 40
                width: parent.width

                background: Rectangle {
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "loadImage"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    confirmLoadDialog.open();
                }
            }
            Button {
                id: captureImageButton
                height: 40
                width: parent.width

                background: Rectangle {
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "captureImage"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    root.onCaptureImage();
                }
            }

            Button {
                id: toggleResultButton
                height: 40
                width: parent.width

                background: Rectangle {
                    height: 40
                    color: "#101019"
                    Text {
                        anchors.centerIn: parent
                        text: "toggleResult"
                        color: "white"
                        font.pointSize: 14
                    }
                }
                onClicked: {
                    root.onToggleResult();
                }
            }
        }
    }

    // BARCODES LIST
    ListModel {
        id: barcodesListModel
    }
    Rectangle {
        id: rectBarcodes
        anchors.left: rectReco.right
        anchors.leftMargin: 50
        y: 0
        width: 300
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.bottomMargin: 100
        color: "transparent"
        visible: true

        ListView {
            id: listViewBarcodes

            anchors.fill: parent

            model: barcodesListModel
            ScrollIndicator.vertical: ScrollIndicator {
                parent: listview.parent
                anchors.top: listview.top
                anchors.left: listview.right
                anchors.bottom: listview.bottom
            }
            spacing: 1
            delegate: Rectangle {
                color: ListView.isCurrentItem ? "#203030" : "#101019"
                width: parent.width
                height: 60
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listViewBarcodes.currentIndex = index;
                        root.onBarcodeClicked(index);
                    }
                }
                Column {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 60

                    Label {
                        text: "[" + idx + "]: " + content
                        color: "white"
                        font.bold: true
                        font.pixelSize: 16
                    }
                    Label {
                        text: "type: " + type //+    "\t"+  "width:"+  "\t" +widthpx
                        color: "white"
                        font.pixelSize: 16
                    }
                }
            }
        }
    }
    // FILEDIALOGS
    MessageDialog {
        id: confirmLoadDialog
        title: "Загрузить изображение"
        text: "Вы уверены, что хотите загрузить новое изображение шаблона? Старый шаблон будет перезаписан"
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            // пользователь нажал OK — выполняем вашу функцию
            fileDialog.open();
        }
    }
    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            root.onLoadImage(selectedFile);
        }
    }

    //STACKVIEW
    Rectangle {
        id: stackRectParse
        x: rectReco.x + width + 50
        y: 0
        width: rectReco.width + 200
        color: "transparent"
        height: parent.height - y
        StackView {
            id: stackParse
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        customListView.itemClicked.connect(onItemClicked);
        myHeader.backClicked.connect(onBackClicked);
        var MaterialNames = barcodeAnalyzerId.getFolderJsonBasenames(templatePath);
        console.log("MaterialNames.length:", MaterialNames.length);
        listViewBarcodes.currentIndex = -1;
        for (let i = 0; i < MaterialNames.length; ++i) {
            templateList.append(MaterialNames[i]);
        }
        console.log("templateList.count:", templateList.count);
        console.log("templateImage.source = ", Qt.resolvedUrl("C:/projects/qt/QmlAppTest/templates/NoImage.bmp"));

        templateImage.source = "file:///C:/projects/qt/QmlAppTest/templates/NoImage.bmp";
    }

    // NOT USED COMPONENTS
    

    Component {
        id: templatePropertyParse
        TemplatePropertyParse {}
    }
}
