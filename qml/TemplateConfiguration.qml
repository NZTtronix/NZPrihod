import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Layouts 2.15
import Backend 1.0
import MyComponents 1.0
import KeywordsModel 1.0
import EnabledProxyModel 1.0
import BarcodesModel 1.0

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

        // getBarcodesList();

        barcodesModel.loadFromJson(templatePath + chosenTemplate + ".json");
        matchModel.loadFromJson(templatePath + chosenTemplate + ".json");
        matchModel.setDataAccordingToBarcodesModel(barcodesModel);
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
    }
    function onBarcodeClicked(indexClicked) {
        console.log("Clicked at barcode #:  ", indexClicked);
    }

    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId
    property var keywordsPath: "C:/projects/qt/QmlAppTest/keywords/keywords.json"
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
                        console.log("clicked at AddNew")
                        root.barcodeAnalyzerId.addNewEmptyTemplateJson(root.templatePath);

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
                                                console.log("clicked at AddNew")

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
                                                console.log("clicked at AddNew")

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
    // ListModel {
    //     id: barcodesListModel
    // }
    BarcodesModel {
        id: barcodesModel
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
    Column{
        
    }
    Rectangle {
        id: rectBarcodes
        anchors.left: rectReco.right
        anchors.leftMargin: 50
        y: 0
        width: 450
        implicitHeight: 400
        // anchors.bottom: parent.verticalCenter

        anchors.top: parent.top
        // anchors.bottomMargin: 100
        color: "transparent"
        visible: true

        ListView {
            id: listViewBarcodes

            anchors.fill: parent

            model: barcodesModel
            ScrollIndicator.vertical: ScrollIndicator {
                parent: listViewBarcodes.parent
                anchors.top: listViewBarcodes.top
                anchors.left: listViewBarcodes.right
                anchors.bottom: listViewBarcodes.bottom
            }
            spacing: 1
            delegate: Rectangle {
                color: ListView.isCurrentItem ? "#203030" : "#101019"
                width: parent.width
                height: 50
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listViewBarcodes.currentIndex = index;
                    }
                }
                Column {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 50
                    Label {
                        text: "["+  index +"] : "+ContentRole
                        color: "white"
                        font.bold: true
                        font.pixelSize: 14
                    }
                    Label {
                        text: "type: " + CodeTypeRole
                        color: "white"
                        font.pixelSize: 14
                    }

                    // Label {
                    //     text: "width: " + WidthRole

                    //     color: "white"
                    //     font.pixelSize: 14
                    // }
                    // Label {
                    //     text: "area: " + AreaRole

                    //     color: "white"
                    //     font.pixelSize: 14
                    // }
                }
            }
        }
    }

    Rectangle {
        id: rectMatch
        anchors.left: rectReco.right
        anchors.leftMargin: 50
        y: 0
        width: 450
        anchors.bottom: parent.bottom
        anchors.top: rectBarcodes.bottom
        
        anchors.bottomMargin: 100
        color: "transparent"
        visible: true

      
        ListView {
            id: listView
            anchors.fill: parent
            anchors.bottomMargin: 100
            model: matchModel
            spacing: 30

            
            ScrollBar.vertical: ScrollBar {
                parent: listView.parent
                anchors.top: listView.top
                anchors.left: listView.right
                anchors.bottom: listView.bottom
            }
            
            delegate: Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 150
                color: ListView.isCurrentItem ? "#203030" : "#101019"
                border.width: 1;
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

                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {

                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0
                            Label {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                text: "Barcode ID:"
                            }
                            ComboBox {
                                id: comboboxid
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20

                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                model: barcodesModel

                                palette.windowText: "white"
                                displayText: "Barcode ID: " + CodeIDRole
                                textRole: "IdxRole"
                                onActivated: {
                                    CodeIDRole = currentText;
                                    // CodeTypeRole = barcodesListModel.data()

                                }
                            }
                        }
                    }
                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {

                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0
                            Label {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                text: "keyword"
                            }
                            ComboBox {
                                id: combobox
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                model: enabledKeywords
                                palette.windowText: "white"
                                displayText: KeywordRole
                                textRole: "IDRole"
                                onActivated: {
                                    KeywordRole = currentText;
                                }
                            }
                        }
                    }

                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {

                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0
                            CheckBox {
                                id: checkCodeTypeBox
                                checked: CheckCodeTypeRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                onCheckedChanged: CheckCodeTypeRole = checked
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                    // border.width: 1
                                    // border.color: "grey"
                                }

                                text: ": Match type"
                            }
                            TextField {
                                text: CodeTypeRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                color: "white"
                                visible: checkCodeTypeBox.checked
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                enabled: checkCodeTypeBox.checked
                                onEditingFinished: CodeTypeRole = text
                            }
                        }
                    }

                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {
                            CheckBox {
                                id: matchStartBox
                                checked: MatchStartRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                onCheckedChanged: MatchStartRole = checked
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                    // border.width: 1
                                    // border.color: "grey"
                                }
                                text: ": Match start"
                            }
                            TextField {
                                text: StartTextRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                color: "white"
                                placeholderText: "start text"
                                placeholderTextColor: "grey"
                                visible: matchStartBox.checked
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                onEditingFinished: StartTextRole = text
                            }
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0
                        }
                    }

                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0

                            CheckBox {
                                id: matchEndBox
                                checked: MatchEndRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20

                                onCheckedChanged: MatchEndRole = checked
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                    // border.width: 1
                                    // border.color: "grey"
                                }
                                text: ": Match end"
                            }
                            TextField {
                                text: EndTextRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                placeholderText: "end text"
                                placeholderTextColor: "#3f3f3f"

                                color: "white"
                                visible: matchEndBox.checked
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                onEditingFinished: EndTextRole = text
                            }
                        }
                    }

                    Item {

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40

                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0

                            CheckBox {
                                id: matchMiddleBox
                                checked: MatchMiddleRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                onCheckedChanged: MatchMiddleRole = checked
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                }
                                text: ": match middle"
                            }
                            TextField {
                                text: MiddleTextRole
                                visible: matchMiddleBox.checked
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                placeholderText: "middle text"
                                placeholderTextColor: "#3f3f3f"
                                color: "white"
                                enabled: matchMiddleBox.checked
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                onEditingFinished: MiddleTextRole = text
                            }
                        }
                    }

                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        ColumnLayout {
                             Layout.preferredWidth: 100
                        Layout.preferredHeight: 20
                        Label {
                            id: substring_start
                            text: "substring start"
                        }

                        SpinBox {
                            id: substringstartbox
                             Layout.preferredWidth: 100
                        Layout.preferredHeight: 20

                            from: -1
                            to: 100
                            stepSize: 1
                            value: SubstringStartRole
                            onValueChanged: SubstringStartRole = value
                        }
                        }
                        
                    }

                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20

                    
                            Label {

                                background: Rectangle {
                                    color: "transparent"
                                }
                                text: "substring length"
                            }

                        SpinBox {
                            id: substringLengthBox
                            from: -1
                            to: 100
                            value: SubstringLengthRole
                            onValueChanged: SubstringLengthRole = value
                            stepSize: 1
                        }
                        }
                    }

                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20

                            CheckBox {
                                id: matchSplitBox
                                checked: MatchSplitRole
                                onCheckedChanged: MatchSplitRole = checked
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                }
                                text: "split character"
                            }
                            TextField {
                                // anchors.top: matchSplitBox.bottom
                                text: SplitTextRole
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                placeholderText: "split symbols"
                                placeholderTextColor: "#3f3f3f"
                                color: "white"
                                background: Rectangle {
                                    color: "transparent"
                                    border.width: 1
                                    border.color: "grey"
                                }
                                onEditingFinished: SplitTextRole = text
                            }
                        }
                    }

                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        visible: matchSplitBox.checked
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20
                            spacing: 0
                            Label {
                                id: splitPart_l
                                text: "Split Part"
                                height: 20
                            }
                            SpinBox {
                                id: splitPart
                                anchors.top: splitPart_l.bottom
                                from: 0
                                to: 100
                                value: SplitPartRole
                                onValueChanged: SplitPartRole = value
                                stepSize: 1
                            }
                        }
                    }

                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20

                            CheckBox {
                                id: minlen_cb
                                checked: MatchMinLengthRole

                                onCheckedChanged: {
                                    MatchMinLengthRole = checked;
                                    if (!checked)
                                        minLen_box.value = 0;
                                }
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                }
                                text: ": Minimal length"
                            }

                            SpinBox {
                                // anchors.top:minlen_cb.bottom
                                id: minLen_box
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                // anchors.top: splitPart_l.bottom
                                from: 0
                                to: 100
                                value: MinLengthRole
                                onValueChanged: MinLengthRole = value
                                stepSize: 1
                            }
                        }
                    }
                    Item {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 40
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 20

                            CheckBox {
                                id: maxlen_cb
                                checked: MatchMaxLengthRole

                                onCheckedChanged: {
                                    MatchMaxLengthRole = checked;
                                    if (!checked)
                                        maxLen_box.value = 0;
                                }
                                hoverEnabled: false
                                background: Rectangle {
                                    color: "transparent"
                                }
                                text: ": Maximal length"
                            }

                            SpinBox {
                                id: maxLen_box
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 20
                                // anchors.top: splitPart_l.bottom
                                from: 0
                                to: 100
                                value: MaxLengthRole
                                onValueChanged: MaxLengthRole = value
                                stepSize: 1
                            }
                        }
                    }

                     
                }
            } 
        }
          Column  {
                anchors.top: listView.bottom
                anchors.bottom: parent.bottom
                anchors.left: listView.left
                anchors.right: listView.right
                width: rectMatch.width

                Button {
                    width: parent.width
                    height: 40

                    background: Rectangle {
                        anchors.fill: parent
                                                height: 40
                        color: "#101019"
                        Text {
                            anchors.centerIn: parent
                            text: "добавить новое правило"
                            color: "white"
                            font.pointSize: 14
                        }
                    }
                    onClicked: {
                        matchModel.addEmptyItem();
                    }
                }

                  Button {
                    width: parent.width
                    height: 40

                    background: Rectangle {
                        anchors.fill: parent
                                                height: 40
                        color: "#101019"
                        Text {
                                                text: "сохранить изменения в файл"

                            anchors.centerIn: parent
                            color: "white"
                            font.pointSize: 14
                        }
                    }
                    onClicked: {
                             matchModel.saveToJson(templatePath + chosenTemplate + ".json");
                    }
                }


                  Button {
                    width: parent.width
                    height: 40

                    background: Rectangle {
                        anchors.fill: parent
                                                height: 40
                        color: "#101019"
                        Text {
                            anchors.centerIn: parent
                    text: "удалить выделенное правило"
                            color: "white"
                            font.pointSize: 14
                        }
                    }
                    onClicked: {
                        matchModel.deleteItem(listView.currentIndex);
                    }
                }
                // Button {
                  

                //     Layout.alignment: Qt.AlignRight
                //     anchors.top: listView.bottom
                //     onClicked: {
                //         matchModel.addEmptyItem();
                //     }
                // }
                // Button {
                //     text: "удалить выделенное правило"
                //     Layout.alignment: Qt.AlignRight
                //     anchors.top: listView.bottom
                //     background: Rectangle {
                //         color:   hovered ?    "#203030": palette.dark ;
                //     }
                //     onClicked: {
                //         matchModel.deleteItem(listView.currentIndex);
                //     }
                // }
                // Button {

                //     text: "сохранить изменения в файл"
                //     Layout.alignment: Qt.AlignRight
                //     anchors.top: listView.bottom
                //     onClicked: {
                //         matchModel.saveToJson(templatePath + chosenTemplate + ".json");
                //     }
                // }
            }

        // FILEDIALOGS
    }
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
        keywordsModel.loadFromJson(keywordsPath);
        enabledKeywords.sourceModel = keywordsModel;

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

        keywordsModel.loadFromJson(keywordsPath);
    }
    
}
