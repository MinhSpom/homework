#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <vector>
#include "book.cpp"
#include <memory>
#include <thread>
#include "thread_pool.cpp"
using json = nlohmann::json;

static const std::map<std::string, int> trade_event = {std::make_pair("trade", 1)};

std::vector<BOOK *> book_tasks;
// std::vector<std::future<int>> futures;
bool test(BOOK *a)
{
    a->process_queue();
    return true;
}
bool is_book_exists(std::string book)
{
    for (auto i : book_tasks)
    {
        if (i->get_book_name() == book)
        {
            return true;
        }
    }
    return false;
}

void push_to_queue(std::string book, std::map<std::string, int> bid, std::map<std::string, int> ask)
{
    for (auto i : book_tasks)
    {
        if (i->get_book_name() == book)
        {
            i->push_queue(bid, ask);
            return;
        }
    }
}
int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file("/home/vvu/test_folder/o/1.txt");
    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
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
            if (!is_book_exists(book_name))
            {

                BOOK *b;
                b = new BOOK(book_name);
                book_tasks.push_back(b);
            }
            if (book_name == "CIMB")
            {
                result << data["book"].dump() << std::endl;
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
            push_to_queue(book_name, bid_ref, ask_ref);
            bid_ref.clear();
            ask_ref.clear();
            // book_tasks[].push_queue(bid_ref, ask_ref);
        }
        else if (_isTrade)
        {

            std::string book_name = data["trade"]["symbol"].get<std::string>();
            if (book_name == "CIMB")
            {
                result << data.dump() << std::endl;
            }
            push_to_queue(book_name, trade_event, {std::make_pair(data["trade"]["price"].dump(), data["trade"]["quantity"].get<int>())});

            // king.push_queue(trade_event, {std::make_pair(data["trade"]["price"].dump(),
            //                                              data["trade"]["quantity"].get<int>())});
        }
    }
    // std::cout << book_tasks.size() << std::endl;
    // std::cout << std::thread::hardware_concurrency() << std::endl;
    // BS::thread_pool _pool;
    thread_pool pool;
    for (auto i : book_tasks)
    {
        // _pool.push_task(&BOOK::process_queue, &i);
        // (pool.execute(test, i));
        std::cout << "*******************\n";
        std::cout << i->get_book_name() << std::endl;
        // if (i->get_book_name() == "CIMB")
        // {
        //     i->process_queue();
        // }
        pool.submit([i]()
                    { i->process_queue(); });
        std::cout << "*******************\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    return 0;
}