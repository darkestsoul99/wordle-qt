import QtQuick 2.0
import QtQuick.Controls 2.0

Switch {
    id: darkModeSwitch
    checked: false
    signal darkThemeSignal
    // Emit the custom signal when the Switch state changes
    onClicked: {
        darkThemeSignal();
        checked = !checked;
        console.log("HERE CLICKED");
    }
}
