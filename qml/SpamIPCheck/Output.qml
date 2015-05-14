import QtQuick 2.0
import QtQuick.Controls 1.0

FocusScope {
    id: container

    property bool open: false
    property bool error: false
    property alias textContent: outText.text

    Item {
        anchors.fill: parent
        Border{anchors.fill: parent}
        Rectangle {
            anchors.fill: parent
            focus: true
            Keys.onDownPressed: mainView.focus = true
            Rectangle{
                width: parent.height
                height: parent.width
                anchors.centerIn: parent
                rotation: 90
                gradient: Gradient{
                    GradientStop { position: 0.0; color: "#B2B2B2" }
                    GradientStop { position: 1.0; color: "#DFDFDF" }
                }
            }
        }
        ScrollView {
//            id: flickArea
            anchors{fill:parent; margins: 10}
//            contentWidth: outText.width; contentHeight: outText.implicitHeight
//            flickableDirection: Flickable.VerticalFlick
            clip: true
            Text {
                id: outText
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.RichText
                wrapMode: Text.Wrap
            }
        }
    }

    states: [
        State {
            name: "ErrorOccured"
            when: error&&!open
            PropertyChanges {target: image; source: "qrc:///qml/SpamIPCheck/res/error.png" }
        },
        State {
            name: "Opened"
            when: open
            PropertyChanges {target: image; source: "qrc:///qml/SpamIPCheck/res/close_down.png" }
        }
    ]

}
