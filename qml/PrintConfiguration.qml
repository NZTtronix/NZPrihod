import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import MyComponents 1.0
import Backend 1.0





Page {
    id: root

    // TemplateMatchConditions{}
    property var headerText
    property var barcodeAnalyzerId
    property var stackViewId

}
//     Rectangle{
//         id: PrintingField
//         implicitHeight: 100
//         implicitWidth: 100
//     }

//     ListModel{
//         id: paperFormatModel
//         ListElement {
//             format: "A4"
//             width: 210
//             height: 297 
//         }
//         ListElement {
//             format: "A4"
//             width: 210
//             height: 297 
//         }
        
//     }    

//     RowLayout{

//                 Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

//                             CheckBox {
//                                 id: minlen_cb
//                                 checked: MatchMinLengthRole

//                                 onCheckedChanged: {
//                                     MatchMinLengthRole = checked;
//                                     if (!checked)
//                                         minLen_box.value = 0;
//                                 }
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: ": Custom XY"
//                             }

//                             RowLayout {
//                                 SpinBox {
//                                 // anchors.top:minlen_cb.bottom
//                                 id: xxxxxx
                                
//                                 Layout.preferredWidth: 50
//                                 Layout.preferredHeight: 20
//                                 // anchors.top: splitPart_l.bottom
//                                 from: 0
//                                 to: 1000
//                                 value: MinLengthRole
//                                 onValueChanged: MinLengthRole = value
//                                 stepSize: 1
//                             }
//                                 SpinBox {
//                                     // anchors.top:minlen_cb.bottom
                                    
                                    
//                                     Layout.preferredWidth: 50
//                                     Layout.preferredHeight: 20
//                                     // anchors.top: splitPart_l.bottom
//                                     from: 0
//                                     to: 1000
//                                     value: MinLengthRole
//                                     onValueChanged: MinLengthRole = value
//                                     stepSize: 1
//                                 }
//                             }

                            
//                         }
//                     }


// Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

//                             CheckBox {
//                                 id: minlen_cb
//                                 checked: MatchMinLengthRole

//                                 onCheckedChanged: {
//                                     MatchMinLengthRole = checked;
//                                     if (!checked)
//                                         minLen_box.value = 0;
//                                 }
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: ": Custom XY"
//                             }
//                             ComboBox {
//                                 id: comboboxFormat
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20

//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 model: paperFormatModel

//                                 palette.windowText: "white"
//                                 displayText: modelData
//                                 textRole: "IdxRole"
//                                 onActivated: {
//                                     CodeIDRole = currentText;
//                                     // CodeTypeRole = barcodesListModel.data()

//                                 }
//                             }

                           

                            
//                         }
//                     }

    
    
    
//     }

        







    



//  Rectangle {
//         id: rectMatch
//         anchors.left: rectReco.right
//         anchors.leftMargin: 50
//         y: 0
//         width: 450
//         anchors.bottom: parent.bottom
//         anchors.top: rectBarcodes.bottom
        
//         anchors.bottomMargin: 100
//         color: "transparent"
//         visible: true

      
//         ListView {
//             id: listView
//             anchors.fill: parent
//             anchors.bottomMargin: 100
//             model: matchModel
//             spacing: 30

            
//             ScrollBar.vertical: ScrollBar {
//                 parent: listView.parent
//                 anchors.top: listView.top
//                 anchors.left: listView.right
//                 anchors.bottom: listView.bottom
//             }
            
//             delegate: Rectangle {
//                 anchors.left: parent.left
//                 anchors.right: parent.right
//                 height: 150
//                 color: ListView.isCurrentItem ? "#203030" : "#101019"
//                 border.width: 1;
//                 border.color: "grey"

//                 MouseArea {
//                     anchors.fill: parent
//                     onClicked: {
//                         listView.currentIndex = index;
//                     }
//                 }
//                 GridLayout {
//                     columns: 4
//                     columnSpacing: 10
//                     rowSpacing: 10

//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {

//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0
//                             Label {
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 text: "Barcode ID:"
//                             }
//                             ComboBox {
//                                 id: comboboxid
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20

//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 model: barcodesModel

//                                 palette.windowText: "white"
//                                 displayText: "Barcode ID: " + CodeIDRole
//                                 textRole: "IdxRole"
//                                 onActivated: {
//                                     CodeIDRole = currentText;
//                                     // CodeTypeRole = barcodesListModel.data()

//                                 }
//                             }
//                         }
//                     }
//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {

//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0
//                             Label {
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 text: "keyword"
//                             }
//                             ComboBox {
//                                 id: combobox
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 model: enabledKeywords
//                                 palette.windowText: "white"
//                                 displayText: KeywordRole
//                                 textRole: "IDRole"
//                                 onActivated: {
//                                     KeywordRole = currentText;
//                                 }
//                             }
//                         }
//                     }

//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {

//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0
//                             CheckBox {
//                                 id: checkCodeTypeBox
//                                 checked: CheckCodeTypeRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 onCheckedChanged: CheckCodeTypeRole = checked
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     // border.width: 1
//                                     // border.color: "grey"
//                                 }

//                                 text: ": Match type"
//                             }
//                             TextField {
//                                 text: CodeTypeRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 color: "white"
//                                 visible: checkCodeTypeBox.checked
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 enabled: checkCodeTypeBox.checked
//                                 onEditingFinished: CodeTypeRole = text
//                             }
//                         }
//                     }

//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {
//                             CheckBox {
//                                 id: matchStartBox
//                                 checked: MatchStartRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 onCheckedChanged: MatchStartRole = checked
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     // border.width: 1
//                                     // border.color: "grey"
//                                 }
//                                 text: ": Match start"
//                             }
//                             TextField {
//                                 text: StartTextRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 color: "white"
//                                 placeholderText: "start text"
//                                 placeholderTextColor: "grey"
//                                 visible: matchStartBox.checked
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 onEditingFinished: StartTextRole = text
//                             }
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0
//                         }
//                     }

//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0

//                             CheckBox {
//                                 id: matchEndBox
//                                 checked: MatchEndRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20

//                                 onCheckedChanged: MatchEndRole = checked
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     // border.width: 1
//                                     // border.color: "grey"
//                                 }
//                                 text: ": Match end"
//                             }
//                             TextField {
//                                 text: EndTextRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 placeholderText: "end text"
//                                 placeholderTextColor: "#3f3f3f"

//                                 color: "white"
//                                 visible: matchEndBox.checked
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 onEditingFinished: EndTextRole = text
//                             }
//                         }
//                     }

//                     Item {

//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40

//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0

//                             CheckBox {
//                                 id: matchMiddleBox
//                                 checked: MatchMiddleRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 onCheckedChanged: MatchMiddleRole = checked
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: ": match middle"
//                             }
//                             TextField {
//                                 text: MiddleTextRole
//                                 visible: matchMiddleBox.checked
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 placeholderText: "middle text"
//                                 placeholderTextColor: "#3f3f3f"
//                                 color: "white"
//                                 enabled: matchMiddleBox.checked
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 onEditingFinished: MiddleTextRole = text
//                             }
//                         }
//                     }

//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                              Layout.preferredWidth: 100
//                         Layout.preferredHeight: 20
//                         Label {
//                             id: substring_start
//                             text: "substring start"
//                         }

//                         SpinBox {
//                             id: substringstartbox
//                              Layout.preferredWidth: 100
//                         Layout.preferredHeight: 20

//                             from: -1
//                             to: 100
//                             stepSize: 1
//                             value: SubstringStartRole
//                             onValueChanged: SubstringStartRole = value
//                         }
//                         }
                        
//                     }

//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

                    
//                             Label {

//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: "substring length"
//                             }

//                         SpinBox {
//                             id: substringLengthBox
//                             from: -1
//                             to: 100
//                             value: SubstringLengthRole
//                             onValueChanged: SubstringLengthRole = value
//                             stepSize: 1
//                         }
//                         }
//                     }

//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

//                             CheckBox {
//                                 id: matchSplitBox
//                                 checked: MatchSplitRole
//                                 onCheckedChanged: MatchSplitRole = checked
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: "split character"
//                             }
//                             TextField {
//                                 // anchors.top: matchSplitBox.bottom
//                                 text: SplitTextRole
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 placeholderText: "split symbols"
//                                 placeholderTextColor: "#3f3f3f"
//                                 color: "white"
//                                 background: Rectangle {
//                                     color: "transparent"
//                                     border.width: 1
//                                     border.color: "grey"
//                                 }
//                                 onEditingFinished: SplitTextRole = text
//                             }
//                         }
//                     }

//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         visible: matchSplitBox.checked
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20
//                             spacing: 0
//                             Label {
//                                 id: splitPart_l
//                                 text: "Split Part"
//                                 height: 20
//                             }
//                             SpinBox {
//                                 id: splitPart
//                                 anchors.top: splitPart_l.bottom
//                                 from: 0
//                                 to: 100
//                                 value: SplitPartRole
//                                 onValueChanged: SplitPartRole = value
//                                 stepSize: 1
//                             }
//                         }
//                     }

//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

//                             CheckBox {
//                                 id: minlen_cb
//                                 checked: MatchMinLengthRole

//                                 onCheckedChanged: {
//                                     MatchMinLengthRole = checked;
//                                     if (!checked)
//                                         minLen_box.value = 0;
//                                 }
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: ": Minimal length"
//                             }

//                             SpinBox {
//                                 // anchors.top:minlen_cb.bottom
//                                 id: minLen_box
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 // anchors.top: splitPart_l.bottom
//                                 from: 0
//                                 to: 100
//                                 value: MinLengthRole
//                                 onValueChanged: MinLengthRole = value
//                                 stepSize: 1
//                             }
//                         }
//                     }
//                     Item {
//                         Layout.preferredWidth: 100
//                         Layout.preferredHeight: 40
//                         ColumnLayout {
//                             Layout.preferredWidth: 100
//                             Layout.preferredHeight: 20

//                             CheckBox {
//                                 id: maxlen_cb
//                                 checked: MatchMaxLengthRole

//                                 onCheckedChanged: {
//                                     MatchMaxLengthRole = checked;
//                                     if (!checked)
//                                         maxLen_box.value = 0;
//                                 }
//                                 hoverEnabled: false
//                                 background: Rectangle {
//                                     color: "transparent"
//                                 }
//                                 text: ": Maximal length"
//                             }

//                             SpinBox {
//                                 id: maxLen_box
//                                 Layout.preferredWidth: 100
//                                 Layout.preferredHeight: 20
//                                 // anchors.top: splitPart_l.bottom
//                                 from: 0
//                                 to: 100
//                                 value: MaxLengthRole
//                                 onValueChanged: MaxLengthRole = value
//                                 stepSize: 1
//                             }
//                         }
//                     }

                     
//                 }
//             } 
//         }
//           Column  {
//                 anchors.top: listView.bottom
//                 anchors.bottom: parent.bottom
//                 anchors.left: listView.left
//                 anchors.right: listView.right
//                 width: rectMatch.width

//                 Button {
//                     width: parent.width
//                     height: 40

//                     background: Rectangle {
//                         anchors.fill: parent
//                                                 height: 40
//                         color: "#101019"
//                         Text {
//                             anchors.centerIn: parent
//                             text: "добавить новое правило"
//                             color: "white"
//                             font.pointSize: 14
//                         }
//                     }
//                     onClicked: {
//                         matchModel.addEmptyItem();
//                     }
//                 }

//                   Button {
//                     width: parent.width
//                     height: 40

//                     background: Rectangle {
//                         anchors.fill: parent
//                                                 height: 40
//                         color: "#101019"
//                         Text {
//                                                 text: "сохранить изменения в файл"

//                             anchors.centerIn: parent
//                             color: "white"
//                             font.pointSize: 14
//                         }
//                     }
//                     onClicked: {
//                              matchModel.saveToJson(templatePath + chosenTemplate + ".json");
//                     }
//                 }


//                   Button {
//                     width: parent.width
//                     height: 40

//                     background: Rectangle {
//                         anchors.fill: parent
//                                                 height: 40
//                         color: "#101019"
//                         Text {
//                             anchors.centerIn: parent
//                     text: "удалить выделенное правило"
//                             color: "white"
//                             font.pointSize: 14
//                         }
//                     }
//                     onClicked: {
//                         matchModel.deleteItem(listView.currentIndex);
//                     }
//                 }
               
//             }

//     }

    
    
// }
