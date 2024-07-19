#include "openai.hpp"
#include <iostream>
using namespace std;
using json = nlohmann::json;

void geneate_response()
{
    openai::start("sk-0DITh75zrxk-8tM6mhckTA", "", true, "https://agino.me/");

    auto completion = openai::chat().create(R"(
    {
        "model": "gpt-3.5-turbo",
        "messages": [
                {"role":"system","content":"你叫泽连斯基，是乌克兰的总统。"},
                {"role": "user", "content": "回来吧刺激战场。"}
                ],
        "max_tokens": 4096,
        "temperature": 0
    }
    )"_json);

    // cout << "Response is:\n"
    //      << completion.dump(2) << '\n';     //这里使用的是dump，即传入的是一个python对象，通过dump将其转化成了io流；如果使用dumps，就可以将其转化成str形式存储

    // string tempStr = completion.dump();         //dump能够将json对象转化为string格式
    // string temp = "content";
    // int pianyi = tempStr.find(temp);
    // string* pos = &tempStr+pianyi+10;
    // while(*pos!='"')

    string tempStr = completion.dump();
    string temp = "content";
    int pianyi = tempStr.find(temp);

    if (pianyi != std::string::npos)
    {
        string contentStr = tempStr.substr(pianyi + 9);
        size_t endQuote = contentStr.find('"');
        if (endQuote != std::string::npos)
        {
            contentStr = contentStr.substr(0, endQuote);
            std::cout << "Content: " << contentStr << std::endl;
        }
    }
}

int main()
{
    geneate_response();
    return 0;
}