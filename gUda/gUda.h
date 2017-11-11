#ifndef GUDA_H
#define GUDA_H


#include <gfw_global.h>
#include <gString/gString.h>
#include <gArray/gDualList.h>
namespace gfw
{
//utility class for accessing the network

//some useful dual lists
    typedef gDualList<gString,gString> gUdaDualStrings;

    class SHARED_GFW gUda{
    public:
        enum GUDA_PROXY_TYPES
        {
            GUDA_PROXY_HTTP = 0,   /* added in 7.10, new in 7.19.4 default is to use
                                     CONNECT HTTP/1.1 */
            GUDA_PROXY_HTTP_1_0 = 1,   /* added in 7.19.4, force to use CONNECT
                                         HTTP/1.0  */
            GUDA_PROXY_SOCKS4 = 4, /* support added in 7.15.2, enum existed already
                                     in 7.10 */
            GUDA_PROXY_SOCKS5 = 5, /* added in 7.10 */
            GUDA_PROXY_SOCKS4A = 6, /* added in 7.18.0 */
            GUDA_PROXY_SOCKS5_HOSTNAME = 7 /* Use the SOCKS5 protocol but pass along the
                                             host name rather than the IP address. added
                                             in 7.18.0 */
        };
        //int ProgressFunc(void *pData,double dTotal,double dNew,double dUt,double dnT)
        typedef int (*IProgressFunc)(void *,double,double,double,double);

        struct gUdaProxy
        {
            gString proxyServer;
            gu32 proxyPort;
            gu32 proxyType;
            gString userName;
            gString passWord;

            gUdaProxy():proxyPort(8080),proxyType(GUDA_PROXY_HTTP)
            {

            }
        };

        struct gUdaParams
        {
            gu32 connectTimeOut;
            gu32 downloadTimeOut;
            gString cookie;
            gString postFields;
            gString referer;
            IProgressFunc progress;
            void *progressUserData;
            gUdaParams():connectTimeOut(10),downloadTimeOut(30),progress(0),progressUserData(0)
            {

            }
            //and more to come
            gList<gString> header;
            gString cookieJar;//to store session cookies
        };

        enum GUDA_ERROR_VALUES
        {
            GUDA_E_OK = 0,
            GUDA_E_UNSUPPORTED_PROTOCOL,    /* 1 */
            GUDA_E_FAILED_INIT,             /* 2 */
            GUDA_E_URL_MALFORMAT,           /* 3 */
            GUDA_E_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
                                              7.17.0, reused in April 2011 for 7.21.5] */
            GUDA_E_COULDNT_RESOLVE_PROXY,   /* 5 */
            GUDA_E_COULDNT_RESOLVE_HOST,    /* 6 */
            GUDA_E_COULDNT_CONNECT,         /* 7 */
            GUDA_E_FTP_WEIRD_SERVER_REPLY,  /* 8 */
            GUDA_E_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                              due to lack of access - when login fails
                                              this is not returned. */
            GUDA_E_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
                                              7.15.4, reused in Dec 2011 for 7.24.0]*/
            GUDA_E_FTP_WEIRD_PASS_REPLY,    /* 11 */
            GUDA_E_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
                                              [was obsoleted in August 2007 for 7.17.0,
                                              reused in Dec 2011 for 7.24.0]*/
            GUDA_E_FTP_WEIRD_PASV_REPLY,    /* 13 */
            GUDA_E_FTP_WEIRD_227_FORMAT,    /* 14 */
            GUDA_E_FTP_CANT_GET_HOST,       /* 15 */
            GUDA_E_OBSOLETE16,              /* 16 - NOT USED */
            GUDA_E_FTP_COULDNT_SET_TYPE,    /* 17 */
            GUDA_E_PARTIAL_FILE,            /* 18 */
            GUDA_E_FTP_COULDNT_RETR_FILE,   /* 19 */
            GUDA_E_OBSOLETE20,              /* 20 - NOT USED */
            GUDA_E_QUOTE_ERROR,             /* 21 - quote command failure */
            GUDA_E_HTTP_RETURNED_ERROR,     /* 22 */
            GUDA_E_WRITE_ERROR,             /* 23 */
            GUDA_E_OBSOLETE24,              /* 24 - NOT USED */
            GUDA_E_UPLOAD_FAILED,           /* 25 - failed upload "command" */
            GUDA_E_READ_ERROR,              /* 26 - couldn't open/read from file */
            GUDA_E_OUT_OF_MEMORY,           /* 27 */
            /* Note: GUDA_E_OUT_OF_MEMORY may sometimes indicate a conversion error
                     instead of a memory allocation error if CURL_DOES_CONVERSIONS
                     is defined
            */
            GUDA_E_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
            GUDA_E_OBSOLETE29,              /* 29 - NOT USED */
            GUDA_E_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
            GUDA_E_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
            GUDA_E_OBSOLETE32,              /* 32 - NOT USED */
            GUDA_E_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
            GUDA_E_HTTP_POST_ERROR,         /* 34 */
            GUDA_E_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
            GUDA_E_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
            GUDA_E_FILE_COULDNT_READ_FILE,  /* 37 */
            GUDA_E_LDAP_CANNOT_BIND,        /* 38 */
            GUDA_E_LDAP_SEARCH_FAILED,      /* 39 */
            GUDA_E_OBSOLETE40,              /* 40 - NOT USED */
            GUDA_E_FUNCTION_NOT_FOUND,      /* 41 */
            GUDA_E_ABORTED_BY_CALLBACK,     /* 42 */
            GUDA_E_BAD_FUNCTION_ARGUMENT,   /* 43 */
            GUDA_E_OBSOLETE44,              /* 44 - NOT USED */
            GUDA_E_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
            GUDA_E_OBSOLETE46,              /* 46 - NOT USED */
            GUDA_E_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
            GUDA_E_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
            GUDA_E_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
            GUDA_E_OBSOLETE50,              /* 50 - NOT USED */
            GUDA_E_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
                                               wasn't verified fine */
            GUDA_E_GOT_NOTHING,             /* 52 - when this is a specific error */
            GUDA_E_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
            GUDA_E_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                              default */
            GUDA_E_SEND_ERROR,              /* 55 - failed sending network data */
            GUDA_E_RECV_ERROR,              /* 56 - failure in receiving network data */
            GUDA_E_OBSOLETE57,              /* 57 - NOT IN USE */
            GUDA_E_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
            GUDA_E_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
            GUDA_E_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
            GUDA_E_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
            GUDA_E_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
            GUDA_E_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
            GUDA_E_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
            GUDA_E_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                              that failed */
            GUDA_E_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
            GUDA_E_LOGIN_DENIED,            /* 67 - user, password or similar was not
                                              accepted and we failed to login */
            GUDA_E_TFTP_NOTFOUND,           /* 68 - file not found on server */
            GUDA_E_TFTP_PERM,               /* 69 - permission problem on server */
            GUDA_E_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
            GUDA_E_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
            GUDA_E_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
            GUDA_E_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
            GUDA_E_TFTP_NOSUCHUSER,         /* 74 - No such user */
            GUDA_E_CONV_FAILED,             /* 75 - conversion failed */
            GUDA_E_CONV_REQD,               /* 76 - caller must register conversion
                                              callbacks using curl_easy_setopt options
                                              CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                              CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                              CURLOPT_CONV_FROM_UTF8_FUNCTION */
            GUDA_E_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                              or wrong format */
            GUDA_E_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
            GUDA_E_SSH,                     /* 79 - error from the SSH layer, somewhat
                                              generic so the error message will be of
                                              interest when this has happened */

            GUDA_E_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                              connection */
            GUDA_E_AGAIN,                   /* 81 - socket is not ready for send/recv,
                                              wait till it's ready and try again (Added
                                              in 7.18.2) */
            GUDA_E_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                              wrong format (Added in 7.19.0) */
            GUDA_E_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                              7.19.0) */
            GUDA_E_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
            GUDA_E_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
            GUDA_E_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
            GUDA_E_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
            GUDA_E_CHUNK_FAILED,            /* 88 - chunk callback reported error */
            GUDA_E_NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the */
            GUDA_E_CANNOTINITIALIZE,
            GUDA_E_OPENFILE,
            GUDA_E_SIZE
        };

        static gu32 init();
        static void finish();
        static gu32 urlContent(const gString &url,gByteArray &out,const gString &userAgent=gString(),
                        const gUdaProxy *proxy=0,const gUdaParams &params=gUdaParams());

        static gu32 ftpUpload(const gString &server,const gString &sourceFile,const gString &uploadAs,
                              const gString &destNameFile,
                              const gString &username=gString(),const gString &password=gString(),
                              const gString &directory=gString(),const gUdaProxy *proxy=0,
                              const gUdaParams &params=gUdaParams());
        //same as php
        //New post for a wordpress blog via rpc
        static gu32 xmlrpc_wp_post(const gString &url,const gString &user, const gString &password,
                                   const gString &title,const gString &content,bool bPublish,
                                   const gList<gString> &categories,const gList<gString> &tags, const gUdaDualStrings &customFields);
        static gu32 xmlrpc_wp_getcategories(const gString &url,const gString &user,const gString &password,
                                            gUdaDualStrings &out);
        static gString url_encode(const gString &url);
        static gString url_decode(const gString &url);


    };
}
#endif // GUDA_H
