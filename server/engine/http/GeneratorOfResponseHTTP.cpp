/*******************************************************************************/
#include <QFile>

#include "GeneratorOfResponseHTTP.h"
#include "WSLexer.h"


/*******************************************************************************/
GeneratorOfResponseHTTP::GeneratorOfResponseHTTP()
{
    connect(this, SIGNAL(HEADIsParsed(const cfgOfResponse)), this, SLOT(GenerateResponse(const cfgOfResponse)));
}


/*******************************************************************************/
GeneratorOfResponseHTTP::~GeneratorOfResponseHTTP()
{
    // do nothing
}

/*******************************************************************************/
GeneratorOfResponseHTTP::cfgOfResponse GeneratorOfResponseHTTP::ParseHttpHEAD(const QString inHEAD)
{
    try
    {
        cfgOfResponse cfgResponse;

        WSLexer mLexer(inHEAD);
        while( mLexer.NextToken(true) )
        {
            QString strCurrToken(mLexer.getToken()->getStart(), mLexer.getToken()->getLen());

            if( cfgResponse.requestMethod == RequestMethodsList::RErrorType )
            {
                if( cfgResponse.requestMethod = getRequestMethod(strCurrToken) )
                {
                    continue;
                }
            }

            if(     cfgResponse.fileType == FileTypeList::fDefault
                &&  mLexer.getToken()->getPrevToken()->getType() == wsTokenTypes::wsSymbolType
                &&  *(mLexer.getToken()->getPrevToken()->getStart()) == '.'
                )
            {
                if( cfgResponse.fileType = getFileType(strCurrToken) )
                {
                    cfgResponse.fileName = QString(mLexer.getToken()->getPrevToken()->getPrevToken()->getStart(),
                        mLexer.getToken()->getPrevToken()->getPrevToken()->getLen());
                    cfgResponse.fileName += "." + strCurrToken;
                    continue;
                }
            }

            if( cfgResponse.requestMethod == RequestMethodsList::RErrorType )
            {
                if( cfgResponse.requestMethod = getRequestMethod(strCurrToken) )
                {
                    continue;
                }
            }

            if(     false == cfgResponse.isWebSocket
                ||  false == cfgResponse.isCorretVersion
                ||  cfgResponse.keyWebSocket.isEmpty() )
            {
                WebSocketFieldType fieldType = WebSocketFieldType::WErrorType;

                if( fieldType = getWebSockFieldType(strCurrToken) )
                {
                    mLexer.NextToken(true);
                    if(     *(mLexer.getToken()->getStart()) == ':' 
                        &&  mLexer.getToken()->getType() == wsTokenTypes::wsSymbolType)
                    {
                        mLexer.NextToken(true);

                        QString strInform;
                        while(mLexer.getToken()->getType() != wsTokenTypes::wsNewLineSymbolType);
                        {
                            strInform += QString(mLexer.getToken()->getStart(), mLexer.getToken()->getLen());
                            mLexer.NextToken(true);
                        }

                        setWebSockFieldData(fieldType, strInform, cfgResponse);
                    }
                }
            }
        }

        emit(HEADIsParsed(cfgResponse));
    }
    catch (...)
    {
        // TODO: do somethig... Think about it
    }
}


/*******************************************************************************/
void GeneratorOfResponseHTTP::GenerateResponse( const cfgOfResponse cfgResponse )
{
    // TODO: Ivan: We should do something if this is WebSocket. Think about it.
    try
    {
        QString HTTPResponse;

        if(     cfgResponse.requestMethod == RequestMethodsList::GET
                &&  cfgResponse.HTTTPVersion == HTTPVersionsList::HTTP11)
        {
            HTTPResponse = "HTTP/1.1 200 OK\n";
        }

        HTTPResponse += "Server: VaV/V2\n";

        switch(cfgResponse.fileType)
        {
            case fCssType:
            {
                HTTPResponse += "Content-Type: text/css;\r\n\r\n";

            }break;

            case fJSType:
            {
                HTTPResponse += "Content-Type: application/javascript;\r\n\r\n";

            }break;

            case fImageType:
            {
                HTTPResponse += "Content-Type: image/png;\r\n\r\n";

            }break;

            case fHtmlType:
            default:
            {
                HTTPResponse += "Content-Type: text/html;\n";
                HTTPResponse += "Connection: keep-alive\n";
                HTTPResponse += "X-Powered-By: c++\r\n\r\n";

            }break;
        }

        // TODO: Ivan: temporary solution! Fix me somebody! Please!!!!
        /*QFile file(cfgResponse.fileName);
        file.open(QIODevice::ReadOnly, QIODevice::Text);
        QTextStream str(&file);
        while(!str.atEnd())
        {
            HTTPResponse += str.readAll();
        }*/
        HTTPResponse += "<!DOCTYPE html> <html><body>  <h1>Server says Hello!</h1> </body></html>";
        emit(RespIsGenerated(HTTPResponse));
    }
    catch(...)
    {
        // TODO: Ivan: do somethig... Think about it
    }
}


/*******************************************************************************/
GeneratorOfResponseHTTP::FileTypeList GeneratorOfResponseHTTP::getFileType(const QString& inFileType)
{
    FileTypeList res = FileTypeList::fDefault;

    if(inFileType == "html" )
    {
        res = FileTypeList::fHtmlType;
        goto getFileTypeExitMark;
    }

    if(inFileType == "css")
    {
        res = FileTypeList::fCssType;
        goto getFileTypeExitMark;
    }

    if(inFileType == "js")
    {
        res = FileTypeList::fJSType;
        goto getFileTypeExitMark;
    }

    if( inFileType == "jpg"		||
        inFileType == "tiff"	||
        inFileType == "png"		||
        inFileType == "jpeg"	||
        inFileType == "gif"		||
        inFileType == "ico"		||
        inFileType == "svg")
    {
        res = FileTypeList::fImageType;
        goto getFileTypeExitMark;
    }


getFileTypeExitMark:
    return res;
}


/*******************************************************************************/
GeneratorOfResponseHTTP::RequestMethodsList GeneratorOfResponseHTTP::getRequestMethod(const QString& inRequestMethod)
{
    RequestMethodsList res = RequestMethodsList::RErrorType;

    if(inRequestMethod == "GET")
    {
        res = RequestMethodsList::GET;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "HEAD")
    {
        res = RequestMethodsList::HEAD;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "POST")
    {
        res = RequestMethodsList::POST;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "PUT")
    {
        res = RequestMethodsList::PUT;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "DELETE")
    {
        res = RequestMethodsList::DEL;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "CONNECT")
    {
        res = RequestMethodsList::CONNECT;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "OPTIONS")
    {
        res = RequestMethodsList::OPTIONS;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "TRACE")
    {
        res = RequestMethodsList::TRACE;
        goto getRequestMethodExitMark;
    }

getRequestMethodExitMark:
    return res;
}


/*******************************************************************************/
GeneratorOfResponseHTTP::HTTPVersionsList GeneratorOfResponseHTTP::getHTTPVersion(const QString& inHTTPVersion)
{
    HTTPVersionsList res = HTTPVersionsList::HErrorType;

    if(inHTTPVersion == "0.9")
    {
        res = HTTPVersionsList::HTTP09;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "1.0")
    {
        res = HTTPVersionsList::HTTP10;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "1.1")
    {
        res = HTTPVersionsList::HTTP11;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "2")
    {
        res = HTTPVersionsList::HTTP20;
        goto getHTTPVersionExitMark;
    }

getHTTPVersionExitMark:
    return res;
}


/*******************************************************************************/
GeneratorOfResponseHTTP::WebSocketFieldType GeneratorOfResponseHTTP::getWebSockFieldType(const QString& inStrField)
{
    WebSocketFieldType res = WebSocketFieldType::WErrorType;

    if( inStrField == "Upgrade")
    {
        res = WebSocketFieldType::Upgrade;
        goto getWebSockFieldTypenExitMark;
    }

    if( inStrField == "Sec-WebSocket-Key")
    {
        res = WebSocketFieldType::WebSockKey;
        goto getWebSockFieldTypenExitMark;
    }

    if( inStrField == "Sec-WebSocket-Version")
    {
        res = WebSocketFieldType::WebSockVersion;
        goto getWebSockFieldTypenExitMark;
    }

getWebSockFieldTypenExitMark:
    return res;
}


/*******************************************************************************/
void GeneratorOfResponseHTTP::setWebSockFieldData(const WebSocketFieldType& inType,
                                                  const QString& inInform,
                                                  cfgOfResponse& cfgResponse)
{
    switch( inType )
    {
        case WebSocketFieldType::Upgrade:
        {
            cfgResponse.isWebSocket = !(QString::compare(inInform, "WebSocket", Qt::CaseInsensitive));
            break;
        }

        case WebSocketFieldType::WebSockVersion:
        {
            cfgResponse.isCorretVersion = !(QString::compare(inInform, "13", Qt::CaseInsensitive));
            break;
        }

        case WebSocketFieldType::WebSockKey:
        {
            cfgResponse.keyWebSocket = inInform;
            break;
        }

        case WebSocketFieldType::WErrorType:
        default:
        {
            break;
        }
    }
}
