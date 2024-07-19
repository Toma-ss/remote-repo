#include "openai.hpp"
#include <iostream>
#include <string>
#include <vector>

std::string getContentFromJson(const std::string &jsonStr)
{
    nlohmann::json jsonObject;

    try
    {
        jsonObject = nlohmann::json::parse(jsonStr);

        if (jsonObject.is_object() && jsonObject["choices"].is_array() && jsonObject["choices"][0]["message"].is_object() && jsonObject["choices"][0]["message"]["content"].is_string())
        {
            return jsonObject["choices"][0]["message"]["content"].get<std::string>();
        }
        else
        {
            return "Invalid response format or content not found.";
        }
    }
    catch (const nlohmann::json::parse_error &e)
    {
        return "Failed to parse JSON response: " + std::string(e.what());
    }
}

void solveProblem(const std::string &problemDescription)
{
    openai::start("sk-0DITh75zrxk-8tM6mhckTA", "", true, "https://agino.me/");

    nlohmann::json messageSystem = {
        {"role", "system"},
        {"content", "请根据用户描述给出相应的代码题解。"}};

    nlohmann::json messageUser = {
        {"role", "user"},
        {"content", problemDescription}};

    nlohmann::json messagesJson = {
        messageSystem,
        messageUser};

    nlohmann::json data = {
        {"model", "gpt-4o"},
        {"messages", messagesJson},
        {"max_tokens", 4096},
        {"temperature", 0}};

    auto completion = openai::chat().create(data);

    std::string content = getContentFromJson(completion.dump());
    std::cout << "题解代码：" << content << std::endl;
}

int main()
{
    std::string problemDescription;

    std::cout << "提示：如果需要退出，请输入'退出'。" << std::endl;

    while (true)
    {
        std::cout << "请输入问题描述: ";
        std::getline(std::cin, problemDescription);

        if (problemDescription == "退出")
        {
            break;
        }

        solveProblem(problemDescription);
    }
}