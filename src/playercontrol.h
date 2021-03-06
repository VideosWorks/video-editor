#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <QWidget>
#include <QEvent>
#include <QMediaPlayer>

class QSlider;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;

/**
 * @brief The PlayerControl class
 * Class to manage the VideoPlayer::mediaPlayer with connection between object
 */
class PlayerControl : public QWidget
{
    Q_OBJECT
public:
    PlayerControl(QWidget *parent = Q_NULLPTR);
    ~PlayerControl();
    int getVolume();
    void enablePlayerControl(bool enabled);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void volumeChanged(int vol);
    void positionChanged(qint64 pos);
    void playClicked();
    void stopClicked();
    void pauseClicked();

public slots:
    void playerStateChanged(QMediaPlayer::State state);
    void updateDuration(qint64 duration);
    void updateCursorPosition(qint64 position);
    void emitVolumeChanged(int vol);
    void emitPositionChanged(int pos);
    void emitPlayPauseClicked();
    void emitStopClicked();

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *hTopLayout;
    QHBoxLayout *hBottomLayout;

    QSlider *positionSlider;
    QSlider *volumeSlider;

    QLabel *lblPosition;
    QLabel *lblVolume;

    QToolButton *btnPlayPause;
    QToolButton *btnStop;

};

#endif // PLAYERCONTROL_H
