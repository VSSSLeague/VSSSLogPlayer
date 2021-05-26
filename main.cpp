#include <QApplication>
#include <src/player/player.h>
#include <src/player/gui/playergui.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);

    Player *player = new Player("224.0.0.1", 10002, "224.0.0.1", 10003);
    PlayerGUI *window = new PlayerGUI(player);
    window->show();

    int exec = a.exec();

    window->close();

    return exec;
}
