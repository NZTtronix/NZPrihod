import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Shapes
import QtMultimedia
import QtCore
import KeywordsModel 1.0
import QtQuick.Controls.impl

// ОБЯЗАТЕЛЬНО: для работы StandardPaths в Qt 6
import ZXing 1.0
import Backend 1.0

Page {
    function onBackClicked() {
        console.log("back clicked:");
        root.stackViewId.popCurrentItem();
    }

    property var hikLiveProvider

    property var headerText: "Main Page"
    property var barcodeAnalyzerId
    property var stackViewId
    property var keywordsModelID
    property var fieldListModelID

    property var keywordsPath: "C:/projects/qt/QmlAppTest/appdata/keywords/keywords.json"
    property var templatePath: "C:/projects/qt/QmlAppTest/appdata/templates/"
    property var printLabelsPath: "C:/projects/qt/QmlAppTest/appdata/printlabels/"
    property var pathToChosenImage

    anchors.fill: parent

    background: Rectangle {
        color: "#4f8f8f"
        opacity: 0.1
    }
    header: CustomPageHeader {
        id: myHeader
    }

    palette {
        text: "white"
        buttonText: "white"
        windowText: "white"
        window: "#101019"
        button: "#101019"
        base: "#101019"
        dark: "#101019"
        brightText: "white"
        highlight: "#101019"
        alternateBase: "#101019"
        accent: "#101019"
        light: "#101019"
    }

    ColumnLayout {
        id: rightcolumnlayout
        width: 300
        Layout.fillHeight: true
        anchors.right: parent.right
        ExtractionListView {
            id: extractionListView
            anchors.right: parent.right
        }
        Label {
            text: "Лог:"
            font.bold: true
        }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 400
            Rectangle {
                anchors.fill: parent
                border.color: "#cccccc"
                border.width: 1
                color: "transparent"
                Text {
                    id: resultArea

                    color: "white"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    font.pointSize: 10
                }
            }
        }
    }

    GridLayout {
        id: gridlayout
        columnSpacing: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.rightMargin: 300
        height: 120

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -100

        MainPageButton {
            id: buttonTakeAPhoto
            iconSource: "Resources/icons/add_a_photo.svg"
            textlabel: "Сделать фото"
            marea.onClicked: {
                console.log("onClicked on ", textlabel);
                var now = new Date();
                var name = now.getFullYear() + pad(now.getMonth() + 1) + pad(now.getDate()) + "_" + pad(now.getHours()) + pad(now.getMinutes()) + pad(now.getSeconds()) + ".png";
                var path = "C:/projects/qt/QmlAppTest/appdata/History/" + name;

                analyzer.imgPath = "file:///" + path;
                imageCapture.captureToFile(path);
                image.source = analyzer.imgPath;
            }
            function pad(n) {
                return n < 10 ? "0" + n : n;
            }
        }
        MainPageButton {
            id: buttonLoadFromImage
            iconSource: "Resources/icons/add_photo_alternate.svg"
            textlabel: "Загрузить фото"
            marea.onClicked: {
                console.log("onClicked on ", textlabel);
                fileDialog.open();
            }
        }
        MainPageButton {
            id: buttonScan
            iconSource: "Resources/icons/qr_code_scanner.svg"
            textlabel: "Сканировать"
            marea.onClicked: {
                console.log("onClicked on ", textlabel);
                // if (pathToChosenImage.trim() === "") {
                //     resultArea.text = "Ошибка: Сначала укажите путь к файлу!";
                //     return;
                // }
                resultArea.text = "идет обрыботка";

                let outputLog = analyzer.processImage(pathToChosenImage);
                var newline1 = outputLog.indexOf("/n");
                let datetime = outputLog.substring(0, newline1);
                resultArea.text = outputLog;
            }
        }
        MainPageButton {
            id: buttonSwitchLiveMode
            property var checked: capture.camera.active
            iconSource: checked ? "Resources/icons/live_tv.svg" : "Resources/icons/tv_off.svg"
            textlabel: "Live картинка"
            marea.onClicked: {
                console.log("onClicked on ", textlabel);
                checked: capture.camera.active;
                onClicked: {
                    capture.camera.active ^= true;
                }
            }
        }
        MainPageButton {
            id: buttonSwitchLight
            iconSource: "Resources/icons/lightbulb.svg"
            textlabel: "Подсветка"
            marea.onClicked: {
                console.log("onClicked on ", textlabel);
            }
        }
    }
    FileDialog {
        id: fileDialog
        title: "Выберите файл изображения"
        // nameFilters: ["Изображения (*.jpg *.png *.bmp *.jpeg)"]
        onAccepted: {
            // Записываем локальный путь в текстовое поле ввода
            pathToChosenImage = fileDialog.selectedFile;
        }
    }
    

    MediaDevices {
        id: devices
    }
    Camera {
        id: myCameraId
        cameraDevice: devices.videoInputs[camerasComboBox.currentIndex] ? devices.videoInputs[camerasComboBox.currentIndex] : devices.defaultVideoInput
        focusMode: Camera.FocusModeAutoNear
        onErrorOccurred: console.log("camera error:" + errorString)
        active: cameraPermission.status === Qt.PermissionStatus.Granted // for Qt 6.6 and above

    }
    CaptureSession {
        id: capture
        camera: myCameraId
        imageCapture: ImageCapture {
            id: imageCapture
            quality: ImageCapture.VeryHighQuality
            onImageCaptured: {
                console.log("onImageCaptured ");
            }
            onImageSaved: {
                console.log("onImageSaved ");
                image.source = analyzer.imgPath;
                myCameraId.active = false;
                analyzer.processImage(analyzer.imgPath);
            }
        }
        videoOutput: myVideoOutputID
        recorder: MediaRecorder {
            id: recorder
            videoFrameRate: 30
        }
    }

    CameraPermission {
        id: cameraPermission
        Component.onCompleted: {
            if (status !== Qt.PermissionStatus.Granted);
            request();
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -100

        width: 1280
        height: 900

        anchors.leftMargin: 100
        anchors.topMargin: 0
        Rectangle {
            id: recoRectangle

            anchors.fill: parent
            color: "transparent"

            border.width: 1
            border.color: "#203030"

            // VideoOutput {
            //     id: myVideoOutputID
            //     visible: myCameraId.active ? true : false
            //     anchors.fill: recoRectangle
            // }
            
            Image {
                id: image
                cache: false // Обязательно!
                anchors.fill: parent
                source: analyzer.imgPath
                fillMode: Image.PreserveAspectFit
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false
            visible: devices.videoInputs.length > 1
            Label {
                text: qsTr("Camera: ")
                Layout.fillWidth: false
            }
            ComboBox {
                id: camerasComboBox
                Layout.fillWidth: true
                model: devices.videoInputs
                textRole: "description"
                currentIndex: 0
            }
        }
    }
}
