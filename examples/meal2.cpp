#include "openai.hpp"
#include <iostream>
#include <string>

int main()
{
    openai::start("sk-0DITh75zrxk-8tM6mhckTA", "", true, "https://agino.me/");

    std::string dishName;

    std::cout << "提示：如果需要退出，请输入'我吃饱了'。" << std::endl;

    while (true)
    {
        std::cout << "请输入菜名: ";
        std::cin >> dishName;

        if (dishName == "我吃饱了")
        {
            break;
        }

        nlohmann::json messageSystem = {
            {"role", "system"},
            {"content", "请给我一份食谱。"}};

        nlohmann::json messageUser = {
            {"role", "user"},
            {"content", "请告诉我" + dishName + "的做法和准备步骤。"}};

        nlohmann::json messagesJson = {
            messageSystem,
            messageUser};

        nlohmann::json data = {
            {"model", "gpt-4o"},
            {"messages", messagesJson},
            {"max_tokens", 4096},
            {"temperature", 0}};

        auto completion = openai::chat().create(data);

        std::cout << "Response is:\n"
                  << completion.dump(2) << '\n';
    }
}