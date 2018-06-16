/*******************************************************************************/
#include "Transport.h"
#include "../http/GeneratorOfResponseHTTP.h"


/*******************************************************************************/
Transport::Transport(ServerConfig config)
    :
      obj(config)
{
    connect(&obj, SIGNAL(Ready(QTcpSocket*)), this, SLOT(recieveRequest(QTcpSocket*)));
}


/*******************************************************************************/
Transport::~Transport()
{
    // do nothing
}


/*******************************************************************************/
void Transport::recieveRequest(QTcpSocket* socket)
{
    GeneratorOfResponseHTTP parseObj(this);
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
