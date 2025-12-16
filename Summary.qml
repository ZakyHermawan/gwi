import QtQuick

Item {
    Text {
        text: {
            if(dataManager) {
                return dataManager.summary
            }
            return ""
        }

        font.pointSize: 24
        leftPadding: 30
    }
}
