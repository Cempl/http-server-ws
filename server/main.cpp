#include <QCoreApplication>

//
#include "engine/core/SslServer.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerConfig config;

    config.pathToKey = "127.0.0.1.key";
    config.pathToCertificate = "127.0.0.1.pem";
    config.port = 8080;
    config.address = "127.0.0.1";
    config.algorithm = QSsl::Rsa;

    SslServer server(config);

    auto func = [](QTcpSocket* socket){

            qDebug() << socket->readAll();

            socket->write("HTTP/1.1 200 OK\nContent-Type: text/html;\r\n\r\n<!DOCTYPE html> <html><body>  <h1>Server says Hello!</h1> </body></html>");

            socket->close();

    };

    QObject::connect(&server, &SslServer::Ready, func);

    return a.exec();
}

