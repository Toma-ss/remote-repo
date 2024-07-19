#include "openai.hpp"
#include <iostream>
#include <random>

void playGame()
{
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    std::string userInput;
    std::random_device rd;
    std::mt19937 gen(rd());

    do
    {
        std::uniform_int_distribution<int> dis(1, 3);
        int monsterIndex = dis(gen);

        std::string monster;
        if (monsterIndex == 1)
        {
            monster = "宝箱怪";
        }
        else if (monsterIndex == 2)
        {
            monster = "恐龙";
        }
        else
        {
            monster = "数学老师";
        }

        std::cout << "您遭遇了 " << monster << "，选择战斗（输入 fight）还是逃走（输入 escape）: ";
        std::cin >> userInput;

        if (userInput == "fight")
        {
            std::cout << "您选择了战斗，准备迎接挑战！" << std::endl;
        }
        else if (userInput == "escape")
        {
            std::cout << "您选择了逃走，成功避开了危险！" << std::endl;
        }
        else
        {
            std::cout << "输入错误，请重新输入。" << std::endl;
            continue;
        }

        std::cout << "点击继续（输入 continue）或者退出（输入 exit）: ";
        std::cin >> userInput;

        if (userInput == "continue")
        {
            // 继续下一轮
        }
        else if (userInput == "exit")
        {
            std::cout << "游戏结束，感谢您的参与！" << std::endl;
            return;
        }
        else
        {
            std::cout << "输入错误，请重新输入。" << std::endl;
        }
    } while (userInput != "exit");
}

int main()
{
    std::cout << "请输入 start 开始游戏: ";
    std::string startInput;
    std::cin >> startInput;

    if (startInput == "start")
    {
        playGame();
    }
    else
    {
        std::cout << "输入错误，请重新输入 start 开始游戏。" << std::endl;
    }

    return 0;
}