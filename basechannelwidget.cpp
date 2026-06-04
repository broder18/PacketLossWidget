#include "basechannelwidget.h"

BaseChannelWidget::BaseChannelWidget(QWidget *parent) :
    QWidget(parent),
    m_isNumberMode(false),
    m_isNumberSetted(false),
    m_activeChannel(-1)
{
    m_channelValues.fill(0, CHANNEL_COUNT);
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(1000);
    connect(m_updateTimer, &QTimer::timeout, this, &BaseChannelWidget::onTimeout);
    m_updateTimer->start();
}

/*!
 * \brief BaseChannelWidget::setActiveChannel
 * \param channel - номер активного канала
 * Устанавливает активный канал и обновляет стили всех кнопок
 */
void BaseChannelWidget::setActiveChannel(int channel)
{
    if (channel < 0 || channel >= CHANNEL_COUNT)
        return;

    m_activeChannel = channel;

    // Обновляем стили всех кнопок с их текущими значениями
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        if (i < m_channelButtons.size()) {
            applyButtonStyle(m_channelButtons[i], i, m_channelValues[i], i == m_activeChannel);
        }
    }
}

/*!
 * \brief BaseChannelWidget::toggleRandomMode
 * Переключает режим отображения (ChX <-> числа потерь)
 */
void BaseChannelWidget::toggleRandomMode()
{
    m_isNumberMode = !m_isNumberMode;
    generateRandomValues();

    if (!m_isNumberMode)
        restoreChannelNames();
}

/*!
 * \brief BaseChannelWidget::generateRandomValues
 * Генерирует случайные значения потерь пакетов для всех каналов
 */
void BaseChannelWidget::generateRandomValues()
{
    QRandomGenerator generator(QDateTime::currentMSecsSinceEpoch());

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        m_channelValues[i] = generator.bounded(MAX_PACKET_LOSS);

        if (i < m_channelButtons.size()) {
            QPushButton* button = m_channelButtons[i];

            if (m_isNumberMode) {
                button->setText(QString::number(m_channelValues[i]));
            }

            applyButtonStyle(button, i, m_channelValues[i], i == m_activeChannel);
        }
    }

    m_isNumberSetted = true;
}

/*!
 * \brief BaseChannelWidget::restoreChannelNames
 * Восстанавливает названия каналов (Ch0-Ch8) на кнопках
 */
void BaseChannelWidget::restoreChannelNames()
{
    if (!m_isNumberSetted)
        return;

    for (int i = 0; i < m_channelButtons.size(); ++i) {
        QPushButton* button = m_channelButtons[i];
        button->setText(QString("Ch%1").arg(i));
        applyButtonStyle(button, i, m_channelValues[i], i == m_activeChannel);
    }

    m_isNumberSetted = false;
}

/*!
 * \brief BaseChannelWidget::getColorByValue
 * \param value - значение потерь пакетов
 * \return QString - CSS цвета в формате "mainColor;hoverColor;pressedColor"
 * Возвращает цвета в зависимости от значения потерь:
 * - 0: зеленый (отлично)
 * - 1-10: желтый (средне)
 * - >10: красный (плохо)
 */
QString BaseChannelWidget::getColorByValue(int value) const
{
    if (value == 0) {
        return "#33FF99;#66FFB3;#00CC66";
    } else if (value <= 10) {
        return "#e8ff4f;#f7f774;#adbd46";
    } else {
        return "#ff4f4f;#f78686;#bd3131";
    }
}

