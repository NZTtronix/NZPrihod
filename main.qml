import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore
// ОБЯЗАТЕЛЬНО: для работы StandardPaths в Qt 6
import Backend 1.0

Window {
    id: root
    function onItemClicked(itemName) {
        var obj;
        var params;
        console.log("Clicked at ", itemName);
        if (itemName == "Main") {
            stack.replaceCurrentItem(mainPage);
        }

        if (itemName == "Templates") {
            params = {
                barcodeAnalyzerId: analyzer,
                stackViewId: stack,
                templatePath: "C:/projects/qt/QmlAppTest/templates/",
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
                stackViewId: stack,
                path: "C:/projects/qt/QmlAppTest/keywords/",
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
                path: "C:/projects/qt/QmlAppTest/print/",
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
    x: 1400
    y: 800

    visible: true
    color: "#101019"
    BarcodeAnalyzer {
        id: analyzer
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
        MainPage {
            stackViewId: stack
            barcodeAnalyzerId: analyzer
        }
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
    }
}
