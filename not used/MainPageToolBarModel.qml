import QtQuick

ListModel {
    id: listModel

    ListElement {
        name: "printbutton"
        icon: "file:///C:/Users/admin/Desktop/fonts/material-design-icons/svg/outlined/print.svg"
        label: "Печать наклейки"
    }
    ListElement {
        name: "add_photo"
        icon: "file:///C:/projects/qt/QmlAppTest/qml/Resources/icons/add_a_photo.svg"
        label: "Сделать фото"
    }
    ListElement {
        name: "add_image"
        icon: "file:///C:/projects/qt/QmlAppTest/qml/Resources/icons/add_photo_alternate.svg"
        label: "Выбрать из изображения"
    }
    ListElement {
        name: "enable_live"
        icon: "file:///C:/Users/admin/Desktop/fonts/material-design-icons/svg/outlined/live_tv.svg"
        label: "вкл. Live с камеры"
    }
    ListElement {
        name: "enable_light"
        icon: "file:///C:/projects/qt/QmlAppTest/qml/Resources/icons/lightbulb.svg"
        label: "вкл. подсветку"
    }
}
