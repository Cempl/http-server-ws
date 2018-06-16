#ifndef _Response_H
	#define _Response_H
#pragma once


/*******************************************************************************/
#include <QObject>
#include <QString>


using namespace std;


/*******************************************************************************/
class GeneratorOfResponseHTTP : public QObject
{
    Q_OBJECT

    public://////////////////////////////////////////////////////////////////////

        enum FileTypeList
        {
            fDefault = 0,

            fHtmlType,
            fCssType,
            fJSType,
            fImageType
        };

        enum RequestMethodsList
        {
            RErrorType = 0,

            GET,
            HEAD,
            POST,
            PUT,
            DEL, // This is an abbreviated name because DELETE is macros
            CONNECT,
            OPTIONS,
            TRACE
        };

        enum HTTPVersionsList
        {
            HErrorType = 0,

            HTTP09,
            HTTP10,
            HTTP11,
            HTTP20
        };

        enum WebSocketFieldType
        {
            WErrorType = 0,

            Upgrade,
            WebSockKey,
            WebSockVersion
        };

        struct cfgOfResponse
        {
            FileTypeList fileType = FileTypeList::fDefault;
            RequestMethodsList requestMethod = RequestMethodsList::RErrorType;
            HTTPVersionsList HTTTPVersion = HTTPVersionsList::HErrorType;

            QString fileName = "home.html";

            bool isWebSocket = false;
            bool isCorretVersion = false; // Version of websocket
            QString keyWebSocket;

        };


	public://////////////////////////////////////////////////////////////////////

                            GeneratorOfResponseHTTP();
                            GeneratorOfResponseHTTP(const GeneratorOfResponseHTTP& inOther) = delete;
                            ~GeneratorOfResponseHTTP();


    signals://///////////////////////////////////////////////////////////////////

        void RespIsGenerated(const QString strResp);
        void HEADIsParsed(const cfgOfResponse inCfg);


    private slots:///////////////////////////////////////////////////////////////

        cfgOfResponse ParseHttpHEAD( const QString inHEAD );
        void GenerateResponse( const cfgOfResponse inCfg );


    private://///////////////////////////////////////////////////////////////////

        FileTypeList        getFileType(const QString& inFileType);
        RequestMethodsList  getRequestMethod(const QString& inRequestMethod);
        HTTPVersionsList    getHTTPVersion(const QString& inHTTPVersion);
        WebSocketFieldType  getWebSockFieldType(const QString& inStrField);

        void                setWebSockFieldData(const WebSocketFieldType& inType,
                                                const QString& inInform,
                                                cfgOfResponse& cfgResponse);
};

Q_DECLARE_METATYPE(GeneratorOfResponseHTTP::cfgOfResponse)

#endif // _Response_H
