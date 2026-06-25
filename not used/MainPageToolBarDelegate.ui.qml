import QtQuick
import QtQuick.Controls

Button {
    id: delegateButton
    implicitWidth: 100
    implicitHeight: 100
    text: qsTr(model.label)
    icon.source: model.icon
    icon.color: "white"
    display: AbstractButton.TextUnderIcon
    onClicked: delegateButton.GridView.view.currentIndex = index
}
