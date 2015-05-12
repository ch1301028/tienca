#include "NetworkManagement.h"
#include "Utils.h"

const string NetworkManagement::SERVER_DOMAIN = "http://g4m3online.com/fish/index.php/api";

#define COMMON  "common"
#define PAYMENT "payment"
#define SMS     "sms"
#define USER     "user"

NetworkManagement& NetworkManagement::getInstance(){
    static NetworkManagement _instance;
    return _instance;
}




NetworkManagement::NetworkManagement():
_callback(nullptr)
{
   
    
}

NetworkManagement::~NetworkManagement(){
   // httpRequest = NULL;
}


string NetworkManagement::urlEncode(const string &SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i = 0; i<SRC.length(); i++) {
        if (int(SRC[i]) == 37) {
            sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
        else {
            ret += SRC[i];
        }
    }
    return (ret);
}
void NetworkManagement::initData(const ccNetworkCallback& callback)
{
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
   
    document->AddMember("data", myData, allocator);
    document->AddMember("action", "data", allocator);
    
    this->sendPost(COMMON,callback,document);
    
}

void NetworkManagement::requestUserData(const ccNetworkCallback& callback){
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
    // Add charge coin seri & code
    string udid = Utils::getUniqueId();
    myData.AddMember("udid",udid.c_str(),allocator);
    
    document->AddMember("data", myData, allocator);
    document->AddMember("action", "requestUserData", allocator);
    
    this->sendPost(USER,callback,document);
}

void NetworkManagement::requestServerCoin(const ccNetworkCallback& callback){
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
    // Add charge coin seri & code
    string udid = Utils::getUniqueId();
    myData.AddMember("udid",udid.c_str(),allocator);
    
    document->AddMember("data", myData, allocator);
    document->AddMember("action", "requestServerCoin", allocator);
    
    this->sendPost(USER,callback,document);
}

void NetworkManagement::requestCoin(const ccNetworkCallback& callback,string _seri, string _code,string _provider){
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
    // Add charge coin seri & code
    string udid = Utils::getUniqueId();
    myData.AddMember("udid",udid.c_str(),allocator);
    myData.AddMember("seri",_seri.c_str(),allocator);
    myData.AddMember("code",_code.c_str(),allocator);
    myData.AddMember("provider",_provider.c_str(),allocator);
    
    document->AddMember("data", myData, allocator);
    document->AddMember("action", "requestCoin", allocator);
    
    this->sendPost(PAYMENT,callback,document);
}

void NetworkManagement::requestSmsCoin(const ccNetworkCallback& callback,string _device_id){
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
    document->AddMember("action", "requestCoin", allocator);
    
    this->sendPost(SMS,callback,document);
}

void NetworkManagement::requestAd(const ccNetworkCallback& callback,string _seri, string _code){
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    
    document->AddMember("action", "requestCoin", allocator);
    
    this->sendPost(PAYMENT,callback,document);
}


rapidjson::Document* NetworkManagement::createNewDocument(){
    rapidjson::Document *newDoc = new rapidjson::Document;
    
    // Parse an empty JSON objext to initialize the document.
    std::string json = "{  }";
    if (newDoc->Parse<0>(json.c_str()).HasParseError())
    {
        // Oops, something went wrong
    }
    else
    {
        
    }
    
    return newDoc;
}
void NetworkManagement::sendPost(string controller, const ccNetworkCallback& callback,rapidjson::Document* document)
{
    
    _callback = callback;
    
    // Manufacture json document to string
    vector<string> headers;
    headers.push_back("Content-Type: application/json");
    string query = parseJsonDocToString(document);
    auto httpRequest = new HttpRequest();
    // Set response target
    httpRequest->setResponseCallback(CC_CALLBACK_2(NetworkManagement::callbackHttpCompleted,this));
    httpRequest->setRequestType(HttpRequest::Type::POST);
    httpRequest->setHeaders(headers);
    httpRequest->setRequestData(query.c_str(), query.length());
    httpRequest->setTag("POST");
    string request = NetworkManagement::SERVER_DOMAIN + "/" + controller + "/request";
    httpRequest->setUrl(request.c_str());
    
    HttpClient::getInstance()->send(httpRequest);
    httpRequest->release();
}

void NetworkManagement::sendGet(string controller,const ccNetworkCallback& callback,rapidjson::Document* document){
    _callback = callback;
    
    // Manufacture json document to string
    string query = parseJsonDocToString(document);
    
    // Set response target
    auto httpRequest = new HttpRequest();
    httpRequest->setResponseCallback(CC_CALLBACK_2(NetworkManagement::callbackHttpCompleted,this));
    httpRequest->setRequestType(HttpRequest::Type::GET);
    httpRequest->setTag("GET charge");
    string request = NetworkManagement::SERVER_DOMAIN + "/" + controller + "/request" + query;
    httpRequest->setUrl(request.c_str());
    
    HttpClient::getInstance()->send(httpRequest);
    httpRequest->release();
}
string NetworkManagement::parseJsonDocToString(rapidjson::Document * document){
    
    // Convert JSON document to string
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document->Accept(writer);
    
    return strbuf.GetString();
}


void NetworkManagement::callbackHttpCompleted(HttpClient *sender, HttpResponse *response)
{
    rapidjson::Document* document = createNewDocument();
    
    rapidjson::Value myData(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
    rapidjson::Value value;
    if (!response)
    {
        document->AddMember("error", true,allocator);
        document->AddMember("description",response->getErrorBuffer(),allocator);
       
        _callback(document);

        return;
    }
    
    if (!response->isSucceed())
    {
  
        document->AddMember("error", true,allocator);
        document->AddMember("description",response->getErrorBuffer(),allocator);
        _callback(document);
        return;
    }
    
    // Response string data from buffer
    std::vector<char> *buffer = response->getResponseData();
    std::string result(buffer->begin(),buffer->end());
    
   
    rapidjson::Document data;
    CCLOG("%s",result.c_str());
    // Parse string data to document json object
    if (data.Parse<0>(result.c_str()).HasParseError() == false){
        rapidjson::Value &dataValue = data;
        document->AddMember("error", false,allocator);
        document->AddMember("data", dataValue,allocator);
    }
    else{
        document->AddMember("error", true,allocator);
        document->AddMember("description","Wrong json format",allocator);

    }
    
    _callback(document);

}

void NetworkManagement::downloadFile(Ref* target, SEL_HttpResponse reponse,string query){
    
}

