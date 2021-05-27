#ifndef PLAYERGUI_H
#define PLAYERGUI_H

#include <QStyleFactory>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include <QFileDialog>

#include <src/player/player.h>
#include <src/player/gui/fieldview/fieldview.h>
#include <include/vssref_command.pb.h>

namespace Ui {
class PlayerGUI;
}

class PlayerGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerGUI(Player *player, QWidget *parent = nullptr);
    ~PlayerGUI();

private:
    Ui::PlayerGUI *ui;

    // Player
    Player *_player;

    // Video
    QVBoxLayout *_videoMainLayout;
    QHBoxLayout *_videoControlsLayout;
    FieldView *_videoWidget;
    QSlider *_videoSlider;
    QPushButton *_playButton;
    QPushButton *_pauseButton;
    QPushButton *_stopButton;
    QLabel *_videoTimeLabel;

    // Terminal
    QVBoxLayout *_terminalMainLayout;
    QTextEdit *_terminal;

    void setDarkTheme();
    void loadWidgets();
    void loadTime(qint64 currentTime, qint64 totalTime);

public slots:
    void updateSlider(qint64 timeStamp);
    void loadFile();
};

#endif // PLAYERGUI_H
