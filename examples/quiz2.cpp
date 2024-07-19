#include "openai.hpp"
#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

// 提取 JSON 中指定内容的函数
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

// 判断答案是否正确的函数
bool isAnswerCorrect(const nlohmann::json &answerCheck)
{
    if (answerCheck["choices"][0]["message"]["content"].is_string())
    {
        std::string result = answerCheck["choices"][0]["message"]["content"].get<std::string>();
        if (result.find("用户的答案是正确的") != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    openai::start("sk-0DITh75zrxk-8tM6mhckTA", "", true, "https://agino.me/");

    int score = 0;
    int currentPrize = 100;
    bool continuePlaying = true;

    while (continuePlaying)
    {
        nlohmann::json messageSystem = {
            {"role", "system"},
            {"content", "生成一个地理知识单选题，不要给出正确答案，并且不要出现重复，使题目更有多样性"}};

        nlohmann::json messageUser = {
            {"role", "user"},
            {"content", "生成一个地理题目"}};

        nlohmann::json messagesJson = {
            messageSystem,
            messageUser};

        nlohmann::json data = {
            {"model", "gpt-4o"},
            {"messages", messagesJson},
            {"max_tokens", 4096},
            {"temperature", 1}};

        auto completion = openai::chat().create(data);

        // 提取生成题目的内容并输出
        std::string questionContent = getContentFromJson(completion.dump());
        std::cout << "生成结果: " << questionContent << std::endl;

        if (completion.contains("choices") &&
            completion["choices"].is_array() &&
            completion["choices"].size() > 0 &&
            completion["choices"][0].contains("message") &&
            completion["choices"][0]["message"].contains("content") &&
            completion["choices"][0]["message"]["content"].is_string())
        {

            // 不再重复输出问题
            // std::cout << "问题: " << questionContent << std::endl;

            std::cout << "请输入您的答案（仅输入选项字母）: ";
            std::string userAnswer;
            std::cin >> userAnswer;

            nlohmann::json checkMessageSystem = {
                {"role", "system"},
                {"content", "检查用户答案是否正确。"}};

            nlohmann::json checkMessageUser = {
                {"role", "user"},
                {"content", "问题是: " + questionContent + ", 用户的答案是: " + userAnswer}};

            nlohmann::json checkMessagesJson = {
                checkMessageSystem,
                checkMessageUser};

            nlohmann::json checkData = {
                {"model", "gpt-4o"},
                {"messages", checkMessagesJson},
                {"max_tokens", 4096},
                {"temperature", 0}};

            auto answerCheck = openai::chat().create(checkData);

            // 提取答案检查的内容并输出
            std::string answerCheckContent = getContentFromJson(answerCheck.dump());
            std::cout << "答案检查结果: " << answerCheckContent << std::endl;

            if (isAnswerCorrect(answerCheck))
            {
                score++;
                std::cout << "正确！您的分数是: " << score << ", 您赢得 $" << currentPrize << std::endl;

                std::cout << "您要继续吗？(y/n): ";
                char choice;
                std::cin >> choice;

                if (choice == 'n')
                {
                    continuePlaying = false;
                }
                currentPrize *= 2;
            }
            else
            {
                std::cout << "错误！游戏结束。您的最终分数是: " << score << ", 您失去所有奖金。" << std::endl;
                continuePlaying = false;
            }
        }
        else
        {
            std::cout << "获取问题时出错。游戏结束。" << std::endl;
            continuePlaying = false;
        }
    }

    return 0;
}