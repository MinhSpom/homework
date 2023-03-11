// #include <iostream>
// #include <thread>
// #include <vector>
// #include <numeric>
// #include <future>
// #include "thread_pool.hpp"
// int main() {
//     const size_t num_threads = 4;
//     std::vector<std::future<int>> futures;
//     std::vector<int> data(10000, 1);

//     thread_pool pool(num_threads);

//     for (size_t i = 0; i < 10; ++i) {
//         futures.push_back(pool.submit([&data] {
//             return std::accumulate(data.begin(), data.end(), 0);
//         }));
//     }

//     int sum = 0;
//     for (auto& future : futures) {
//         sum += future.get();
//     }

//     std::cout << "Sum: " << sum << std::endl;

//     return 0;
// }

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include "book.cpp"
#define PASSIVE 0
#define AGGRESSIVE 0
#define CANCELED 0

using json = nlohmann::json;
// struct BOOK
// {
//     std::string symbol = "";
//     json bid;
//     json ask;
//     std::map<float, unsigned int> bid_map;
//     std::map<float, unsigned int> ask_map;
// };

// std::string compare(std::map<float, unsigned int> current, std::map<float, unsigned int>newcomer, bool is_buy)
// {
//     std::string ret ="";
//     if(is_buy)
//     {

//     }
//     else
//     {

//     }
//     //find

//     return;
// }

// int main()
// {
//     // Open the file for reading
//     std::ifstream file("/home/vvu/test_folder/o/afile.txt");

//     // Check if the file was opened successfully
//     if (!file.is_open())
//     {
//         std::cerr << "Failed to open file!" << std::endl;
//         return 1;
//     }
//     BOOK a;

//     // Read the file line by line
//     std::string line;
//     while (std::getline(file, line))
//     {
//         // Parse each line as a JSON object

//         json data = json::parse(line);
//         // std::cout << line << std::endl;
//         // Hey there should be an update
//         if (data.contains("book"))
//         {
//             if (data["book"]["symbol"] != a.symbol)
//             {
//                 a.symbol = data["book"]["symbol"];
//                 std::cout << "Here\n";
//                 continue;
//             }

//             if (data["book"]["bid"] != a.bid && data["book"]["bid"] != NULL)
//             {
//                 std::cout << "Here2\n";
//                 // a.bid = data["book"]["bid"];
//                 auto bid_price_array = data["book"]["bid"].get<std::vector<json>>();
//                 // unsigned int bid_quantity = data["book"]["bid"]["quantity"].get<int>();
//                 for (auto i : bid_price_array)
//                 {
//                     std::cout << i << '\n';
//                 }
//                 // if (!a.bid_map.count(data["book"]["bid"]["price"]))
//                 // {
//                 //     //a.bid_map.insert(std::make_pair(bid_price, bid_quantity));
//                 // }
//                 // std::cout << a.bid << std::endl;
//             }
//             if (data["book"]["ask"] != a.ask && data["book"]["ask"] != NULL)
//             {
//                 a.bid = data["book"]["ask"];
//                 auto ask_price_array = data["book"]["ask"].get<std::vector<json>>();
//             }
//         }
//         else if (data.contains("trade"))
//         {
//         }
//     }

//     // Close the file
//     file.close();

//     return 0;
// }

// class BOOK
// {
//     std::string symbol;
//     std::map<float, int> bid;
//     // float prev_bid;
//     std::map<float, int> ask;
//     json rawContent;

//     // float prev_ask;
//     bool isTrade;
//     BOOK(json rawContentBid, json rawContentAsk)
//     {
//     }

// public:
//     std::string diff(BOOK toDiff)
//     {
//         std::string _ret = "";
//         float deltaAsk;
//         float deltaBid;
//         if (this->isTrade)
//         {
//             _ret += "AGRESSIVE";
//         }
//         else
//         {
//             _ret += "PASSIVE";
//         }
//         if (deltaAsk > 0)
//         {
//             // Have some stocks to trade ....
//         }

//         return _ret;
//     }

// private:
//     std::map<float, int> mapDiff(std::map<float, int> sourceMap, std::map<float, int> toDiffMap)
//     {
//         std::map<float, int> _ret;
//         std::map<float, int> _referencedMap = sourceMap;

//         for (auto const &[key, val] : toDiffMap)
//         {
//             // price already exists...
//             if (sourceMap.count(key) != 0)
//             {
//                 if (val != sourceMap[key])
//                 {
//                     // we have change
//                     _ret.insert(std::make_pair(key, val - sourceMap[key]));
//                     return _ret;
//                 }
//             }
//             // we have some thing diff...
//             else
//             {
//                 _ret.insert(std::make_pair(key, val));
//                 return _ret;
//             }
//         }

//         // loop from source side

//         for (auto const &[key, val] : sourceMap)
//         {
//             if (toDiffMap.count(key) == 0)
//             {
//                 _ret.insert(std::make_pair(key, val));
//                 return _ret;
//             }
//             else
//             {
//                 // nothing to do here
//             }
//         }

//         return _ret;
//     }
// };

static const std::map<std::string, int> trade_event = {std::make_pair("trade", 1)};
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
    std::ofstream _file("KING.json");
    BOOK king("KING");
    while (std::getline(file, line))
    {

        std::string _ret = "";
        std::string _mode = "";
        json data = json::parse(line);
        auto _book = data.find("book");
        auto _isTrade = (data.find("trade") != data.end());

        if (_book != data.end())
        {
            if (data["book"]["symbol"].get<std::string>() == "KING")
            {
                _file << data.dump() << std::endl;
                std::cout << data["book"].dump() << std::endl;
                if (!data["book"]["bid"].empty())
                {

                    _ret = "";

                    std::map<std::string, int> bid;
                    for (auto &[key, val] : data["book"]["bid"].items())
                    {
                        std::string bid_price = val["price"].dump();
                        volatile int bid_quantity = val["quantity"].get<int>();
                        bid_ref.insert((std::make_pair(bid_price, bid_quantity)));
                    }
                }
                if (!data["book"]["ask"].empty())
                {

                    std::map<std::string, int> ask;
                    for (auto &[key, val] : data["book"]["ask"].items())
                    {
                        std::string ask_price = val["price"].dump();
                        int ask_quantity = val["quantity"].get<int>();
                        ask_ref.insert(std::make_pair(ask_price, ask_quantity));
                    }

                    king.push_queue(bid_ref, ask_ref);
                    bid_ref.clear();
                    ask_ref.clear();
                }
                for (auto &[key, val] : bid_ref)
                {
                    if (val == 0)
                    {
                        std::cout << "Here!!!";
                    }
                }
                for (auto &[key, val] : ask_ref)
                {
                    if (val == 0)
                    {
                        std::cout << "Here!!!";
                    }
                }
            }
        }
        else if (_isTrade)
        {
            if (data["trade"]["symbol"].get<std::string>() == "KING")
            {
                got_trade = true;
                _file << data.dump() << std::endl;

                king.push_queue(trade_event, {std::make_pair(data["trade"]["price"].dump(),
                                                             data["trade"]["quantity"].get<int>())});
            }
        }
    }
    std::cout << "======================\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(start - end);
    king.process_queue();
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}