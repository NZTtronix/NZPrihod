import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15



Item {
    id: root
    property var barcodeAnalyzerId: analyzer
    property var stackViewId
    property var templateName: "murata.json"
    property var templatePath: "C:/projects/qt/QmlAppTest/templates/murata.json"

    ListModel {
        id: templatePropertyRecognitionListModel
    }
    Component.onCompleted :{
        getBarcodesList()
    }
    function getBarcodesList() {
        console.log("root.templatePath: ", templatePath, "root.templateName: ", templateName);
        var currentTemplateJsonString = barcodeAnalyzerId.loadJsonStringFromTemplateFile(templatePath + templateName);
        console.log("currentTemplateJsonString:", currentTemplateJsonString)
        
        var obj = JSON.parse(currentTemplateJsonString)
        templatePropertyRecognitionListModel.clear()
        if (!obj || !obj.barcodes)
            return
        for (var i = 0; i < obj.barcodes.length; ++i) {
            var entry = obj.barcodes[i]
            templatePropertyRecognitionListModel.append({
                                                "idx":  i,
                                                "content": entry.content ,
                                                "type":  entry.type,
                                                "widthpx":  entry.width

                                            })
                        console.log("appended model with entry.content = " + entry.content)
        }      
        console.log()
    }


    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "transparent"
         ListView {

                id: listViewBarcodes
              
                anchors.fill: parent
                anchors.margins: 50

                model: templatePropertyRecognitionListModel
                clip: true
                spacing: 10
                delegate: Rectangle {
                    color: "#153030"
                    width: parent.width
                    height: 100
                    
                    Column {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 60
                        
                        Label {
                            text: "["+ idx + "]:" + "\t" + content 
                            color: "white"
                            font.bold: true
                            font.pixelSize: 24
                        }
                        Label {
                             text: "type:"+  "\t" + "<b>" + type +"</b>"  +  "\t"+  "\t"+  "width:"+  "\t" + "<b>"+ widthpx +"</b>"  
                             color: "white"
                             
                             font.pixelSize: 24
                        }
    
                    }
                    

                }
                   

                   
        }
         Row{
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 50
                        height: 70
                        spacing: 80
                        Button{
                            height: parent.height 
                            width: parent.width * 0.4
                            background: Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                border.width: 1
                                border.color: "#306060"
                            }
                            icon.source : "resources/icons/image_capture.svg"
                            icon.color: "white"
                            icon.width: 48
                            icon.height: 48
                            font.pixelSize: 30
                            display: Button.TextBesideIcon
                            text: "Задать из файла"
                            palette.buttonText: "white"


                        }
                         Button{
                            height: parent.height
                            width: parent.width * 0.4
                            background: Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                border.width: 1
                                border.color: "#306060"
                            }
                            icon.source : "resources/icons/image_search.svg"
                            icon.color: "white"
                            icon.width: 48
                            icon.height: 48
                            font.pixelSize: 30
                            display: Button.TextBesideIcon
                            text: "Задать из файла"
                            palette.buttonText: "white"

                        }
                    }

    }

    


}