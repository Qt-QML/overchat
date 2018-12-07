import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "./components" as Components

ApplicationWindow {
    id: root

    visible: true

    readonly property int responsiveWidth: 500

    width: 720
    height: 480

    minimumWidth: 300
    minimumHeight: 400

    title: qsTr("overchat")

    SwipeView  {
        id: swipeView
        currentIndex: 0
        anchors.fill: parent
        states: [
            State {
                when: root.width >= responsiveWidth
                ParentChange { target: sidebar; parent: sidebarContainer; }
                ParentChange { target: chat; parent: chatContainer; }
                PropertyChanges { target: indicator; visible: hide }
            }
        ]
        Item {
            Components.Sidebar {
                id: sidebar

                onMenuButtonClicked: menu.opened = true
            }
        }
        Item {
            Rectangle{
                id: chat
                anchors.fill: parent
                color: "lightgray";
            }
        }
    }

    PageIndicator {
        id: indicator
        count: swipeView.count
        currentIndex: swipeView.currentIndex
        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: swipeView.horizontalCenter
    }

    Row {
        id: splitView
        anchors.fill: parent

        Item {
            id: sidebarContainer
            width: 260; height: parent.height
        }
        Item {
            id: chatContainer
            width: parent.width - 260; height: parent.height
        }
    }

    Components.PopupMenu {
        id: popup
        windowWidth: root.width
        windowHeight: root.height
    }

    Components.SlideMenu {
        id: menu

        items: menuItems
    }

    ListModel {
        id: menuItems

        ListElement { label: "Element 1"; onclick: function() {popup.open("./forms/BasicForm.qml");} }
        ListElement { label: "Element 2"; onclick: function() {} }
    }
}
