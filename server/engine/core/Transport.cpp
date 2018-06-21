/*******************************************************************************/
#include "Transport.h"


/*******************************************************************************/
Transport::Transport(ServerConfig config)
:
    obj(config),
    mpSocket(nullptr)
{
    connect(&obj, SIGNAL(Ready(QTcpSocket*)), this, SLOT(recieveRequest(QTcpSocket*)));
    connect(&parseObj, SIGNAL(RespIsGenerated(QString)), this, SLOT(sendResponse(QString)));
}


/*******************************************************************************/
Transport::~Transport()
{
    deleteLater();
}


/*******************************************************************************/
void Transport::recieveRequest(QTcpSocket* socket)
{
    mpSocket = socket;

    if(mpSocket && mpSocket->isReadable())
       parseObj.ParseHttpHEAD(QString(mpSocket->readAll()));
}


/*******************************************************************************/
void Transport::sendResponse(const QString inResponse)
{
    qDebug() << "sendResponse(response): " << inResponse;

    if(mpSocket && mpSocket->isWritable())
    {
       mpSocket->write(inResponse.toLatin1().data());
       mpSocket->close();
    }
}
