import QtQuick 2.1
import QtQuick.Controls 1.1

import com.embedded.io 1.0

Rectangle {
    id: rect
    width: 720
    height: 720
    color: "blue"
    function getDirection(direction){
        if(direction === QGpio.In)
            return "QGpio::In";
        else if(direction === QGpio.High)
            return "QGpio::High";
        else if(direction === QGpio.Low)
            return "QGpio::Low";
        else
            return "QGpio::Out";
    }

    Connections
    {
        target: GPIO204
        onValueChanged:
        {
            pin204.color = new_value? "green":"red";
        }
    }
    Connections
    {
        target: GPIO199
        onValueChanged:
        {
              pin199.color = new_value? "green":"red";
        }
    }
    ListView{
        id:list
        model: GpioList
        anchors.fill: parent
        delegate: Rectangle{
            width: list.width
            height: 60
            color:"grey"
            border.color:"black"
            border.width: 2
            Text{
                id: textPin
                width: 200
                anchors.verticalCenter: parent.verticalCenter
                anchors.left:parent.left
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Pin Number:" + model.modelData.number
                font.pixelSize: 16
            }
            Rectangle{
                anchors.top: parent.top
                anchors.left:textPin.right
                anchors.right: parent.right
                anchors.bottom:parent.bottom
                Rectangle{
                    id: direction
                    color: "blue"
                    anchors.margins: 8
                    anchors.top: parent.top
                    anchors.left:parent.left
                    anchors.right: parent.horizontalCenter
                    anchors.bottom:parent.bottom
                    border.color:"white"
                    border.width: 1
                    Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "yellow"
                        text: ""
                        Component.onCompleted: {
                            text = getDirection(model.modelData.direction);
                        }
                    }
                }
                Rectangle{
                    id: inValue
                    color: model.modelData.value? "green":"red"
                    anchors.top: parent.top
                    anchors.left:direction.right
                    anchors.right: parent.right
                    anchors.bottom:parent.bottom
                    anchors.margins: 8
                    border.color:"white"
                    border.width: 1

                }
                Rectangle{
                    id: outValue
                    z:inValue.z+1
                    anchors.top: parent.top
                    anchors.left:direction.right
                    anchors.right: parent.right
                    anchors.bottom:parent.bottom
                    anchors.margins: 8
                    color: "red"
                    border.color:"white"
                    border.width: 1

                    Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "yellow"
                        text: "Click to Turn On"
                    }
                    MouseArea{
                        anchors.fill: parent
                        id:mouseA
                        enabled:true
                        onPressed:{
                            gpio200.value = true;
                            outValue.color = "green";
                        }
                        onReleased: {
                            gpio200.value = false;
                            outValue.color = "red";
                        }
                    }
                }
            }
            Component.onCompleted: {
                if(model.modelData.direction == QGpio.In)
                {
                    outValue.visible = false;
                    outValue.enabled = false;
                    inValue.visible = true;
                    inValue.enabled = true;
                }
                else
                {
                    outValue.visible = true;
                    outValue.enabled = true;
                    inValue.visible = false;
                    inValue.enabled = false;
                }

            }
        }
    }
    QGpio{
        id:gpio200
        number:"200";
        onValueChanged: {
            //pin200.color = (new_value? "green":"red");
        }
        Component.onCompleted: {
            direction = QGpio.In;
            edge = QGpio.Both;
            active_low = false;
            GpioWatcher.watchGpioItem(gpio200);
            list.model = GpioWatcher.exportedObjects();
        }
    }

    Rectangle{
        id: pinOuter204
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 20
        width: 100
        height: 100
        color: "lightsteelblue"
        border.color: "black"
        border.width: 2
        Text{
            anchors.horizontalCenter:  parent.horizontalCenter
            anchors.top: parent.top
            text: "GPIO204"
        }

        Rectangle{
            id: pin204
            width: 80
            height: 80
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: "red"
            border.color: "white"
            border.width: 2
            anchors.bottomMargin: 4
        }
    }
    Rectangle{
        id: pinOuter200
        anchors.left: pinOuter204.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 20
        width: 100
        height: 100
        color: "teal"
        border.color: "black"
        border.width: 2
        Text{
            anchors.horizontalCenter:  parent.horizontalCenter
            anchors.top: parent.top
            text: "GPIO200"
        }
        Rectangle{
            id: pin200
            width: 80
            height: 80
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: gpio200.value ? "green":"red"
            border.color: "white"
            border.width: 2
            anchors.bottomMargin: 4
        }

    }
    Rectangle{
        id: pinOuter199
        anchors.left: pinOuter200.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 20
        width: 100
        height: 100
        color: "yellow"
        border.color: "black"
        border.width: 2
        Text{
            anchors.horizontalCenter:  parent.horizontalCenter
            anchors.top: parent.top
            text: "GPIO199"
        }
        Rectangle{
            id: pin199
            width: 80
            height: 80
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: "red"
            border.color: "white"
            border.width: 2
            anchors.bottomMargin: 4
        }
    }
}
