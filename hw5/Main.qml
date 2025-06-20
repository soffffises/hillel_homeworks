import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: root
    visible: true
    width: 400
    height: 300
    title: qsTr("Temperature Converter")

    property string currentEditing: ""

    Dialog {
        id: errorDialog
        modal: true
        standardButtons: Dialog.Ok
        title: "Input Error"

        contentItem: Text {
            id: errorText
            text: ""
            wrapMode: Text.WordWrap
            width: parent.width * 0.9
            color: "red"
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onAccepted: {
            errorDialog.close()
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "Convert Temperatures"
            font.pixelSize: 22
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            color: "#2c3e50"
        }

        // Celsius
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Label {
                text: "Celsius"
                font.bold: true
                Layout.preferredWidth: 100
                color: "#34495e"
                verticalAlignment: Text.AlignVCenter
            }

            TextField {
                id: celsiusField
                Layout.preferredWidth: 220
                placeholderText: "Enter °C"
                font.pointSize: 12
                inputMethodHints: Qt.ImhPreferNumbers
                color: "black"
                background: Rectangle {
                    color: "white"
                    radius: 6
                    border.color: "#bdc3c7"
                    border.width: 1
                }
                palette {
                    text: "black"
                    base: "white"
                    highlight: "#3498db"
                    highlightedText: "white"
                }
                onTextEdited: {
                    root.currentEditing = "C"
                    if (text === "" || text === "-") return;
                    temperatureController.updateFromCelsius(text)
                }
                onEditingFinished: {
                    root.currentEditing = ""
                }
            }
        }

        // Fahrenheit
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Label {
                text: "Fahrenheit"
                font.bold: true
                Layout.preferredWidth: 100
                color: "#34495e"
                verticalAlignment: Text.AlignVCenter
            }

            TextField {
                id: fahrenheitField
                Layout.preferredWidth: 220
                placeholderText: "Enter °F"
                font.pointSize: 12
                inputMethodHints: Qt.ImhPreferNumbers
                color: "black"
                background: Rectangle {
                    color: "white"
                    radius: 6
                    border.color: "#bdc3c7"
                    border.width: 1
                }
                palette {
                    text: "black"
                    base: "white"
                    highlight: "#3498db"
                    highlightedText: "white"
                }
                onTextEdited: {
                    root.currentEditing = "F"
                    if (text === "" || text === "-") return;
                    temperatureController.updateFromFahrenheit(text)
                }
                onEditingFinished: {
                    root.currentEditing = ""
                }
            }
        }

        // Kelvin
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Label {
                text: "Kelvin"
                font.bold: true
                Layout.preferredWidth: 100
                color: "#34495e"
                verticalAlignment: Text.AlignVCenter
            }

            TextField {
                id: kelvinField
                Layout.preferredWidth: 220
                placeholderText: "Enter K"
                font.pointSize: 12
                inputMethodHints: Qt.ImhPreferNumbers
                color: "black"
                background: Rectangle {
                    color: "white"
                    radius: 6
                    border.color: "#bdc3c7"
                    border.width: 1
                }
                palette {
                    text: "black"
                    base: "white"
                    highlight: "#3498db"
                    highlightedText: "white"
                }
                onTextEdited: {
                    root.currentEditing = "K"
                    if (text === "" || text === "-") return;
                    temperatureController.updateFromKelvin(text)
                }
                onEditingFinished: {
                    root.currentEditing = ""
                }
            }
        }

        Button {
            text: "Reset"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                celsiusField.text = ""
                fahrenheitField.text = ""
                kelvinField.text = ""
                root.currentEditing = ""
            }
        }
    }

    Connections {
        target: temperatureModel

        function onTemperatureChanged() {
            const minC = -273.15;
            const minF = -459.67;
            const minK = 0.0;

            if (root.currentEditing !== "C") {
                let valC = temperatureModel.celsius;
                if (valC < minC) valC = minC;
                celsiusField.text = valC.toFixed(2);
            }
            if (root.currentEditing !== "F") {
                let valF = temperatureModel.fahrenheit;
                if (valF < minF) valF = minF;
                fahrenheitField.text = valF.toFixed(2);
            }
            if (root.currentEditing !== "K") {
                let valK = temperatureModel.kelvin;
                if (valK < minK) valK = minK;
                kelvinField.text = valK.toFixed(2);
            }
        }
    }

    Connections {
        target: temperatureController

        function onInvalidInput(message) {
            errorText.text = message
            errorDialog.open()
        }
    }

    Component.onCompleted: {
        temperatureController.updateFromCelsius("0")
    }
}
