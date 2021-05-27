#ifndef PLAYER_H
#define PLAYER_H

#include <QThread>
#include <QFile>
#include <QMutex>
#include <QUdpSocket>
#include <QDataStream>
#include <QNetworkDatagram>

#include <thread>
#include <src/utils/text/text.h>
#include <src/utils/timer/timer.h>
#include <src/utils/types/messagetype/messagetype.h>

typedef struct {
    MessageType _messageType;
    QByteArray _data;
} Frame;

class Player : public QThread
{
    Q_OBJECT
public:
    Player(QString visionAddress, quint16 visionPort, QString refereeAddress, quint16 refereePort, QString fileName = "");
    ~Player();

    // File management
    void setFileName(QString fileName);
    bool canReadFile();
    qint64 maxTimeStamp();

    // Execution control
    void reproducePacket(Frame frame);
    void setFrame(qint64 timeValue);
    void playExecution();
    void pauseExecution();
    bool completedExecution();

    //
    Frame takeFrame(qint64 timeStamp);

private:
    // QThread inherit
    void run();

    // File
    QFile *_file;
    QString _fileName;
    void openFile();
    void loadFile();
    void readDatagram(MessageType &messageType, QByteArray &data, qint64 &timeStamp);

    // Frames
    QMap<qint64, Frame> _datagramsMapping;

    // Vision network
    QUdpSocket *_visionSocket;
    QString _visionAddress;
    quint16 _visionPort;
    void connectToVisionNetwork();

    // Referee network
    QUdpSocket *_refereeSocket;
    QString _refereeAddress;
    quint16 _refereePort;
    void connectToRefereeNetwork();

    // Timestamp internal control
    QMutex _dataMutex;
    bool _pauseEnabled;
    int _actualFrame;

signals:
    void sendTimeStamp(qint64 timeStamp);
};

#endif // PLAYER_H
