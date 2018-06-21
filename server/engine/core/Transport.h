#ifndef _TRANSPORT_H
    #define _TRANSPORT_H

// TODO: Ivan: This is temporary class.I think this class should be remake. Think about it!
/*******************************************************************************/
#include "SslServer.h"
#include "../http/GeneratorOfResponseHTTP.h"

// Types
#include "../types/ServerTypes.h"



/*******************************************************************************/
class Transport : public QObject
{
    Q_OBJECT

    public://////////////////////////////////////////////////////////////////////

explicit    Transport(ServerConfig config);
            Transport(const Transport& inOther) = delete;
            ~Transport();


    private slots:///////////////////////////////////////////////////////////////

    void recieveRequest(QTcpSocket* socket);
    void sendResponse(const QString inResponse);

    private://///////////////////////////////////////////////////////////////////

    SslServer obj;
    GeneratorOfResponseHTTP parseObj;

    QPointer<QTcpSocket> mpSocket;
};


#endif // _TRANSPORT_H
