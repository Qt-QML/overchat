import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0

//import IOSCamera 1.0

import "qrc:/components/singletons/."

import "./components" as Components

ApplicationWindow {
    id: root

    visible: true

    readonly property int responsiveWidth: 500
    readonly property string webFormUrl: _is_ios ? "./forms/WebFormIOS.qml" : "./forms/WebForm.qml";

    width: 720
    height: 480

    minimumWidth: 300
    minimumHeight: 400

    title: qsTr("overchat")

    Component.onCompleted: function() {
        console.info("PLATFORM isIOS", _is_ios)

        User.loginLocal(function(type, data) {
            if (type === "fail") {
                /*внутр. ошибка*/
                popup.open(root.webFormUrl);
            }
            if (type === "void") {
                popup.open(root.webFormUrl);
            }
        });
    }


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

                onListItemClicked: swipeView.currentIndex = 1
            }
        }
        Item {
            Components.Chat {
                id: chat

                onBackButtonClicked: function() {
                    swipeView.currentIndex = 0;
                    sidebar.deselect();
                }
            }
        }
    }

    PageIndicator {
        id: indicator
        count: swipeView.count
        currentIndex: swipeView.currentIndex
        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: swipeView.horizontalCenter

        opacity: 0
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

        ListElement { label: "Authentication"; onclick: function() {popup.open(root.webFormUrl);} }
        ListElement { label: "Profile"; onclick: function() {popup.open("./forms/ProfileForm.qml");} }
        ListElement { label: "Add User"; onclick: function() {popup.open("./forms/UserAddForm.qml");} }
    }

    /*
    IOSCamera {
            id: camera
            onImagePathChanged: {
                // The captured image has changed. But since the path stays the
                // same if you take several snapshots, we clear the source first
                // to force a reload.
                img.source = ""
                img.source = imagePath
            }
      }
            */
}
