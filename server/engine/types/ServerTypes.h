#ifndef _SERVER_TYPES_
#define _SERVER_TYPES_


// Network
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QString>


struct ServerConfig
{
    QString pathToKey;
    QString pathToCertificate;
    QString address;
    QSsl::KeyAlgorithm algorithm;
    qint16 port;
};


#endif // _SERVER_TYPES_

