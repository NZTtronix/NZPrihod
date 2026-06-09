import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import Backend 1.0

Page {
    id: root
    function onBackClicked() {
            console.log("back clicked:");
            root.stackViewId.popCurrentItem();
         }
    function onItemClicked(itemName){
            console.log("Clicked at ", itemName);
            console.log("templatePath ", templatePath);
            
            var paramsReco = {
                barcodeAnalyzerId: root.barcodeAnalyzerId,
                stackViewId: stackReco,
                templateName: itemName,
                templatePath: root.templatePath
            };
            var objReco = templatePropertyRecognition.createObject(stackReco, paramsReco);
            if (!objReco)
                console.log("creation failed:", templatePropertyRecognition.errorString());
            else {
                console.log("created:", objReco);
                stackReco.push(objReco); 
            }
            
            var paramsParse = {
                barcodeAnalyzerId: root.barcodeAnalyzerId,
                stackViewId: stackParse,
                templateName: itemName,
                templatePath: root.templatePath
            };
            var objParse = templatePropertyParse.createObject(stackParse, paramsParse);
            if (!objParse)
                console.log("creation failed:", templatePropertyParse.errorString());
            else {
                console.log("created:", objParse);
                stackParse.push(objParse); 
            }

    }

    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId
    property var templatePath: "C:/projects/qt/QmlAppTest/templates/"
    anchors.fill: parent
    background: Rectangle {
        color: "#4f8f8f"
        opacity: 0.1
    }
    header: CustomPageHeader{
        id: myHeader
    }    
    Component {
        id: templatePropertyRecognition
        TemplatePropertyRecognition {}
    }
    Component {
        id: templatePropertyParse
        TemplatePropertyParse {}
    }
    


    Rectangle {
        anchors.fill:parent
        color: "transparent"
          Text {
                    x: 50
                    text: "Material templates list"
                    font.pointSize: 20
                    font.bold: true
                    color: "white"

                }
            Rectangle {
                
                width: 250
                height: 300
                color: "#CCCCCC"
                x: 50
                y: 50
              
                CustomListView {
                
                id: customListView; marginsTop: 0; margins: 10; itemHeight: 60; 
                
                textColor: "#000000"
                customListItemNames: root.barcodeAnalyzerId.getFolderContent(root.templatePath);
                }

            }
           
           Button {
               id: addNewTemplate;
               width: 250
               height: 50
               y:350
                x:50
                text: "Create New"


           }

    }
    
    Component.onCompleted: {
        myHeader.backClicked.connect(onBackClicked);
        customListView.itemClicked.connect(onItemClicked);
    }

    Rectangle {
        id: stackRectReco
        x: 400
        y: 0
        width: 600
        color:"transparent"
        height: parent.height - y
        StackView {
            id: stackReco
            anchors.fill: parent
        }
    }
    Rectangle {
        id: stackRectParse
        x: stackRectReco.x + width + 50
        y: 0
        width: stackRectReco.width + 200
color:"transparent"
        height: parent.height - y
        StackView {
            id: stackParse
            anchors.fill: parent
        }
    }
        
    
}
