#ifndef _SSL_SERVER_
#define _SSL_SERVER_


// Network
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>

// Interfaces
#include "../interfaces/I_SslServer.h"


class SslServer : public I_SslServer, public QObject
{
    Q_OBJECT

    public:

        SslServer();

       ~SslServer();

    private:


};


#endif // _SSL_SERVER_
