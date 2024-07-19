#include "openai.hpp"
#include <iostream>

int main()
{
    openai::start("sk-0DITh75zrxk-8tM6mhckTA", "", true, "https://agino.me/");

    auto completion = openai::chat().create(R"(
    {
        "model": "gpt-3.5-turbo",
        "messages": [
                {"role":"system","content":""},
                {"role": "user", "content": ""}
                ],
        "max_tokens": 4096,
        "temperature": 0
    }
    )"_json);

    std::cout << "Response is:\n"
              << completion.dump(2) << '\n';

    auto image = openai::image().create({{"prompt", "A logo with a cello in a heart"},
                                         {"n", 1},
                                         {"size", "512x512"}});
    std::cout << "Image URL is: " << image["data"][0]["url"] << '\n';
}