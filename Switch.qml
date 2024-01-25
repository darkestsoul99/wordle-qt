import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
// TODO: SEPERATE EACH SWITCH TO CHANGE COLORS
Switch {
    id: control
    property color backgroundColor: "#D9DDDC"
    indicator: Rectangle {
        id: switchBackground
        implicitHeight: 32
        implicitWidth: 56
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: width / 2
        color: control.backgroundColor
        border.width: 1
        border.color: control.backgroundColor

        Rectangle {
            id: switchIndicator
            x: control.checked ? (parent.width - width) - 2 : 2
            width: 28
            height: 28
            radius: height / 2
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
            Behavior on x {
                NumberAnimation { duration: 200 }
            }
        }
    }

    // Signals to be emitted based on the selected switch
    signal hardModeSignal(bool checked)
    signal darkModeSignal(bool checked)
    signal highContrastModeSignal(bool checked)
    signal keyboardOnlySignal(bool checked)

    onClicked: {
        switch(objectName) {
            case "hardModeWidget":
                hardModeSignal(checked);
                break;
            case "darkThemeWidget":
                darkModeSignal(checked);
                break;
            case "highContrastWidget":
                highContrastModeSignal(checked);
                break;
            case "onscreenInputOnlyWidget":
                keyboardOnlySignal(checked);
                break;
        }
    }
}
