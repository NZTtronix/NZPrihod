import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore
import Backend 1.0
import KeywordsModel 1.0
import FieldListModel 1.0


Window {
    id: root
    function onItemClicked(itemName) {
        var obj;
        var params;
        console.log("Clicked at ", itemName);
        if (itemName == "Main") {
            params = {
                barcodeAnalyzerId: analyzer,
                keywordsModelID: keywordsModel,
                fieldListModelID: fieldListModel,
                stackViewId: stack,
                headerText: itemName
            };
            obj = mainPage.createObject(stack, params);  
            if (!obj)
                console.log("creation failed:", mainPage.errorString());
            else {
                console.log("created:", obj);
                stack.replaceCurrentItem(obj); // или push/insert в StackView
            }

        }

        if (itemName == "Templates") {
            params = {
                barcodeAnalyzerId: analyzer,
                stackViewId: stack,
                templatePath: "C:/projects/qt/QmlAppTest/appData/templates/",
                headerText: itemName
            };
            obj = templateConfiguration.createObject(stack, params);
            if (!obj)
                console.log("creation failed:", templateConfiguration.errorString());
            else {
                console.log("created:", obj);
                stack.replaceCurrentItem(obj); // или push/insert в StackView
            }
        }
         if (itemName == "Keywords") {
            params = {
                barcodeAnalyzerId: analyzer,
                keywordsModelID: keywordsModel,
                stackViewId: stack,
                path: "C:/projects/qt/QmlAppTest/appData/keywords/",
                headerText: itemName
            };
             obj = keywordsConfiguration.createObject(stack, params);
            if (!obj)
                console.log("creation failed:", keywordsConfiguration.errorString());
            else {
                console.log("created:", obj);
                stack.replaceCurrentItem(obj); // или push/insert в StackView
            }

         }
         if (itemName == "PrintCfg") {
            params = {
                barcodeAnalyzerId: analyzer,
                stackViewId: stack,
                path: "C:/projects/qt/QmlAppTest/appdata/printlabels/",
                headerText: itemName
            };
            obj = printConfiguration.createObject(stack, params);
            if (!obj)
                console.log("creation failed:", printConfiguration.errorString());
            else {
                console.log("created:", obj);
                stack.replaceCurrentItem(obj); // или push/insert в StackView
            }
        }
       
          if (itemName == "History") {
                stack.replaceCurrentItem(clearPage);
            }
            if (itemName == "Settings") {
                stack.replaceCurrentItem(clearPage);
            }
            if (itemName == "About")
                stack.replaceCurrentItem(clearPage);
        

    }
    width: 1600
    height: 1200
    x: 1000
    y: 500

    visible: true
    color: "#101019"
    BarcodeAnalyzer {
        id: analyzer
    }
    KeywordsModel {
        id: keywordsModel
    }

    FieldListModel {
        id: fieldListModel
    }

    Component {
        id: keywordsConfiguration
        KeywordsConfiguration {}
    }
    Component {
        id: templateConfiguration
        TemplateConfiguration {}
    }
    Component {
        id: printConfiguration
        PrintConfiguration {}
    }
    Component {
        id: mainPage
        MainPage {}
    }
    Component {
        id: clearPage
        Rectangle {
            color: "black"
        }
    }
    MainMenuSideBar {
        id: menu
        stackViewId: stack
        menuItemNames: ["Main", "Templates", "Keywords", "PrintCfg", "History", "Settings", "About"]
        menuIconSize: 30
        menuWidth: 80
        menuItemHeight: 60
    }

    StackView {
        id: stack
        anchors.left: menu.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
    Component.onCompleted: {
            menu.itemClicked.connect(onItemClicked);
            keywordsModel.loadFromJson(path + "keywords.json");
            console.log("keywordsModel.rowCount():", keywordsModel.rowCount());


            
    }
}
