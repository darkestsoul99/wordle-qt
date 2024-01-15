import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0

Rectangle {
    id: wordBoxInstance // Set a dynamic ID
    width: parent.width
    height: parent.height
    color: "white"
    border.color: "black"
    radius: 0

//        Behavior on scale {
//            NumberAnimation {
//                duration: 200
//            }
//        }

//        Behavior on rotation {
//            NumberAnimation {
//                duration: 500
//            }
//        }

//        states: [
//            State {
//                name: "correct"
//                PropertyChanges {
//                    target: wordBox
//                    color: "#8F8" // Green
//                }
//            },
//            State {
//                name: "partiallyCorrect"
//                PropertyChanges {
//                    target: wordBox
//                    color: "#FF8" // Yellow
//                }
//            },
//            State {
//                name: "incorrect"
//                PropertyChanges {
//                    target: wordBox
//                    color: "#F88" // Red
//                }
//            }
//        ]

//        transitions: [
//            Transition {
//                NumberAnimation {
//                    properties: "scale"
//                    duration: 500
//                    easing.type: Easing.OutBounce
//                }
//            },
//            Transition {
//                RotationAnimation {
//                    properties: "rotation"
//                    duration: 500
//                    easing.type: Easing.OutBounce
//                }
//            }
//        ]

    Label {
        id: letter
        anchors.centerIn: parent
        text: ""
        width: parent.width
        font.pixelSize: 16
        color: "black"
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
    }
}
