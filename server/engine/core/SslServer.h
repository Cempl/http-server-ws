#ifndef _SSL_SERVER_
#define _SSL_SERVER_


// Network
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSsl>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>

// Types
#include "../types/ServerTypes.h"


class SslServer : public QTcpServer
{
    Q_OBJECT

    public:

explicit SslServer(
                ServerConfig config,
                QObject* parent = nullptr);

       ~SslServer();


    protected:

        void incomingConnection(qintptr descriptor) override final;

    signals:

        void Ready(QTcpSocket* socket);

    private slots:

        void SslErrors(const QList<QSslError>& errors);

        void Link();

        void Disconnected();

        void ConnectIsReady();


    private:

        QSslKey mKey;
        QSslCertificate mCertificate;

};


#endif // _SSL_SERVER_
