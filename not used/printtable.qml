import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import PrintTableModel

Page {
    width: 600
    height: 400

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: tableView.left
        anchors.top: parent.top
        syncView: tableView
        clip: true
    }

    VerticalHeaderView {
        id: verticalHeader
        anchors.top: tableView.top
        anchors.left: parent.left
        syncView: tableView
        clip: true
    }
    TableView {
        id: tableView
        anchors.left: verticalHeader.right
        anchors.top: horizontalHeader.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: PrintTableModel

        
        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            color: "#f0f0f0"
            border.color: "#ccc"

            Loader {
                anchors.fill: parent
                anchors.margins: 5

                sourceComponent: {
                    // Определяем тип данных
                    var displayValue = display;

                    if (typeof displayValue === 'boolean') {
                        return checkBoxComponent;
                    } else if (typeof displayValue === 'number') {
                        return numberComponent;
                    } else {
                        return textComponent;
                    }
                }
            }

            Component {
                id: textComponent
                Text {
                    text: display
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Component {
                id: numberComponent
                Text {
                    text: display
                    color: "blue"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
            }

            Component {
                id: checkBoxComponent
                CheckBox {
                    checked: display
                    anchors.centerIn: parent
                }
            }
        }
    }
}
// import QtQuick
// import QtQuick.Controls
// import Qt.labs.qmlmodels

// ApplicationWindow {
//     visible: true
//     width: 640
//     height: 480
//     title: qsTr("HeaderView")

//     Rectangle {
//         anchors.fill: parent

//         color: Application.styleHints.appearance === Qt.Light ? palette.mid : palette.midlight

//         HorizontalHeaderView {
//             id: horizontalHeader
//             anchors.left: tableView.left
//             anchors.top: parent.top
//             syncView: tableView
//             clip: true
//         }

//         VerticalHeaderView {
//             id: verticalHeader
//             anchors.top: tableView.top
//             anchors.left: parent.left
//             syncView: tableView
//             clip: true
//         }

//         TableView {
//             id: tableView
//             anchors.left: verticalHeader.right
//             anchors.top: horizontalHeader.bottom
//             anchors.right: parent.right
//             anchors.bottom: parent.bottom
//             clip: true

//             columnSpacing: 1
//             rowSpacing: 1

//             model: TableModel {
//                 TableModelColumn { display: "name" }
//                 TableModelColumn { display: "color" }

//                 rows: [
//                     {
//                         "name": "cat",
//                         "color": "black"
//                     },
//                     {
//                         "name": "dog",
//                         "color": "brown"
//                     },
//                     {
//                         "name": "bird",
//                         "color": "white"
//                     }
//                 ]
//             }

//             delegate: Rectangle {
//                 implicitWidth: 100
//                 implicitHeight: 20
//                 color: palette.base
//                 Label {
//                     text: display
//                 }
//             }
//         }
//     }
// }
