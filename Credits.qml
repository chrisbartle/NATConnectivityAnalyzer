import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
            Layout.alignment: Qt.AlignHCenter
            text: "Back"
            onClicked: {
                mainStackView.pop()
            }
        }
    }
}
