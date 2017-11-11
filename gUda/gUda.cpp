#include "gUda.h"

#include <curl/curl.h>
#include <gFile/gFile.h>
#include <gXMLParser/gXMLParser.h>
#include <cstring>
//download function used by curl
using namespace gfw;
size_t download(void *ptr,size_t size,size_t msize,void *pUser)
{
    gu32 dwRealSize;
    gByteArray *sData;//Data
    gByteArray szTemp;

    sData=(gByteArray *)pUser;
    dwRealSize=msize * size;//The real size

    if(sData->isEmpty())
    {
        sData->alloc(dwRealSize);
        memcpy(sData->data(),ptr,dwRealSize);
    }
    else
    {
        szTemp.alloc(dwRealSize);
        memcpy(szTemp.data(),ptr,dwRealSize);
        sData->insert(szTemp);
    }
    return dwRealSize;
}

size_t upload(void *ptr,size_t size,size_t msize,void *pUser)
{
    curl_off_t nread;

     /* in real-world cases, this would probably get this data differently
        as this fread() stuff is exactly what the library already would do
        by default internally */
    gFile *f=(gFile *)pUser;
    //size_t retcode = fread(ptr, size, msize, pUser);
    gsize retcode=f->read(ptr,size,msize);

     nread = (curl_off_t)retcode;


     return retcode;
}

//download

gu32 gUda::init()
{
     return curl_global_init(CURL_GLOBAL_ALL);
}

void gUda::finish()
{
    curl_global_cleanup();
}

gu32 gUda::urlContent(const gString &url, gByteArray &out, const gString &userAgent,
                      const gUdaProxy *proxy, const gUdaParams &params)
{
    CURL *curl;



    curl=curl_easy_init();
    if(!curl)
        return GUDA_E_CANNOTINITIALIZE;

    curl_easy_setopt(curl,CURLOPT_URL,url.ascii());
    if(proxy)
    {
        curl_easy_setopt(curl,CURLOPT_PROXY,proxy->proxyServer.ascii());
        curl_easy_setopt(curl,CURLOPT_PROXYPORT,proxy->proxyPort);
        curl_easy_setopt(curl,CURLOPT_PROXYTYPE,proxy->proxyType);
        if(proxy->proxyType==CURLPROXY_HTTP)
            curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,true);

        if(proxy->userName.isEmpty()==false)
        {
            curl_easy_setopt(curl,CURLOPT_PROXYUSERNAME,proxy->userName.ascii());
            curl_easy_setopt(curl,CURLOPT_PROXYUSERNAME,proxy->passWord.ascii());
            curl_easy_setopt(curl,CURLOPT_PROXYAUTH,CURLAUTH_ANY);
        }
    }
    //next
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);//Follows redirects
    if(userAgent.isEmpty()==false)
        curl_easy_setopt(curl,CURLOPT_USERAGENT,userAgent.ascii());//Set the user agent
    curl_easy_setopt(curl,CURLOPT_AUTOREFERER,1);//auto referrer on redirects
    curl_easy_setopt(curl,CURLOPT_ENCODING,"");
 //   curl_easy_setopt(curl,CURLOPT_HEADER,true);
    curl_easy_setopt(curl,CURLOPT_MAXREDIRS,10);//default 10 which is an acceptable number

    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,download);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&out);
    curl_easy_setopt(curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_1);

    curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);//avoid signals to avoid crashings on linux

    //extras
    if(params.progress)
    {
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,params.progress);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,params.progressUserData);
    }
    curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,params.connectTimeOut);
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,params.downloadTimeOut);
    if(params.cookie.isEmpty()==false)
    {
        curl_easy_setopt(curl,CURLOPT_COOKIE,params.cookie.ascii());
    }
    if(!params.postFields.isEmpty())
    {
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,params.postFields.ascii());
    }
    struct curl_slist *slist=0;
    if(!params.header.isEmpty()){

        for(int i=0;i<params.header.size();i++){
            slist=curl_slist_append(slist,params.header[i].ascii());
        }
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,slist);
    }
    if(!params.cookieJar.isEmpty()){
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,params.cookieJar.ascii());
        curl_easy_setopt(curl,CURLOPT_COOKIEFILE,params.cookieJar.ascii());
    }
    if(!params.referer.isEmpty()){
        curl_easy_setopt(curl,CURLOPT_REFERER,params.referer.ascii());
    }
    long code;
    gu32 res=curl_easy_perform(curl);
    //check for errors
    if(slist)
        curl_slist_free_all(slist);
    if(res==CURLE_OK)
    {
        res=curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&code);
        if(code>400)
            res=code;
    }

    curl_easy_cleanup(curl);
    return res;

}

gu32 gUda::ftpUpload(const gString &server, const gString &sourceFile, const gString &uploadAs,
                     const gString &destNameFile, const gString &username,
                     const gString &password,const gString &directory,const gUdaProxy *proxy, const gUdaParams &params)
{
    CURL *curl;
    struct curl_slist *header=0;

    gFile f;
    curl_off_t fsize;
    //dest url
    gString url;
    //To initialzie headers
    gString buf1("RNFR "),buf2("RNTO ");
    buf1+=uploadAs;
    buf2+=destNameFile;
    if(!f.open(sourceFile,gFile::GFILEAM_ACCESSREAD))
    {
        return GUDA_E_OPENFILE;
    }

    fsize=(curl_off_t)f.size();

    curl=curl_easy_init();
    if(!curl)
        return GUDA_E_CANNOTINITIALIZE;

    url+=server + "/";
    if(directory.isEmpty()==false)
    {
        url+=directory + "/";
    }
    url+=uploadAs;
    curl_easy_setopt(curl,CURLOPT_URL,url.ascii());

    //set the headers for ftp
    header=curl_slist_append(header,buf1.ascii());
    header=curl_slist_append(header,buf2.ascii());
    //Set params for upload
    curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
    curl_easy_setopt(curl,CURLOPT_POSTQUOTE,header);
    //file size
    curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,fsize);
    //If we are using proxies
    //set user and password
    if(username.isEmpty()==false)
    {
        curl_easy_setopt(curl,CURLOPT_USERNAME,username.ascii());
        curl_easy_setopt(curl,CURLOPT_PASSWORD,password.ascii());
    }

    if(proxy)
    {
        curl_easy_setopt(curl,CURLOPT_PROXY,proxy->proxyServer.ascii());
        curl_easy_setopt(curl,CURLOPT_PROXYPORT,proxy->proxyPort);
        curl_easy_setopt(curl,CURLOPT_PROXYTYPE,proxy->proxyType);
        if(proxy->proxyType==CURLPROXY_HTTP)
            curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,true);

        if(proxy->userName.isEmpty()==false)
        {
            curl_easy_setopt(curl,CURLOPT_PROXYUSERNAME,proxy->userName.ascii());
            curl_easy_setopt(curl,CURLOPT_PROXYUSERNAME,proxy->passWord.ascii());
            curl_easy_setopt(curl,CURLOPT_PROXYAUTH,CURLAUTH_ANY);
        }
    }
    //next
    //curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);//Follows redirects
    //curl_easy_setopt(curl,CURLOPT_USERAGENT,userAgent.ascii());//Set the user agent
    //curl_easy_setopt(curl,CURLOPT_AUTOREFERER,1);//auto referrer on redirects
    //curl_easy_setopt(curl,CURLOPT_ENCODING,"");
 //   curl_easy_setopt(curl,CURLOPT_HEADER,true);
    //curl_easy_setopt(curl,CURLOPT_MAXREDIRS,10);//default 10 which is an acceptable number

    curl_easy_setopt(curl,CURLOPT_READFUNCTION,upload);
    curl_easy_setopt(curl,CURLOPT_READDATA,(void *)&f);
   // curl_easy_setopt(curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_1);

    //curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);//avoid signals to avoid crashings on linux

    //extras
    if(params.progress)
    {
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,params.progress);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,params.progressUserData);
    }
    curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,params.connectTimeOut);
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,params.downloadTimeOut);
    if(params.cookie.isEmpty()==false)
    {
        curl_easy_setopt(curl,CURLOPT_COOKIE,params.cookie.ascii());
    }
    long code;
    gu32 res=curl_easy_perform(curl);
    //check for errors
    if(res==CURLE_OK)
    {
        res=curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&code);
        if(code>400)
            res=code;
    }

    curl_easy_cleanup(curl);
    return res;
}

gu32 gUda::xmlrpc_wp_getcategories(const gString &url, const gString &user, const gString &password, gUdaDualStrings &out)
{
    //lets prepare the damn xml buffer
    gByteArray bout;
    gString xml("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    gUdaParams p;
    gXMLParser parser;

    xml+="<methodCall>\n";
    xml+="<methodName>wp.getCategories</methodName>\n";
    xml+="<params>\n";
    xml+="<param><value><int>0</int></value></param>";
    //set the user name
    xml+="<param><value><string>";
    xml+=user;
    xml+="</string></value></param>\n";
    //set the password
    xml+="<param><value><string>";
    xml+=password;
    xml+="</string></value></param>\n";
    xml+="</params>\n</methodCall>";

    p.postFields=xml;
    gUda::init();
    gu32 ret=gUda::urlContent(url + "xmlrpc.php",bout,"XMLRPC_POSTER/V1.0",0,p);

    gUda::finish();
    //now check the output and fill the structure
    if(ret!=gUda::GUDA_E_OK)return ret;

    xml=gString::fromByteArray(bout);

    parser.SetTextBuffer(xml.ascii());

    parser.Parse();
    parser.GotoMainNode();

    //check the results from the xmlrpc call
    gString name,value,categoryid,category;
    while(parser.scriptWalk())
    {
        parser.checkLevels();
        if(parser.readTagName()=="struct")
            parser.setLevel("struct");
        if(parser.inLevel("struct"))
        {
            if(parser.readTagName()=="member"){
                parser.setLevel("member");
            }
            if(parser.inLevel("member"))
            {
                if(parser.readTagName()=="name"){
                    name=parser.readTagText();
                }
                if(parser.readTagName()=="string"){
                    value=parser.readTagText();
                }
                if(!name.isEmpty() && !value.isEmpty()){
                    if(name=="categoryId"){
                        categoryid=value;
                    }
                    else if(name=="categoryName"){
                        category=value;
                    }
                    name.clear();
                    value.clear();
                }
            }
            if(!categoryid.isEmpty() && !category.isEmpty()){
                out.append(category,categoryid);
                categoryid.clear();
                category.clear();
            }
        }

    }

    return ret;
}

//the big tamale here...post new
gu32 gUda::xmlrpc_wp_post(const gString &url, const gString &user, const gString &password,
                          const gString &title, const gString &content,bool bPublish, const gList<gString> &categories,
                          const gList<gString> &tags, const gUdaDualStrings &customFields){

    gString xml("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    gUdaParams p;

    xml+="<methodCall>\n";
    xml+="<methodName>wp.newPost</methodName>\n";
    xml+="<params>\n";
    xml+="<param><value><int>0</int></value></param>";
    //set the user name
    xml+="<param><value><string>";
    xml+=user;
    xml+="</string></value></param>\n";
    //set the password
    xml+="<param><value><string>";
    xml+=password;
    xml+="</string></value></param>\n";

    //Adding here the rest of the post aka the content
    xml+="<param>"
         "<value>"
         "<struct>"
         "<member>\n";
    //Set the tile
    xml+="<name>post_title</name>\n";
    xml+="<value><string>";
    xml+=title;
    xml+="</string></value>\n</member>\n";//Close with member
    //set content
    xml+="<member>"
    "<name>"
    "post_content"
    "</name>"
    "<value><string>";
    xml+=content;
    xml+="</string></value>"
         "</member>";
    //Post type
    xml+="<member>"
    "<name>"
    "post_type"
    "</name>"
    "<value><string>";
    xml+="post";
    xml+="</string></value>"
         "</member>";
    //post status
    xml+="<member>"
    "<name>"
    "post_status"
    "</name>"
    "<value><string>";
    if(bPublish)
        xml+="publish";
    else
        xml+="draft";
    xml+="</string></value>"
         "</member>";
    //Now set categories
    if(!categories.isEmpty())
    {
        xml+="<member>"
        "<name>"
        "terms"
        "</name>"
        "<value>"
        "<struct>"
        "<member>"
        "<name>category</name>"
        "<value>"
        "<array>"
        "<data>";
        for(int i=0;i<categories.size();i++){
            xml+="<value><int>";
            xml+=categories[i];
            xml+="</int></value>\n";
        }
        xml+="</data>"
             "</array>";
        xml+="</value>"
        "</member>"
        "</struct>"
        "</value>"
        "</member>";
    }
    //Now set tags if any
    if(!tags.isEmpty()){
        xml+="<member>"
        "<name>"
        "tags_input"
        "</name>"
        "<value><string>";
        int size=tags.size();
        for(int i=0;i<size;i++){

            xml+=tags[i];
            if(i<(size - 1))
                xml+=",";
        }

        xml+="</string></value>"
             "</member>";

    }
    //Now set custom fields if available
    if(!customFields.isEmpty()){
        xml+="<member>"
                "<name>"
                "custom_fields"
                "</name>"
                "<value>"
                "<array>"
                "<data>";

        for(int i=0;i<customFields.count();i++)
        {
            xml+="<value>"
                 "<struct>"
                 "<member>"
                 "<name>key</name>\n";
            xml+="<value><string>";
            xml+=customFields.getU(i);
            xml+="</string></value>";
            xml+="</member>";

            xml+="<member>"
                 "<name>value</name>\n";
            xml+="<value><string>";
            xml+=customFields.getV(i);
            xml+="</string></value>";
            xml+="</member>"
                 "</struct>"
                 "</value>";

        }
        xml+="</data>"
             "</array>"
             "</value>"
             "</member>";
    }
    xml+="</struct>"
         "</value>"
         "</param>";
    //Next param
    xml+="<param><value><boolean>1</boolean></value></param>";
    xml+="</params>\n</methodCall>";
    gFile f;
    f.open("./xmlout.xml",gFile::GFILEAM_ACCESSWRITE);
    f.write(xml.ascii(),sizeof(char),xml.size());
    f.close();

    p.postFields=xml;

    //post this fucking thing
    gByteArray out;
    gUda::init();

    gu32 res=gUda::urlContent(url + "xmlrpc.php",out,"XMLRPC_POSTER/V1.0",0,p);
    //debuging only

    f.open("./log.txt",gFile::GFILEAM_ACCESSWRITE);
    f.write(out.data(),sizeof(char),out.size());

    gUda::finish();

    return res;
}


gString gUda::url_encode(const gString &url){
    CURL *curl;
    curl=curl_easy_init();

    char *c=curl_easy_escape(curl,url.ascii(),url.size());

    gString res(c);

    curl_free(c);

    curl_easy_cleanup(curl);

    res.setShared(true);
    return gString(res);

}

gString gUda::url_decode(const gString &url){
    CURL *curl;
    curl=curl_easy_init();
    int outlenght;
    char *c=curl_easy_unescape(curl,url.ascii(),url.size(),&outlenght);

    gString res(c);

    curl_free(c);

    curl_easy_cleanup(curl);

    res.setShared(true);
    return gString(res);

}

