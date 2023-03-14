#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <vector>
#include "Book.cpp"
#include <memory>
#include "thread_pool.cpp"

using json = nlohmann::json;

static const std::map<std::string, int> trade_event = {std::make_pair("trade", 1)};

std::vector<Book *> bookCollection;

bool isBookExisted(std::string book)
{
    for (auto i : bookCollection)
    {
        if (i->getBookName() == book)
        {
            return true;
        }
    }
    return false;
}

void pushBookToCollection(std::string book, std::map<std::string, int> bid, std::map<std::string, int> ask)
{
    for (auto i : bookCollection)
    {
        if (i->getBookName() == book)
        {
            i->pushToQueue(bid, ask);
            return;
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Please provide input file.";
        return -1;
    }
    std::string fileName = argv[1];
    std::ifstream file(fileName);
    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::string line;
    std::map<std::string, int> bid_ref;
    std::map<std::string, int> ask_ref;
    bool got_trade = false;
    while (std::getline(file, line))
    {

        std::string _ret = "";
        std::string _mode = "";
        json data = json::parse(line);
        auto _book = data.find("book");
        auto _isTrade = (data.find("trade") != data.end());

        if (_book != data.end())
        {
            std::string book_name = data["book"]["symbol"].get<std::string>();
            if (!isBookExisted(book_name))
            {

                Book *b;
                b = new Book(book_name);
                bookCollection.push_back(b);
            }
            if (!data["book"]["bid"].empty())
            {

                for (auto &[key, val] : data["book"]["bid"].items())
                {
                    std::string bid_price = val["price"].dump();
                    volatile int bid_quantity = val["quantity"].get<int>();
                    bid_ref.insert((std::make_pair(bid_price, bid_quantity)));
                }
            }
            if (!data["book"]["ask"].empty())
            {

                for (auto &[key, val] : data["book"]["ask"].items())
                {
                    std::string ask_price = val["price"].dump();
                    int ask_quantity = val["quantity"].get<int>();
                    ask_ref.insert(std::make_pair(ask_price, ask_quantity));
                }
            }
            pushBookToCollection(book_name, bid_ref, ask_ref);
            bid_ref.clear();
            ask_ref.clear();
        }
        else if (_isTrade)
        {

            std::string book_name = data["trade"]["symbol"].get<std::string>();
            pushBookToCollection(book_name, trade_event, {std::make_pair(data["trade"]["price"].dump(), data["trade"]["quantity"].get<int>())});
        }
    }
    auto endProcessFile = std::chrono::high_resolution_clock::now();
    auto durationProcessFile = std::chrono::duration_cast<std::chrono::milliseconds>(endProcessFile - start);
    std::cout << "Process file time: " << durationProcessFile.count() << " microseconds" << std::endl;
    thread_pool pool;
    for (auto i : bookCollection)
    {
        pool.submit([i]()
                    { i->processQueue(); });
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total time: " << duration.count() << " microseconds" << std::endl;
    return 0;
}