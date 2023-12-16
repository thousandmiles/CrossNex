import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "QML Window Resize Example"

    // 主页面布局
    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.15
            Layout.preferredHeight: parent.height
            color: "lightblue"
            Text {
                anchors.centerIn: parent
                text: "Big Content"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.75
            Layout.preferredHeight: parent.height
            color: "lightgreen"
            Text {
                anchors.centerIn: parent
                text: "Small Content"
            }
        }
    }
}
