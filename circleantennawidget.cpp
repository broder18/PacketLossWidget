#include "circleantennawidget.h"

CircleAntennaWidget::CircleAntennaWidget(QWidget *parent) : BaseChannelWidget(parent)
{
    createSummaryButton();
    createChannelButtons();
    setFixedSize(WIDGET_SIZE, WIDGET_SIZE);
}

/*!
 * \brief CircleAntennaWidget::createChannelButtons
 * Создание кнопок переключения антенн
 */
void CircleAntennaWidget::createChannelButtons()
{
    m_channelButtons.clear();

   int radius = 90;

   for (int i = 0; i < CHANNEL_COUNT; ++i) {
       double angle = M_PI / 2.0 - i * (2.0 * M_PI / CHANNEL_COUNT);
       int x = CENTER_X + radius * cos(angle) - BUTTON_SIZE / 2;
       int y = CENTER_Y - radius * sin(angle) - BUTTON_SIZE / 2;

       QPushButton *btn = new QPushButton(QString("Ch%1").arg(i), this);
       btn->setGeometry(x, y, BUTTON_SIZE, BUTTON_SIZE);

       // Круглые кнопки
       btn->setStyleSheet(R"(
           QPushButton {
               border-radius: 12px;
               min-width: 24px;
               min-height: 24px;
               max-width: 24px;
               max-height: 24px;
           }
       )");

       connect(btn, &QPushButton::clicked, this, [this, i]() {
           setActiveChannel(i);
       });

       m_channelButtons.append(btn);
   }

   for (int i = 0; i < CHANNEL_COUNT; ++i) {
       applyButtonStyle(m_channelButtons[i], i, m_channelValues[i], i == m_activeChannel);
   }
}

/*!
 * \brief CircleAntennaWidget::createSummaryButton
 * Создание кнопки переключения режимов
 */
void CircleAntennaWidget::createSummaryButton()
{
    int centerBtnX = CENTER_X - BUTTON_SIZE / 2;
    int centerBtnY = CENTER_Y - BUTTON_SIZE / 2;

    QPushButton *centerBtn = new QPushButton("...", this);
    centerBtn->setGeometry(centerBtnX, centerBtnY, BUTTON_SIZE, BUTTON_SIZE);

    connect(centerBtn, &QPushButton::clicked, this, &BaseChannelWidget::toggleRandomMode);
}

/*!
 * \brief CircleAntennaWidget::updateButtonStyle
 * \param button
 * \param channel
 * \param value
 * Обеспечивает дополнение под основной стиль
 */
void CircleAntennaWidget::updateButtonStyle(QPushButton* button, int channel, int value)
{
    // Обеспечиваем круглую форму
   QString extraStyle = R"(
       QPushButton {
           border-radius: 12px;
           min-width: 24px;
           min-height: 24px;
           max-width: 24px;
           max-height: 24px;
       }
   )";

   QString currentStyle = button->styleSheet();
   if (!currentStyle.contains("border-radius: 12px")) {
       button->setStyleSheet(currentStyle + extraStyle);
       }
}

/*!
 * \brief CircleV1::getDefaultButtonStyle
 * \param channel - номер канала
 * \param isActive - флаг активности канала
 * \return QString - CSS стиль по умолчанию для кнопки
 * Возвращает стандартный стиль для кнопки в обычном режиме (не режим чисел)
 * Для CircleV1 все кнопки одинаковые (в отличие от VerticalV1 с Ch0)
 */
QString CircleAntennaWidget::getDefaultButtonStyle(int channel, bool isActive) const
{
    Q_UNUSED(channel);

   QString borderStyle = isActive ? "border: 2px solid #027fd9;" : "border: 1px solid #094065;";

   return QString(R"(
       QPushButton {
           color: rgb(0, 0, 0);
           background-color: #33FF99;
           %1
           border-radius: 12px;
           min-width: 24px;
           min-height: 24px;
           max-width: 24px;
           max-height: 24px;
           font-weight: bold;
           font-size: 8pt;
       }
       QPushButton:hover {
           background-color: #66FFB3;
       }
       QPushButton:pressed {
           background-color: #00CC66;
       }
   )").arg(borderStyle);
}

void CircleAntennaWidget::applyButtonStyle(QPushButton *button, int channel, int value, bool isActive)
{
    // Получаем цвета на основе значения потерь (всегда, независимо от режима)
   QString colors = getColorByValue(value);
   QStringList colorList = colors.split(';');

   QString borderStyle = isActive ? "border: 2px solid #027fd9;" : "border: 1px solid #094065;";

   // Цвета всегда меняются в зависимости от value
   QString style = QString(R"(
       QPushButton {
           color: rgb(0, 0, 0);
           background-color: %1;
           %2
           border-radius: 12px;
           min-width: 24px;
           min-height: 24px;
           max-width: 24px;
           max-height: 24px;
           font-weight: bold;
           font-size: 8pt;
       }
       QPushButton:hover {
           background-color: %3;
       }
       QPushButton:pressed {
           background-color: %4;
       }
   )").arg(colorList[0], borderStyle, colorList[1], colorList[2]);

   button->setStyleSheet(style);
}
