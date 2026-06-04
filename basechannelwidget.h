#ifndef BASECHANNELSWIDGET_H
#define BASECHANNELSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>

class BaseChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseChannelWidget(QWidget *parent = nullptr);
    virtual ~BaseChannelWidget() = default;

    void setActiveChannel(int channel);
    void toggleRandomMode();
    void generateRandomValues();
    void restoreChannelNames();

protected:
    virtual void createChannelButtons() = 0;
    virtual void createSummaryButton() = 0;
    virtual void updateButtonStyle(QPushButton* button, int channel, int value) = 0;
    virtual QString getDefaultButtonStyle(int channel, bool isActive)  const = 0;

    QString getColorByValue(int value) const;
    virtual void applyButtonStyle(QPushButton* button, int channel, int value, bool isActive) = 0;

    static constexpr int CHANNEL_COUNT = 9;
    static constexpr int MAX_PACKET_LOSS = 31;

    QVector<QPushButton*> m_channelButtons;
    QVector<int> m_channelValues;      // Храним значения для каждого канала
    QTimer* m_updateTimer;

    bool m_isNumberMode;      // true = показываем числа потерь, false = показываем ChX
    bool m_isNumberSetted;    // Флаг, что числа были установлены
    int m_activeChannel;      // Активный канал (-1 = нет активного)

private slots:
    void onTimeout() {generateRandomValues(); }
};

#endif // BASECHANNELSWIDGET_H
