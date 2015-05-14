import QtQuick 2.1

FocusScope {
    clip:true
    activeFocusOnTab:true
    ListView {
        id: listView
        anchors{fill: parent; leftMargin: 30; rightMargin: 30 ;topMargin:5}
        //width: parent.width - 50; height: parent.height - 20
        focus: true
        model: ResultModel
        cacheBuffer: 50
        delegate: ListViewDelegate{}
    }
}
