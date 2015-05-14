import QtQuick 2.0
import QtQuick.Controls 1.0

Item {
    id: container
    width: ListView.view.width; height: 60; anchors.leftMargin: 10; anchors.rightMargin: 10
    property real detailsOpacity : 0

    Rectangle {
        id: content
        anchors.centerIn: parent; width: container.width -6 ; height: container.height - 6
        color: "transparent"
        antialiasing: true
        radius: 10

        Rectangle { anchors.fill: parent; anchors.margins: 3; color: "#FCF8B3"; antialiasing: true; radius: 8 }
    }

    Item{
        id: topLayout
        x:15; y:5; height: iplabel.height; width: container.width -6
        Text {
            id: iplabel
            x:5 ;textFormat: Text.RichText; height: 54
            horizontalAlignment: Text.AlignLeft
            text: "<h1>" + ip + domain + "</h1>"
            font.pixelSize: 18
        }
        Text {
            id: listlabel
            textFormat: Text.RichText; height: 54; width:parent.width
            anchors{right: parent.right; rightMargin: 25 }
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignRight
            text: "<h2><font color=\"red\">" + listedNum + "</font> listed</h2>"
            font.pixelSize: 18
        }
    }
    Flickable{
        id: flick
        x:20; clip:true
        anchors { top: topLayout.bottom; topMargin: 5; bottom: parent.bottom; bottomMargin: 20 ; leftMargin: 20}
        opacity: container.detailsOpacity
        width: container.width -6
        contentWidth: details.width; contentHeight: details.height

        Column{
            id: details
            height: 800
            Text{
                id: dnsblTxt
                textFormat: Text.RichText
                opacity: container.detailsOpacity
                text: "<h2>Dnsbl.info</h2>" + dnsblResult + "<br>"
                font.pixelSize: 20
            }
            Text{
                id: antiTxt
                textFormat: Text.RichText
                opacity: container.detailsOpacity
                text: "<h2>Anti-Spam.org.cn</h2>" + antiResult + "<br>"
                font.pixelSize: 20
            }
            Text{
                id: trmTxt
                textFormat: Text.RichText
                opacity: container.detailsOpacity
                text: "<h2>TrendMicro.com</h2>" + trmResult + "<br>"
                font.pixelSize: 20
            }
        }
    }


    function changestatus(){
        if(container.state != 'Details')
            container.state = 'Details'
        else
            container.state = 'active'
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onDoubleClicked: { changestatus() }
        onClicked: {
            container.ListView.view.currentIndex = index
            container.forceActiveFocus()
        }
    }

    states: [
        State {
            name: "active"; when: container.activeFocus
            PropertyChanges { target: content; color: "#FF9933"; }
        },
        State {
            name: "Details"
            PropertyChanges { target: content; color: "#FF9933"; }
            PropertyChanges { target: iplabel; height: 70;}
            //PropertyChanges { target: domainlabel; font.pixelSize: 15; height: 30 }
            PropertyChanges { target: container; detailsOpacity: 1; } // Make details visible
            PropertyChanges { target: container; height: listMenu.height -10 } // Fill the entire list area with the detailed view

            // Move the list so that this item is at the top.
            PropertyChanges { target: container.ListView.view; explicit: true; contentY: container.y }

            // Disallow flicking while we're in detailed view
            PropertyChanges { target: container.ListView.view; interactive: false }
        }
    ]
    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            ColorAnimation { property: "color"; duration: 200 }
            NumberAnimation { duration: 200; properties: "detailsOpacity,x,contentY,height,width,font.pixelSize" }
        }
    }
}
