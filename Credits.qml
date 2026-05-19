import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    ColumnLayout {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
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
            }
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: '<html><style type="text/css"></style><a href="https://www.chrisbartle.com">www.chrisbartle.com</a></html>'
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Stunner library written by Hitesh Sharma"
            wrapMode: Text.Wrap
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
