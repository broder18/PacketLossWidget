#ifndef CIRCLEANTENNAWIDGET_H
#define CIRCLEANTENNAWIDGET_H

#include "basechannelwidget.h"

class CircleAntennaWidget : public BaseChannelWidget
{
    Q_OBJECT

    /*static QString baseButtonStyle(int size) {
        return QString(R"(
            QPushButton {
                border-radius: %1px;
            }
        )").arg(size / 2);
    }*/

public:
   explicit CircleAntennaWidget(QWidget *parent = nullptr);

protected:
   void createChannelButtons() override;
   void createSummaryButton() override;
   void updateButtonStyle(QPushButton* button, int channel, int value) override;
   QString getDefaultButtonStyle(int channel, bool isActive) const override;
   void applyButtonStyle(QPushButton* button, int channel, int value, bool isActive) override;

private:
   static constexpr int WIDGET_SIZE = 400;
   static constexpr int CENTER_X = 200;
   static constexpr int CENTER_Y = 200;
   static constexpr int RADIUS = 150;
   static constexpr int BUTTON_SIZE = 24;
};

#endif // CIRCLEANTENNAWIDGET_H
