import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    ColumnLayout {
        anchors.fill: parent

        Flickable {
            id: scrollContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: mainColumn.implicitHeight
            clip: true // Prevents text from drawing outside this area
            // This allows the user to drag/flick to scroll
            boundsBehavior: Flickable.StopAtBounds

            Item {
                id: mainColumnWrapper
                width: scrollContainer.width

                ColumnLayout {
                    id: mainColumn
                    spacing: 20
                    width: parent.width - 40   // 20px margin on each side
                    anchors.horizontalCenter: parent.horizontalCenter

                    // TOP PADDING
                    Item { Layout.preferredHeight: 10 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.fillWidth: true
                            text: "NAT Connectivity Analyzer"
                            wrapMode: Text.Wrap
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "Version " + Controller.applicationVersion
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "Built " + Controller.applicationBuildTimestamp
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.fillWidth: true
                            text: "Written by Chris Bartle"
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: '<html><style type="text/css"></style><a href="https://www.chrisbartle.com">www.chrisbartle.com</a></html>'
                            onLinkActivated: {
                                Qt.openUrlExternally(link)
                            }
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        text: "Stunner library written by Hitesh Sharma"
                        wrapMode: Text.Wrap
                        horizontalAlignment: Text.AlignHCenter
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.fillWidth: true
                            text: "libpcpnatpmp Copyright (c) 2013 by Cisco Systems, Inc."
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: '<html><style type="text/css"></style><a href="https://github.com/libpcpnatpmp/libpcpnatpmp">github.com/libpcpnatpmp/libpcpnatpmp</a></html>'
                            onLinkActivated: {
                                Qt.openUrlExternally(link)
                            }
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.fillWidth: true
                            text: "miniupnp Copyright (c) 2005-2026, Thomas BERNARD"
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: '<html><style type="text/css"></style><a href="https://github.com/miniupnp/miniupnp">github.com/miniupnp/miniupnp</a></html>'
                            onLinkActivated: {
                                Qt.openUrlExternally(link)
                            }
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }

        Button {
            text: "Back"
            onClicked: mainStackView.pop()
            font.pixelSize: 25
            height: 30
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Layout.fillWidth: true
        }
    }
}
