#include <QCoreApplication>

//
#include "engine/core/SslServer.h"
#include "engine/core/Transport.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerConfig config;

    config.pathToKey = "127.0.0.1.key";
    config.pathToCertificate = "127.0.0.1.pem";
    config.port = 8080;
    config.address = "127.0.0.1";
    config.algorithm = QSsl::Rsa;

    Transport trs(config);

    return a.exec();
}

