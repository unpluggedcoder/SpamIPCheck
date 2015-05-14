import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.0
import "res"

Rectangle {
    width: 1024
    height: 700

    //! [filedialog]
    FileDialog {
        id: fileDialog
        modality: Qt.WindowModal
        title: "Choose a IP file"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "txt files (*.txt)" ]
        selectedNameFilter: "txt files (*.txt)"
        onAccepted: {
            Controller.ImportFile(fileUrl);
        }
    }
    //! [filedialog]

    FocusScope{
        id: mainView
        anchors.fill: parent
        focus: true
        MouseArea{
            anchors.fill: parent
            onClicked: mainView.focus = true
        }

        Rectangle{
            anchors.fill: parent
            Image {
                id: listbg
                anchors.fill: parent
                source: "qrc:///qml/SpamIPCheck/res/background.png"
                fillMode: Image.Stretch
            }
        }
        ListMenu {
            id: listMenu
            height: parent.height - 160;width: parent.width
            anchors.bottom: proBar.top; anchors.bottomMargin: 10
        }
        Rectangle{
            id: header
            height: 100
            width: 1024
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            Border{anchors.fill: parent}
            Rectangle{
                width: parent.height
                height: parent.width
                anchors.centerIn: parent
                rotation: 90
                gradient: Gradient{
                    GradientStop { position: 0.0; color: "#a6a6a6" }
                    GradientStop { position: 1.0; color: "#DFDFDF" }
                }
            }
            Label{
                text: "Spam IP Check"
                width: 400
                color: Qt.darker("lightblue")
                font.pixelSize: 45
                font.bold: true
                font.family: "MV Boli"
                x:50
                anchors.verticalCenter: parent.verticalCenter
                smooth: true
            }

            TextField{
                id: txtIP
                width: 150; height: 40;font.pixelSize: 18
                verticalAlignment: TextEdit.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                x: 675 ;textColor: "gray"
                text: "IP or Domain"
                onFocusChanged: {txtIP.focus ? txtIP.text="":txtIP.text="IP or Domain"}
                Keys.onReturnPressed: {
                    Controller.SingleCheck(text);
                }
            }
            Button{
                id: btnImport
                width: 100 ; height: 40; x: 850;focus: true
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "qrc:///qml/SpamIPCheck/res/import_A.png"
                onPressedChanged: {btnImport.pressed ? btnImport.iconSource = "qrc:///qml/SpamIPCheck/res/import_B.png": btnImport.iconSource = "qrc:///qml/SpamIPCheck/res/import_A.png"}
                onClicked: fileDialog.open()
            }
//            Button{
//                id: btnRun
//                width: 100;height: 40; x: 850
//                anchors.verticalCenter: parent.verticalCenter
//                iconSource: "res/proc_A.png"
//                onPressedChanged: {btnRun.pressed ? btnRun.iconSource = "res/proc_B.png": btnRun.iconSource = "res/proc_A.png"}
//            }
        }
        SimpleProgressBar  {
            id: proBar
            height: 23
            anchors{right: parent.right; left: parent.left; bottom: parent.bottom;leftMargin: 80; rightMargin: 80; bottomMargin: 30}
            //Behavior on value { NumberAnimation {duration: 500} }
            //value: 0
            //maximum: 0
            minimum: 0
            visible: false
            NumberAnimation{property: "value,opacity"; duration: 300;}
            NumberAnimation{property: "visible"; duration:300}
        }
    }

    Output{
        id: outputArea
        height: 200
        anchors.right: parent.right
        anchors.left: parent.left
        y:parent.height
        error: Controller.errorstatus

        Image{
            id: image
            source: "qrc:///qml/SpamIPCheck/res/open_up.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.top
            smooth: true
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    if(outputArea.activeFocus)
                    {
                        Controller.ResetErrorStatus();
                        mainView.focus = true
                    }
                    else
                        outputArea.focus = true
                }
            }
        }
    }

    states: State {
        name: "contextMenuOpen"
        when: outputArea.activeFocus
        PropertyChanges { target: outputArea; y: outputArea.parent.height-200; open: true; }
    }
    transitions: Transition {
        NumberAnimation { properties: "y"; duration: 300; easing.type: Easing.OutQuint }
    }


    Connections
    {
        target: Controller
        onShowMsg: {
            outputArea.textContent += msg;
        }
        onUpdateMax:{
            proBar.maximum = max;
        }
        onUpdateValue:{
            proBar.value = value;
            if(value >= proBar.maximum)
            {
                proBar.value = 0; proBar.maximum = 0;
                proBar.visible = false;
            }
            else
                proBar.visible = true;
        }
    }
}
