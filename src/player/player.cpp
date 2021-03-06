#include "player.h"

Player::Player(QString visionAddress, quint16 visionPort, QString refereeAddress, quint16 refereePort, QString fileName) {
    // Set objects pointers to null
    _file = nullptr;
    _visionSocket = nullptr;
    _refereeSocket = nullptr;

    // Set default values
    _pauseEnabled = true;
    _actualFrame = 0;

    // Set vars
    _visionAddress = visionAddress;
    _visionPort = visionPort;
    _refereeAddress = refereeAddress;
    _refereePort = refereePort;

    // Check if fileName != ""
    if(fileName != "") {
        if(!setFileName(fileName)) {
            return ;
        }
    }

    // Connect to networks
    connectToRefereeNetwork();
    connectToVisionNetwork();
}

Player::~Player() {
    if(_file != nullptr) {
        // Closing file
        if(_file->isOpen()) {
            _file->close();
        }

        // Delete it
        delete _file;
    }

    if(_visionSocket != nullptr) {
        // Closing socket
        if(_visionSocket->isOpen()) {
            _visionSocket->close();
        }

        // Delete it
        delete _visionSocket;

        std::cout << Text::cyan("[VISION] " , true) + Text::bold("Client finished.") + '\n';
    }

    if(_refereeSocket != nullptr) {
        // Closing socket
        if(_refereeSocket->isOpen()) {
            _refereeSocket->close();
        }

        // Delete it
        delete _refereeSocket;

        std::cout << Text::cyan("[REFEREE] " , true) + Text::bold("Client finished.") + '\n';
    }
}

bool Player::canReadFile() {
    if(_file == nullptr) {
        return false;
    }

    return _file->isOpen();
}

qint64 Player::maxTimeStamp() {
    _dataMutex.lock();
    QList<qint64> timeStamps = _datagramsMapping.uniqueKeys();
    _dataMutex.unlock();

    if(timeStamps.isEmpty()) {
        return 0;
    }

    return (timeStamps.last() - timeStamps.first());
}

void Player::reproducePacket(Frame frame) {
    switch (frame._messageType) {
        case MESSAGE_REFEREE:{
            _refereeSocket->writeDatagram(frame._data);
        } break;
        case MESSAGE_VISION:{
            _visionSocket->writeDatagram(frame._data);
        } break;
    }
}

bool Player::setFileName(QString fileName) {
    _fileName = fileName;

    // Take file info
    QFileInfo fi(_fileName);

    // Check if is '.log' extension
    if(fi.completeSuffix() == "log") {
        openFile();
        return true;
    }
    else {
        std::cout << Text::cyan("[PLAYER] ", true) + Text::red("Error opening file '" + _fileName.toStdString() + "', the file has an wrong format.", true) + '\n';
        return false;
    }
}

void Player::setFrame(qint64 timeValue) {
    _dataMutex.lock();
    QList<qint64> timeStamps = _datagramsMapping.uniqueKeys();
    qint64 frameReference = timeValue + timeStamps.at(0);

    QList<qint64>::iterator lowBound;
    lowBound = std::lower_bound(timeStamps.begin(), timeStamps.end(), frameReference);

    _actualFrame = (lowBound - timeStamps.begin());

    QList<Frame> element = _datagramsMapping.values(timeStamps.at(_actualFrame));
    _dataMutex.unlock();

    for (const auto& f: element) {
        reproducePacket(f);
    }
    emit sendTimeStamp(timeStamps.at(_actualFrame) - timeStamps.first());
}

void Player::run() {
    while(!_pauseEnabled) {
        _dataMutex.lock();
        QList<qint64> timeStamps = _datagramsMapping.uniqueKeys();
        QList<Frame> element = _datagramsMapping.values(timeStamps.at(_actualFrame));
        _dataMutex.unlock();

        for (const auto& f: element) {
            reproducePacket(f);
        }

        emit sendTimeStamp(timeStamps.at(_actualFrame) - timeStamps.first());

        if(_actualFrame == timeStamps.size() - 1) {
            _dataMutex.lock();
            QList<Frame> element = _datagramsMapping.values(timeStamps.at(_actualFrame + 1));
            _dataMutex.unlock();
            for (const auto& f: element) {
                reproducePacket(f);
            }
            _pauseEnabled = true;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeStamps.at(_actualFrame + 1) - timeStamps.at(_actualFrame)));
            _actualFrame++;
        }
    }
}

void Player::loadFile() {
    // Erase old packets
    _dataMutex.lock();

    _datagramsMapping.clear();

    while(true) {
        // Create frame pointer
        Frame packet;
        qint64 timeStamp;

        // Create DataStream
        QDataStream stream(_file);

        if(_file == nullptr || !_file->isReadable() || stream.atEnd()) {
            break;
        }

        // Read datagram
        readDatagram(packet._messageType, packet._data, timeStamp);

        // Push it to list
        _datagramsMapping.insert(timeStamp, packet);
    }
    _dataMutex.unlock();

    emit updateSliderRange();
}

void Player::openFile() {
    // If file exists, close and delete it
    if(_file != nullptr) {
        _file->close();
        delete _file;
    }

    // Create pointer
    _file = new QFile(_fileName);

    // Open file
    if(!_file->open(QIODevice::ReadOnly)) {
        std::cout << Text::cyan("[PLAYER] ", true) + Text::red("Error opening file '" + _fileName.toStdString() + "'", true) + '\n';
        return ;
    }

    // Set actual frame to 0
    _actualFrame = 0;
    _pauseEnabled = true;

    // Load file
    loadFile();
}

void Player::connectToVisionNetwork() {
    // Create socket pointer
    _visionSocket = new QUdpSocket();

    // Connect socket to host
    _visionSocket->connectToHost(_visionAddress, _visionPort, QIODevice::WriteOnly, QAbstractSocket::IPv4Protocol);

    // Debug output
    std::cout << Text::cyan("[VISION] ", true) + Text::bold("Started at address '" + _visionAddress.toStdString() + "' and port '" + std::to_string(_visionPort) + "'.") + '\n';
}

void Player::connectToRefereeNetwork() {
    // Create socket pointer
    _refereeSocket = new QUdpSocket();

    // Connect socket to host
    _refereeSocket->connectToHost(_refereeAddress, _refereePort, QIODevice::WriteOnly, QAbstractSocket::IPv4Protocol);

    // Debug output
    std::cout << Text::cyan("[REFEREE] ", true) + Text::bold("Started at address '" + _refereeAddress.toStdString() + "' and port '" + std::to_string(_refereePort) + "'.") + '\n';
}

void Player::readDatagram(MessageType &messageType, QByteArray &data, qint64 &timeStamp) {
    QDataStream stream(_file);

    // Read from DataStream
    stream >> timeStamp;
    quint32 msgType;
    stream >> msgType;
    messageType = MessageType(msgType);
    stream >> data;
}

void Player::playExecution() {
    _pauseEnabled = false;
}

void Player::pauseExecution() {
    _pauseEnabled = true;
}

bool Player::completedExecution() {
    if(_file == nullptr) {
        return true;
    }

    return !_file->isReadable();
}

QList<Frame> Player::takeFrames(qint64 timeStamp) {
    _dataMutex.lock();
    QList<qint64> timeStamps = _datagramsMapping.uniqueKeys();
    qint64 frameReference = timeStamp + timeStamps.at(0);

    QList<qint64>::iterator lowBound;
    lowBound = std::lower_bound(timeStamps.begin(), timeStamps.end(), frameReference);

    QList<Frame> element = _datagramsMapping.values(timeStamps.at(lowBound - timeStamps.begin()));
    _dataMutex.unlock();

    return element;
}
