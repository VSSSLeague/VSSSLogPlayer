#include <QApplication>
#include <QCommandLineParser>
#include <src/player/player.h>
#include <src/player/gui/playergui.h>
#include <src/utils/exithandler/exithandler.h>

QCoreApplication *createApplication(int &argc, char *argv[]) {
    // Try to found in args an '--gui'
    bool foundArg = false;
    for (int i = 0; i < argc; ++i) {
        if (!qstrcmp(argv[i], "--gui")) {
            foundArg = true;
            break;
        }
    }

    // if not found, call core application
    if(!foundArg) {
        return new QCoreApplication(argc, argv);
    }
    // otherwise, call gui application
    else {
        return new QApplication(argc, argv);
    }
}

int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> a(createApplication(argc, argv));
    a->setApplicationName(APP_NAME);
    a->setApplicationVersion(APP_VERSION);

    // Setup command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // Setup application options
    // Use gui
    QCommandLineOption useGuiOption("gui", "Enable GUI mode");
    parser.addOption(useGuiOption);

    // File name
    QCommandLineOption fileName("fileName",
                                 QCoreApplication::translate("main", "Set desired log fileName (e.g.: 2020-04-01_match)"),
                                 QCoreApplication::translate("main", "fileName"));
    parser.addOption(fileName);

    // Vision network
    QCommandLineOption visionNetwork("vision",
                                 QCoreApplication::translate("main", "Set vision address:port (default is 224.0.0.1:10002)"),
                                 QCoreApplication::translate("main", "address>:<port"));
    parser.addOption(visionNetwork);

    // Referee network
    QCommandLineOption refereeNetwork("referee",
                                 QCoreApplication::translate("main", "Set referee address:port (default is 224.5.23.2:10003)"),
                                 QCoreApplication::translate("main", "address>:<port"));
    parser.addOption(refereeNetwork);

    // Process parser in application
    parser.process((*a));

    // Setup ExitHandler
    ExitHandler::setApplication(a.data());
    ExitHandler::setup();

    // Parse
    // File Name
    QString logFileName = "";
    if(parser.isSet(fileName)) {
        logFileName = parser.value(fileName);
    }

    // Vision
    QString visionAddress = "224.0.0.1";
    quint16 visionPort = 10002;
    if(parser.isSet(visionNetwork)) {
        QString visionNetworkValue = parser.value(visionNetwork);
        visionAddress = visionNetworkValue.split(":").at(0);
        visionPort = visionNetworkValue.split(":").at(1).toUInt();
    }

    // Referee
    QString refereeAddress = "224.5.23.2";
    quint16 refereePort = 10003;
    if(parser.isSet(refereeNetwork)) {
        QString refereeNetworkValue = parser.value(refereeNetwork);
        refereeAddress = refereeNetworkValue.split(":").at(0);
        refereePort = refereeNetworkValue.split(":").at(1).toUInt();
    }

    if(!parser.isSet(useGuiOption) && !parser.isSet(fileName)) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("If you don't use GUI you need to specify the file to open using --fileName argument.") + '\n';
        return 0;
    }

    Player *player = new Player(visionAddress, visionPort, refereeAddress, refereePort, logFileName);
    PlayerGUI *window = nullptr;

    if(parser.isSet(useGuiOption)) {
        window = new PlayerGUI(player);
        window->show();
    }
    else{
        player->start();
    }

    int exec = a->exec();

    // Wait for thread to terminate
    player->pauseExecution();
    if(!player->isFinished()) {
        player->terminate();
        player->wait();
    }
    delete player;

    // Close gui
    if(parser.isSet(useGuiOption)) {
        window->close();
        delete window;
    }

    return exec;
}
