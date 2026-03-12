import QtQuick
import QtQuick.Controls

Button {
    id: control

    // Define a ceiling and floor for the font size
    property int maxFontSize: 24
    property int minFontSize: 8

    contentItem: Text {
        text: control.text
        font: control.font

        // Ensure the font size starts at our maximum
        font.pixelSize: control.maxFontSize

        // Scaling logic
        fontSizeMode: Text.Fit
        minimumPixelSize: control.minFontSize

        // Alignment and color
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: control.enabled ? (control.down ? "#111" : "#333") : "#888"

        // Optional: Adding a bit of padding so text doesn't touch the edges
        leftPadding: 5
        rightPadding: 5
    }
}
