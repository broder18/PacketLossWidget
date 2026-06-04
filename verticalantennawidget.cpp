#include "verticalantennawidget.h"

/*!
 * \brief VerticalAntennaWidget::VerticalAntennaWidget
 * \param parent - родительский виджет
 *
 * Конструктор создает кнопки и настраивает фиксированный размер виджета
 */
VerticalAntennaWidget::VerticalAntennaWidget(QWidget *parent) :
    BaseChannelWidget(parent),
    m_separator(nullptr)
{
    createSummaryButton();
    createChannelButtons();
    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
}

/*!
 * \brief VerticalAntennaWidget::createChannelButtons
 * Создание вертикально расположенных кнопок каналов
 *
 * Особенности размещения:
 * - Группа кнопок центрируется по вертикали
 * - Ch0 неактивен, после него рисуется разделительная линия
 * - Ch1-Ch8 интерактивны и имеют отступы между собой
 */
void VerticalAntennaWidget::createChannelButtons()
{
    m_channelButtons.clear();

    const int buttonHeight = ANTENNA_BUTTON_HEIGHT;
    const int normalSpacing = 8;      // обычное расстояние между кнопками
    const int separatorSpacing = 15;  // увеличенное расстояние после Ch0

    int currentY = CENTER_Y;

    // Вычисление общей высоты для центрирования группы
    int totalHeight = buttonHeight +                    // Ch0
                      separatorSpacing +                // большой отступ
                      8 * buttonHeight +                // Ch1–Ch8
                      7 * normalSpacing;                // отступы между Ch1–Ch8

    currentY -= totalHeight / 2; // центрирование группы по вертикали

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        int x = CENTER_X - ANTENNA_BUTTON_WIDTH / 2;
        int y = currentY;

        QPushButton *btn = new QPushButton(QString("Ch%1").arg(i), this);
        btn->setGeometry(x, y, ANTENNA_BUTTON_WIDTH, buttonHeight);

        if (i == 0) {
            // Кнопка Ch0 - неактивная, отображает общую информацию
            btn->setStyleSheet(R"(
                QPushButton {
                    color: rgba(0, 0, 0, 150);
                    background-color: rgba(51, 255, 153, 100);
                    border: 1px solid rgba(9, 64, 101, 120);
                    border-radius: 3px;
                    min-width: 36px;
                    min-height: 16px;
                    max-width: 36px;
                    max-height: 16px;
                    font-weight: bold;
                    font-size: 9pt;
                }
            )");
            btn->setEnabled(false);  // Ch0 нельзя выбрать
            m_channelButtons.append(btn);

            // Создание разделительной линии после Ch0
            int separatorY = y + buttonHeight + (separatorSpacing / 2);
            m_separator = new QFrame(this);
            m_separator->setFrameShape(QFrame::HLine);
            m_separator->setFrameShadow(QFrame::Sunken);
            m_separator->setLineWidth(1);
            m_separator->setStyleSheet("background-color: #094065;");
            m_separator->setGeometry(x - 3, separatorY, ANTENNA_BUTTON_WIDTH + 8, 1);
            m_separator->show();

            currentY += buttonHeight + separatorSpacing;
        } else {
            // Ch1-Ch8 - интерактивные кнопки выбора каналов
            // Базовый стиль (цвета будут обновлены через applyButtonStyle)
            btn->setStyleSheet(R"(
                QPushButton {
                    border-radius: 3px;
                    min-width: 36px;
                    min-height: 16px;
                    max-width: 36px;
                    max-height: 16px;
                }
            )");

            connect(btn, &QPushButton::clicked, this, [this, i]() {
                setActiveChannel(i);
            });
            m_channelButtons.append(btn);

            currentY += buttonHeight + normalSpacing;
        }
    }

    // Применяем начальные стили ко всем кнопкам (кроме Ch0, у него свой стиль)
    for (int i = 1; i < CHANNEL_COUNT; ++i) {
        if (i < m_channelButtons.size()) {
            applyButtonStyle(m_channelButtons[i], i, m_channelValues[i], i == m_activeChannel);
        }
    }
}

/*!
 * \brief VerticalAntennaWidget::createSummaryButton
 * Создание кнопки переключения режимов отображения
 * Кнопка располагается внизу, под всеми кнопками каналов
 */
void VerticalAntennaWidget::createSummaryButton()
{
    // Вычисляем положение последней кнопки
    int startY = CENTER_Y - (9 * ANTENNA_BUTTON_HEIGHT) / 2;  // верхняя кнопка (Ch1)
    int lastButtonY = startY + 9 * ANTENNA_BUTTON_HEIGHT;     // нижняя из 8 активных кнопок

    // Кнопка переключения опускается ниже кнопок выбора каналов
    int summaryBtnY = lastButtonY + 2 * ANTENNA_BUTTON_HEIGHT + 10; // 10 - отступ от последней кнопки
    int summaryBtnX = CENTER_X - SUMMARY_BUTTON_SIZE / 2;

    QPushButton *centerBtn = new QPushButton("...", this);
    centerBtn->setStyleSheet(R"(
        QPushButton {
            color: rgb(0, 0, 0);
            background-color: transparent;
            border: 1px solid #094065;
            min-width: 18px;
            min-height: 18px;
            max-width: 18px;
            max-height: 18px;
            font-weight: bold;
            font-size: 8pt;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #FFB366;
        }
        QPushButton:pressed {
            background-color: #CC6600;
        }
    )");

    centerBtn->setGeometry(summaryBtnX, summaryBtnY, SUMMARY_BUTTON_SIZE, SUMMARY_BUTTON_SIZE);
    centerBtn->show();

    // Подключаем кнопку к переключению режима
    connect(centerBtn, &QPushButton::clicked, this, &VerticalAntennaWidget::toggleRandomMode);
}

/*!
 * \brief VerticalAntennaWidget::updateButtonStyle
 * \param button - указатель на кнопку
 * \param channel - номер канала
 * \param value - значение потерь пакетов
 * Дополнительная стилизация кнопки для вертикального расположения
 * Добавляет специфичные для VerticalV1 параметры (размеры, скругление)
 */
void VerticalAntennaWidget::updateButtonStyle(QPushButton* button, int channel, int value)
{
    // Ch0 имеет свой фиксированный стиль, не обновляем его
    if (channel == 0) {
        return;
    }

    // Для Ch1-Ch8 добавляем специфичные размеры и скругление
    QString extraStyle = QString(R"(
        QPushButton {
            border-radius: 3px;
            min-width: 36px;
            min-height: 16px;
            max-width: 36px;
            max-height: 16px;
            font-size: 8pt;
        }
    )");

    // Добавляем стиль, не перезаписывая существующий (сохраняем цвета)
    QString currentStyle = button->styleSheet();
    if (!currentStyle.contains("border-radius: 3px")) {
        button->setStyleSheet(currentStyle + extraStyle);
    }
}

/*!
 * \brief VerticalAntennaWidget::getDefaultButtonStyle
 * \param channel - номер канала
 * \param isActive - флаг активности канала
 * \return QString - CSS стиль по умолчанию для кнопки
 * Возвращает стандартный стиль для кнопки в обычном режиме (не режим чисел)
 */
QString VerticalAntennaWidget::getDefaultButtonStyle(int channel, bool isActive) const
{
    if (channel == 0) {
        return R"(
            QPushButton {
                color: rgba(0, 0, 0, 150);
                background-color: rgba(51, 255, 153, 100);
                border: 1px solid rgba(9, 64, 101, 120);
                border-radius: 3px;
                min-width: 36px;
                min-height: 16px;
                max-width: 36px;
                max-height: 16px;
                font-weight: bold;
                font-size: 9pt;
            }
        )";
    }

    QString borderStyle = isActive ? "border: 2px solid #027fd9;" : "border: 1px solid #094065;";

    return QString(R"(
        QPushButton {
            color: rgb(0, 0, 0);
            background-color: #33FF99;
            %1
            border-radius: 3px;
            min-width: 36px;
            min-height: 16px;
            max-width: 36px;
            max-height: 16px;
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

/*!
 * \brief VerticalAntennaWidget::applyButtonStyle
 * \param button
 * \param channel
 * \param value
 * \param isActive
 * Изменение стиля кнопки. Вызывается каждый раз когда получаем новы данные.
 */
void VerticalAntennaWidget::applyButtonStyle(QPushButton *button, int channel, int value, bool isActive)
{
    if (channel == 0) {
       button->setStyleSheet(getDefaultButtonStyle(channel, isActive));
       return;
   }

   QString colors = getColorByValue(value);
   QStringList colorList = colors.split(';');

   QString borderStyle = isActive ? "border: 2px solid #027fd9;" : "border: 1px solid #094065;";

   QString style = QString(R"(
       QPushButton {
           color: rgb(0, 0, 0);
           background-color: %1;
           %2
           border-radius: 3px;
           min-width: 36px;
           min-height: 16px;
           max-width: 36px;
           max-height: 16px;
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
