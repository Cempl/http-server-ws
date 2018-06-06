#include "SslServer.h"

// Files
#include <QFile>


SslServer::SslServer(
    ServerConfig config,
    QObject *parent)
:
    QTcpServer(parent)
{
    QFile keyFile(config.pathToKey);
    if( keyFile.open(QIODevice::ReadOnly) )
    {
        mKey = QSslKey( keyFile.readAll(), config.algorithm );

        keyFile.close();
    }
    else
    {
        qDebug() << "Error open keyFile!";
    }

    QFile certFile(config.pathToCertificate);
    if( certFile.open(QIODevice::ReadOnly) )
    {
        mCertificate = QSslCertificate( certFile.readAll() );

        certFile.close();
    }
    else
    {
        qDebug() << "Error open certFile!";
    }

    if( !listen( QHostAddress(config.address), config.port ) )
    {
        qDebug() << "Unable to start the server!";

        exit(0);
    }

    connect( this, SIGNAL(newConnection()), this, SLOT(Link()) );
}

SslServer::~SslServer()
{
    qDebug() << "SslServer::~SslServer";

    deleteLater();
}

void SslServer::incomingConnection(qintptr descriptor)
{
    qDebug() << "SslServer::incomingConnection";

    QSslSocket* sslSocket = new QSslSocket( this );

    connect( sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(SslErrors(QList<QSslError>)));
    sslSocket->setSocketDescriptor(descriptor);
    sslSocket->setPrivateKey(mKey);
    sslSocket->setLocalCertificate(mCertificate);
    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->startServerEncryption();

    addPendingConnection(sslSocket);
}

void SslServer::SslErrors(const QList<QSslError> &errors)
{
    qDebug() << "SslServer::SslErrors";

    for(const QSslError &error : errors)
        qDebug() << error.errorString();
}

void SslServer::Link()
{
    qDebug() << "SslServer::Link";

    QTcpSocket *clientSocket = nextPendingConnection();

    connect( clientSocket, SIGNAL(readyRead()), this, SLOT(ConnectIsReady()) );
    connect( clientSocket, SIGNAL(disconnected()), this, SLOT(Disconnected()) );
}

void SslServer::ConnectIsReady()
{
    qDebug() << "SslServer::ConnectIsReady";

    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

    emit(Ready(clientSocket));
}

void SslServer::Disconnected()
{
    qDebug("Client Disconnected!");

    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

    clientSocket->deleteLater();
}
