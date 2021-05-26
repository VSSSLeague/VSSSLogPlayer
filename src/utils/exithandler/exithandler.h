#ifndef EXITHANDLER_H
#define EXITHANDLER_H

#include <QObject>
#include <QCoreApplication>

class ExitHandler : public QObject {
private:
    static QCoreApplication *_app;
    static int _counter;

public:
    ExitHandler();
    static void setApplication(QCoreApplication *app);
    static void setup();
    static void run(int s);
};

#endif // EXITHANDLER_H
