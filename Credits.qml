import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    ColumnLayout {
        spacing: 20
        anchors.fill: parent

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "NAT Connectivity Analyzer"
                wrapMode: Text.Wrap
                font.bold: true
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Version " + Controller.applicationVersion
                wrapMode: Text.Wrap
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Built " + Controller.applicationBuildTimestamp
                wrapMode: Text.Wrap
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Written by Chris Bartle"
                wrapMode: Text.Wrap
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: '<html><style type="text/css"></style><a href="https://www.chrisbartle.com">www.chrisbartle.com</a></html>'
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
                wrapMode: Text.Wrap
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Stunner library written by Hitesh Sharma"
            wrapMode: Text.Wrap
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "libpcpnatpmp Copyright (c) 2013 by Cisco Systems, Inc."
                wrapMode: Text.Wrap
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: '<html><style type="text/css"></style><a href="https://github.com/libpcpnatpmp/libpcpnatpmp">github.com/libpcpnatpmp/libpcpnatpmp</a></html>'
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
                wrapMode: Text.Wrap
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "miniupnp Copyright (c) 2005-2026, Thomas BERNARD"
                wrapMode: Text.Wrap
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: '<html><style type="text/css"></style><a href="https://github.com/miniupnp/miniupnp">github.com/miniupnp/miniupnp</a></html>'
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
                wrapMode: Text.Wrap
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
