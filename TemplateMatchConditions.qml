import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    id: root
    property var barcodeAnalyzerId: analyzer
    property var stackViewId
    property var templateName: "murata.json"
    property var templatePath: "C:/projects/qt/QmlAppTest/templates/murata.json"
    implicitHeight: 1000
    implicitWidth: 1000


    
    ListModel {
        id: templatePropertyParseListModel
    }
    ListModel {
        id: keywordsListModel
    }
  
    Component.onCompleted: {
        getParseModelList();
        getKeywords();
    }
    function getParseModelList() {
        console.log("root.templatePath: ", templatePath, "root.templateName: ", templateName);

        var currentTemplateJsonString = barcodeAnalyzerId.loadJsonStringFromTemplateFile(templatePath + templateName);
        console.log("currentTemplateJsonString:", currentTemplateJsonString);

        var obj = JSON.parse(currentTemplateJsonString);
        templatePropertyParseListModel.clear();
        if (!obj || !obj.barcodes)
            return;
        if (!obj.parsemodel) {
            console.log("No Parse Model for this template");
            return;
        }
        for (var i = 0; i < obj.barcodes.length; ++i) {
            var entrybarcodes = obj.barcodes[i];
            var entryparse = obj.parsemodel[i];
            templatePropertyParseListModel.append({
                "idx": i,
                "content": entrybarcodes.content,
                "dst": entryparse.dst,
                "start": entryparse.start,
                "len":entryparse.length
                
            });
        }
        console.log(templatePropertyParseListModel.toString);
    }

    function getKeywords() {
        var currentTemplateJsonString = barcodeAnalyzerId.loadJsonStringFromTemplateFile(templatePath + templateName);
        var obj = JSON.parse(currentTemplateJsonString);
        keywordsListModel.clear();
        if (!obj || !obj.keywords)
            return;
        for (var i = 0; i < obj.keywords.length; ++i) {
            var entry = obj.keywords[i];
            keywordsListModel.append({
                "keyword": entry.keyword,
                "fullname": entry.fullname
            })
            
            
            // console.log("appended model with entry.content = " + entry.content);
        }
        console.log(keywordsListModel.toString);
    }



    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        ListView {
            id: listViewBarcodes

            anchors.fill: parent
            anchors.margins: 50

            model: templatePropertyParseListModel
            clip: true
            spacing: 10
            delegate: Rectangle {
                color: "#153030"
                width: parent.width
                height: 200

                Column {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 60
                    Label {
                        id: labelContent
                        text: "[" + idx + "]:" + "\t" + content + " => " + "\t" + parseStringWithSubstring(content, parseInt(prefixInput.text), parseInt(lengthInput.text))
                        color: "white"
                        font.bold: true
                        font.pixelSize: 24
                    }

                    Row {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 50
                        spacing: 50
                        Label {
                            text: "Начало подстроки: "
                            color: "white"
                            font.pixelSize: 24
                        }
                        Rectangle {
                            width: 100
                            height: 30
                            color: "#101019"
                            TextInput {
                                id: prefixInput
                                anchors.fill: parent
                                color: "white"
                                font.pixelSize: 24
                                horizontalAlignment: TextInput.AlignHCenter
                                inputMethodHints: Qt.ImhDigitsOnly
                                text: start
                               
                            }
                        }
                        Label {
                            text: "Длина подстроки: "
                            color: "white"
                            font.pixelSize: 24
                        }
                        Rectangle {
                            width: 100
                            height: 30
                            color: "#101019"
                            TextInput {
                                id: lengthInput
                                anchors.fill: parent
                                color: "white"
                                font.pixelSize: 24
                                horizontalAlignment: TextInput.AlignHCenter
                                inputMethodHints: Qt.ImhDigitsOnly
                                text: len
                                
                            }
                        }
                    }
                    Row {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 50
                        spacing: 50
                        Label {
                            text: "Назначение: "
                            color: "white"
                            font.pixelSize: 24
                        }
                        Rectangle {
                            id: comborect
                            width: 100
                            height: 40
                            color: "#101019"
                            ComboBox {
                                id: combobox
                                displayText: dst
                                model: keywordsListModel
                                textRole: "keyword"
                                anchors.fill: parent
                                onActivated: {
                                    displayText: currentText
                                    console.log("currentText: ",currentText) 
                                    var item = templatePropertyParseListModel.get(idx);
                                    item.dst = currentText
                                    console.log("item.dst: ",item.dst)
                                }
                            }
                        }
                     }
                    
                     
                    
                 }
            }
        }
    }
}
