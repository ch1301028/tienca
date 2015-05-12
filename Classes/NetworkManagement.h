#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "external/json/document.h"		// rapidjson's DOM-style API
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::network;

typedef std::function<void(rapidjson::Document *)> ccNetworkCallback;

class NetworkManagement:public Ref{
public:
    NetworkManagement();
    virtual ~NetworkManagement();
    
    static NetworkManagement			&getInstance();
    static string urlEncode(const string &value);
    
    static const string CMD_CHARGE;
    static const string SERVER_DOMAIN;
    
    void requestCoin(const ccNetworkCallback& callback, string seri,string code, string provider);
    void requestAd(const ccNetworkCallback& callback, string,string);
    void requestSmsCoin(const ccNetworkCallback& callback,string);
    void requestUserData(const ccNetworkCallback &callback);
    void requestServerCoin(const ccNetworkCallback &callback);
    
    void initData(const ccNetworkCallback& callback);
    
private:
    static string parseJsonDocToString(rapidjson::Document* document);
    static rapidjson::Document* createNewDocument();
    void sendGet(string, const ccNetworkCallback& , rapidjson::Document*);
    void sendPost(string, const ccNetworkCallback& , rapidjson::Document*);
    
    void downloadFile(Ref *ref, SEL_HttpResponse reponse, string query);
    void callbackHttpCompleted(HttpClient *sender, HttpResponse *response);
    
    

    ccNetworkCallback _callback;
};

#endif
