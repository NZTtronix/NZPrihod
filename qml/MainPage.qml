import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Shapes
import QtMultimedia
import QtCore

// ОБЯЗАТЕЛЬНО: для работы StandardPaths в Qt 6
import ZXing 1.0
import Backend 1.0

Page {
    property var barcodeAnalyzerId
    property var stackViewId
    anchors.fill: parent
    background: Rectangle {
        color: "#7fffff"
        opacity: 0.1
    }
    header: Item {
        height: 100

        Label {
            text: "главное акно"
            color: "white"
            wrapMode: Text.WordWrap
            height: 100
            font.pointSize: 14
            font.bold: true
            background: Rectangle {
                anchors.fill: parent
                color: "#7fffff"
                opacity: 0.0
            }
        }
    }

    RowLayout {
        id: rowLayout
        x: 0
        y: 0
        width: 385
        height: 890

        ColumnLayout {
            width: 477
            height: 825

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: parent.width
            spacing: 10

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                FileDialog {
                    id: fileDialog
                    title: "Выберите файл изображения"
                    // nameFilters: ["Изображения (*.jpg *.png *.bmp *.jpeg)"]
                    onAccepted: {
                        // Записываем локальный путь в текстовое поле ввода
                        pathInput.text = fileDialog.selectedFile;
                    }
                }

                anchors.margins: 20
                spacing: 15

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    TextField {
                        id: pathInput
                        placeholderText: "путь к файлу"
                        Layout.fillWidth: true
                        font.pointSize: 11
                    }

                    Button {
                        text: "найти файл"
                        onClicked: fileDialog.open()
                    }
                }

                Button {
                    text: "Сканировать"
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    highlighted: true

                    onClicked: {
                        if (pathInput.text.trim() === "") {
                            resultArea.text = "Ошибка: Сначала укажите путь к файлу!";
                            return;
                        }
                        resultArea.text = "идет обрыботка";

                        let outputLog = analyzer.processImage(pathInput.text);
                        var newline1 = outputLog.indexOf("/n");
                        let datetime = outputLog.substring(0, newline1);
                        resultArea.text = outputLog;
                    }
                }
                Button {
                    text: "Снимок"
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    highlighted: true
                    function pad(n){ return n<10 ? "0"+n : n }

                    onClicked: {
                        var now = new Date();
                        var name = now.getFullYear() + pad(now.getMonth() + 1) + pad(now.getDate()) + "_" + pad(now.getHours()) + pad(now.getMinutes()) + pad(now.getSeconds()) + ".png";
                        var path = "C:/projects/qt/QmlAppTest/History/" + name;
                        analyzer.imgPath = "file:///" + path;
                        imageCapture.captureToFile(path);
                    }
                }
                RadioButton {
                    text: "LIVE CAMERA"
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    
                    checked: capture.camera.active
                    onClicked: {
                        capture.camera.active ^= true;
                    }
                }
                Label {
                    text: "Че получилось:"
                    font.bold: true
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: resultArea
                        readOnly: true
                        placeholderText: "нихуя не происходит"
                        background: Rectangle {
                            border.color: "#cccccc"
                        }
                        font.pointSize: 11
                    }
                }
            }
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
                image.source = analyzer.imgPath
                myCameraId.active = false;
                analyzer.processImage(analyzer.imgPath);
            }
        }
        videoOutput : myVideoOutputID
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
        anchors.fill: parent
        width: 1280
        height: 900 
        anchors.leftMargin: 407
        anchors.topMargin: 6
        Rectangle {
            id: recoRectangle

            anchors.fill: parent
            color: "transparent"

            border.width: 1
            border.color: "#203030"
            
                VideoOutput {
            id: myVideoOutputID
            visible: myCameraId.active?true:false;
            anchors.fill: recoRectangle
            
        }

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
