#ifndef VERTICALANTENNAWIDGET_H
#define VERTICALANTENNAWIDGET_H

#include <QFrame>
#include "basechannelwidget.h"

class VerticalAntennaWidget: public BaseChannelWidget
{
    Q_OBJECT

public:
   explicit VerticalAntennaWidget(QWidget *parent = nullptr);

protected:
   void createChannelButtons() override;
   void createSummaryButton() override;
   void updateButtonStyle(QPushButton* button, int channel, int value) override;
   QString getDefaultButtonStyle(int channel, bool isActive) const override;
   void applyButtonStyle(QPushButton* button, int channel, int value, bool isActive) override;

private:
   // Константы расположения (из вашего исходного кода)
   static constexpr int WIDGET_WIDTH = 100;
   static constexpr int WIDGET_HEIGHT = 400;
   static constexpr int CENTER_X = 50;
   static constexpr int CENTER_Y = 200;
   static constexpr int ANTENNA_BUTTON_WIDTH = 36;
   static constexpr int ANTENNA_BUTTON_HEIGHT = 16;
   static constexpr int SUMMARY_BUTTON_SIZE = 18;

   // Разделительная линия между Ch0 и остальными
   QFrame* m_separator;
};

#endif // VERTICALANTENNAWIDGET_H
