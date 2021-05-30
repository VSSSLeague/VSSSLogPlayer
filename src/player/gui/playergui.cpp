#include "playergui.h"
#include "ui_playergui.h"

#include <iostream>

PlayerGUI::PlayerGUI(Player *player, QWidget *parent) : QMainWindow(parent), ui(new Ui::PlayerGUI) {
    // Setup UI
    ui->setupUi(this);

    // Setup player
    _player = player;

    // Load widgets
    loadWidgets();

    // Set dark theme
    setDarkTheme();

    // Connect player change timeStamp
    QObject::connect(_player, &Player::sendTimeStamp, this, &PlayerGUI::updateSlider);
    QObject::connect(_player, &Player::updateSliderRange, this, &PlayerGUI::updateSliderRange);

    // Connect action to loadFile
    QObject::connect(ui->actionLoad_log_file, &QAction::triggered, this, &PlayerGUI::loadFile);
}

PlayerGUI::~PlayerGUI() {
    delete ui;
}

void PlayerGUI::setDarkTheme() {
    this->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
    darkPalette.setColor(QPalette::Base,QColor(42,42,42));
    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
    darkPalette.setColor(QPalette::Button,QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,QColor(42,130,218));
    darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    darkPalette.setColor(QPalette::HighlightedText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));

    this->setPalette(darkPalette);
}

void PlayerGUI::loadWidgets() {
    /// Terminal frame
    // Set terminal frame layout
    ui->frame_2->setLayout(_terminalMainLayout = new QVBoxLayout());

    // Add terminal widget
    _terminalMainLayout->addWidget(_terminal = new QTextEdit());
    _terminal->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _terminal->setReadOnly(true);
    _terminal->setStyleSheet("color: green; background-color: black;");

    /// Video frame
    // Set video frame layout
    ui->frame->setLayout(_videoMainLayout = new QVBoxLayout());

    // Add video widget
    _videoMainLayout->addWidget(_videoWidget = new FieldView());
    _videoWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Creating video controls layout
    _videoMainLayout->addLayout(_videoControlsLayout = new QHBoxLayout());

    // Adding buttons, connecting and setting icons
    _videoControlsLayout->addWidget(_playButton = new QPushButton());
    QObject::connect(_playButton, &QPushButton::released, _player, [this](){
        if(!_player->canReadFile()) {
            _terminal->insertPlainText("Could not play log file. Have you opened one?\n");
        }
        else {
            _player->start();
            _player->playExecution();
        }
    });

    _videoControlsLayout->addWidget(_pauseButton = new QPushButton());
    QObject::connect(_pauseButton, &QPushButton::released, _player, [this]() {
        if(!_player->canReadFile()) {
            _terminal->insertPlainText("Could not pause log file. Have you opened one?\n");
        }
        else {
            _player->pauseExecution();
        }
    });

    _videoControlsLayout->addWidget(_stopButton = new QPushButton());
    QObject::connect(_stopButton, &QPushButton::released, [this]() {
        if(!_player->canReadFile()) {
            _terminal->insertPlainText("Could not stop log file. Have you opened one?\n");
        }
        else {
            _player->pauseExecution();
            _player->setFrame(0);
        }
    });

    _playButton->setIcon(QIcon(":/gui/play.png"));
    _pauseButton->setIcon(QIcon(":/gui/pause.png"));
    _stopButton->setIcon(QIcon(":/gui/stop.png"));

    // Adding slider
    _videoControlsLayout->addWidget(_videoSlider = new QSlider(Qt::Orientation::Horizontal));
    QObject::connect(_videoSlider, &QSlider::sliderMoved, [this](int value) {
        if(_player->canReadFile()) {
            _videoSlider->setValue(value);
            _player->setFrame(_videoSlider->value());
        }
        else{
            _videoSlider->setValue(0);
        }
    });

    QObject::connect(_videoSlider, &QSlider::sliderPressed, [this]() {
        if(_player->canReadFile()) {
            _player->pauseExecution();
        }
    });

    // Adding video time label
    _videoControlsLayout->addWidget(_videoTimeLabel = new QLabel("00:00:00 / 00:00:00"));
}

void PlayerGUI::loadTime(qint64 currentTime, qint64 totalTime) {
    currentTime = currentTime / 1000.0;
    totalTime = totalTime / 1000.0;

    char buf[100];

    int currentHours = currentTime/3600;
    int currentMinutes = (currentTime - (currentHours * 3600))/60;
    int currentSeconds = currentTime - (currentMinutes * 60) - (currentHours * 3600);

    int totalHours = totalTime/3600;
    int totalMinutes = (totalTime - (totalHours * 3600))/60;
    int totalSeconds = totalTime - (totalMinutes * 60) - (totalHours * 3600);

    sprintf(buf, "%02d:%02d:%02d / %02d:%02d:%02d", currentHours, currentMinutes, currentSeconds,
                                               totalHours, totalMinutes, totalSeconds);

    _videoTimeLabel->setText(buf);
}

void PlayerGUI::loadFile() {
    QString fileName = QFileDialog::getOpenFileName();
    _player->setFileName(fileName);
    _terminal->insertPlainText("Opened file: " + fileName + '\n');
}

void PlayerGUI::updateSlider(qint64 timeStamp) {
    updateSliderRange();
    _videoSlider->setValue(timeStamp);

    Frame frame = _player->takeFrame(timeStamp);
    if(frame._messageType == MessageType::MESSAGE_VISION) {
        fira_message::sim_to_ref::Environment env;
        env.ParseFromArray(frame._data.data(), frame._data.size());

        _videoWidget->updateFieldObjects(env);
        _videoWidget->redraw();
    }
    else {
        VSSRef::ref_to_team::VSSRef_Command refCommand;
        refCommand.ParseFromArray(frame._data.data(), frame._data.size());

        _terminal->insertPlainText(QString("[REFEREE] Received command from referee: %1 for team %2 at quadrant %3.\n")
                                   .arg(VSSRef::Foul_Name(refCommand.foul()).c_str())
                                   .arg(VSSRef::Color_Name(refCommand.teamcolor()).c_str())
                                   .arg(VSSRef::Quadrant_Name(refCommand.foulquadrant()).c_str()));
    }

    loadTime(timeStamp, _player->maxTimeStamp());
}

void PlayerGUI::updateSliderRange() {
    _videoSlider->setRange(0, _player->maxTimeStamp());
    _videoSlider->setValue(0);

    loadTime(0, _player->maxTimeStamp());
}
