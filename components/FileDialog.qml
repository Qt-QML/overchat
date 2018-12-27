import QtQuick 2.0
import QtQuick.Dialogs 1.1

FileDialog {
    property string fileURL: ""

    id: root
    modality: Qt.WindowModal
    title:  "Choose an image file"
    selectMultiple: false
    nameFilters: [ "Image files (*.png *.jpg)" ]
    sidebarVisible: true
    onAccepted: function() {
       console.log("FileDialog - Accepted: " + root.fileUrl);
       root.fileURL = fileUrl;
    }
    onRejected: { console.log("FileDialog - Rejected") }
}
