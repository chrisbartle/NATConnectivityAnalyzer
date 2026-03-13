import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    ColumnLayout {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent
        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: qsTr("This is some
     long text
    to see what happens"
                       )
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