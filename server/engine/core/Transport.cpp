/*******************************************************************************/
#include "Transport.h"


/*******************************************************************************/
Transport::Transport(ServerConfig config)
    :
      obj(config)
{
    connect(&obj, SIGNAL(Ready(QTcpSocket*)), this, SLOT(recieveRequest(QTcpSocket*)));
    connect(this, SIGNAL(incomingRequest(const QString)), &parseObj, SLOT(ParseHttpHEAD(const QString)));
}


/*******************************************************************************/
Transport::~Transport()
{
    // do nothing
}


/*******************************************************************************/
void Transport::recieveRequest(QTcpSocket* socket)
{
    connect(&parseObj, &GeneratorOfResponseHTTP::RespIsGenerated, this, [&](const QString inResponse) { sendResponse(inResponse, socket);});

    emit(incomingRequest(QString(socket->readAll())));
}


/*******************************************************************************/
void Transport::sendResponse(const QString inResponse, QTcpSocket* socket)
{
    qDebug() << socket->readAll() << "\n\n\n\n";
    qDebug() << inResponse << "\n\n\n\n";

    socket->write(inResponse.toLatin1().data());

    socket->close();
}
