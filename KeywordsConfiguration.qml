import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Page {
    id: root
    function onBackClicked() {
        console.log("back clicked:");
        root.stackViewId.popCurrentItem();
    }
    function onSaveChanges(){
        var out = { "keywords": [] };
        for (var i = 0; i < keywordsListModel.count; ++i) {
        var item = keywordsListModel.get(i);
        out.keywords.push({
            "keyword": item.keyword,
            "fullname": item.fullname
        });
        }
        var jsonstring = JSON.stringify(out, null, 4);
        console.log("jsonstring to save:", jsonstring);

        barcodeAnalyzerId.saveTo(jsonstring, path + "keywords.json")
    }
    function onAddNewItem(){
        keywordsListModel.append( {
            "keyword": "empty",
            "fullname": "empty"
        });
    }
    function onRemoveItems(index) {
        console.log("index:",index);
        keywordsListModel.remove(index,1);
        
    }
    signal addNewItem()
    signal saveChanges()
    signal removeItems()

    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId
    property var path
    anchors.fill: parent

    background: Rectangle {
        color: "#4f8f8f"
        opacity: 0.1
    }
    header: CustomPageHeader {
        id: myHeader
    }

    ListModel {
        id: keywordsListModel
    }

    ListView {
        id: listViewKeywords

        anchors.fill: parent
        anchors.margins: 50
        anchors.topMargin: 100
        anchors.bottomMargin: 200

        model: keywordsListModel
        clip: true
        delegate: Rectangle {
            color: "transparent"
            width: parent.width
            height: 41

            Column {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 60

                Row {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 50
                    spacing: 1

                    Rectangle {
                        width: 200
                        height: 40
                        color:  inputKeyword.activeFocus ? "#203030":"#101019"
                        MouseArea {
                            anchors.fill:parent
                            onClicked: {
                                inputKeyword.focus = true;
                                inputKeyword.cursorPosition = inputKeyword.text.length;
                                listViewKeywords.currentIndex = index;
                            }
                        }
                        TextInput {
                            id:inputKeyword
                            anchors.centerIn: parent
                            text: keyword
                            font.pointSize: 14
                            color: "white"
                            onTextChanged: {
                                 keywordsListModel.set(index, {"keyword": text, "fullname": fullname});
                            }
                            onCursorVisibleChanged: {
                                if (cursorVisible==true) listViewKeywords.currentIndex=index;

                            }
                        }
                    }
                    Rectangle {
                        width: 200
                        height: 40
                        color:  inputFullname.activeFocus? "#203030":"#101019"
                         MouseArea {
                            anchors.fill:parent
                            onClicked: {
                                inputFullname.focus = true;
                                inputFullname.cursorPosition = inputFullname.text.length;
                                listViewKeywords.currentIndex = index;
                            }
                        }
                        TextInput {

                            id:inputFullname
                            anchors.centerIn: parent
                            
                            text: fullname
                            color: "white"
                            font.pointSize: 14
                            onTextChanged: {
                                 keywordsListModel.set(index, {"keyword": keyword, "fullname": text});
                            }
                                                        onCursorVisibleChanged: {
                                if (cursorVisible==true) listViewKeywords.currentIndex=index;

                            }
                        }
                    }
                }
            }
        }
    }

    Button {
        id: addNewButton
        anchors.top: listViewKeywords.bottom
        anchors.right: listViewKeywords.right
        anchors.left: listViewKeywords.left 
        height: 40
        
        background: Rectangle{
        width: 400
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
        id: saveButton
        anchors.top: addNewButton.bottom
        anchors.topMargin: 1
        anchors.right: listViewKeywords.right
        anchors.left: listViewKeywords.left 
        height: 40
        
        background: Rectangle{
        width: 400
        height: 40
        color: "#101019"
            Text {
                anchors.centerIn: parent
                text: "Save"
                color: "white"
                font.pointSize: 14
            
            }
            
        }
        onClicked: {
                        root.onSaveChanges();

        }
        
    }
     Button {
        id: removeButton
        anchors.top: saveButton.bottom
        anchors.topMargin: 1
        anchors.right: listViewKeywords.right
        anchors.left: listViewKeywords.left 
        height: 40
        
        background: Rectangle{
        width: 400
        height: 40
        color: "#101019"
            Text {
                anchors.centerIn: parent
                text: "Remove active"
                color: "white"
                font.pointSize: 14
            
            }
            
        }
        onClicked: {    
            

            root.onRemoveItems(listViewKeywords.currentIndex);
        }
        
    }

    Rectangle {
        width: 300
        height: 100
        x: 50
        color: "transparent"
        Column {

            Row {
                height: 50
                width: parent.width
                ItemDelegate {
                    id: control
                    icon.source: "Resources/icons/keywords.svg"
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
                    text: "List of used keywords "
                    font.pointSize: 20
                    font.bold: true
                    color: "white"
                }
            }
            Text {
                width: 300
                color: "white"
                wrapMode: Text.WordWrap
                text: "Add some aliases for keywords which are needed to be parsed from the recognized image"
            }
        }
    }
    Component.onCompleted: {
        myHeader.backClicked.connect(onBackClicked);
        var jsonkeywordsstr = barcodeAnalyzerId.loadJsonStringFromTemplateFile(path + "keywords.json");
        console.log(jsonkeywordsstr);
        var obj = JSON.parse(jsonkeywordsstr);
        for (var i = 0; i < obj.keywords.length; ++i) {
            var entry = obj.keywords[i];
            keywordsListModel.append({
                "keyword": entry.keyword,
                "fullname": entry.fullname
            });
        }
        console.log("keywordsListModel.count:", keywordsListModel.count);
    }
}
