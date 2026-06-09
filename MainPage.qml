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
        color:"#7fffff"
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
                    background: Rectangle{
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

                Label {
                    text: "Чке получилосуь:"
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

    // Starting from Qt 6.6, camera permission should be requested manually. For older Qt versions, CameraPermission object can be simply removed as Qt automatically requests the permission.
    CameraPermission {
        id: cameraPermission
        Component.onCompleted: {
            if (status !== Qt.PermissionStatus.Granted);
            // request()
        }
    }

    // End of Qt 6.6+ section
    Camera {
        id: camera
        cameraDevice: devices.videoInputs[camerasComboBox.currentIndex] ? devices.videoInputs[camerasComboBox.currentIndex] : devices.defaultVideoInput
        focusMode: Camera.FocusModeAutoNear
        onErrorOccurred: console.log("camera error:" + errorString)
        active: false
        // cameraPermission.status === Qt.PermissionStatus.Granted // for Qt 6.6 and above
        // active: true // pre Qt 6.6
    }

    CaptureSession {
        id: captureSession
        camera: camera
        videoOutput: videoOutput
    }

    GridLayout {
        anchors.fill: parent
        anchors.leftMargin: 407
        anchors.topMargin: 6
        Rectangle {

            anchors.fill: parent
            color: "black"

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

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
