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
                if (got_trade)
                {
                    _mode = "AGGRESSIVE";
                    got_trade = false;
                }
                else
                {
                    _mode = "PASSIVE";
                }
                // std::cout << data["book"].dump() << std::endl;
                if (!data["book"]["bid"].empty())
                {
                    // std::cout << "Got BUY" << std::endl;
                    // std::cout << data["book"]["bid"] << std::endl;
                    _ret = "";

                    std::map<std::string, int> bid;
                    for (auto &[key, val] : data["book"]["bid"].items())
                    {
                        std::string bid_price = val["price"].dump();
                        int bid_quantity = val["quantity"].get<int>();
                        bid.insert((std::make_pair(bid_price, bid_quantity)));
                        if (bid_ref.count(bid_price) > 0)
                        {
                            if (std::abs(bid_quantity - bid_ref[bid_price]) != 0)
                            {
                                //TODO: check _mode, if aggressive then should by buy not cancel
                                if (bid_quantity > bid_ref[bid_price])
                                {

                                    // more bid, find delta
                                    _ret += " BUY ";
                                    _ret += std::to_string(std::abs(bid_quantity - bid_ref[bid_price]));
                                    _ret += " @ ";
                                    _ret += (bid_price);
                                    std::cout << _mode + _ret << std::endl;
                                }
                                else
                                {
                                    // cancel case when new quantity less than old quantity
                                    _ret += "CANCELD BUY ";
                                    _ret += std::to_string(std::abs(bid_quantity - bid_ref[bid_price]));
                                    _ret += " @ ";
                                    _ret += (bid_price);
                                    std::cout << _ret << std::endl;
                                }
                            }
                            else
                            {
                                // nothing change
                            }
                        }
                        else
                        {
                            _ret += " BUY ";
                            _ret += std::to_string(bid_quantity);
                            _ret += " @ ";
                            _ret += (bid_price);
                            std::cout << _mode + _ret << std::endl;
                        }
                    }
                    bid_ref = bid;
                    // bid.clear();
                }
                if (!data["book"]["ask"].empty())
                {
                    // std::cout << "Got BUY" << std::endl;
                    // std::cout << data["book"]["ask"] << std::endl;
                    _ret = "";

                    std::map<std::string, int> ask;
                    for (auto &[key, val] : data["book"]["ask"].items())
                    {
                        std::string ask_price = val["price"].dump();
                        int ask_quantity = val["quantity"].get<int>();
                        ask.insert((std::make_pair(ask_price, ask_quantity)));
                        if (ask_ref.count(ask_price) > 0)
                        {
                            if (std::abs(ask_quantity - ask_ref[ask_price]) != 0)
                            {
                                if (ask_quantity > ask_ref[ask_price])
                                {

                                    // more ask, find delta
                                    _ret += " SELL ";
                                    _ret += std::to_string(std::abs(ask_quantity - ask_ref[ask_price]));
                                    _ret += " @ ";
                                    _ret += (ask_price);
                                    std::cout << _mode + _ret << std::endl;
                                }
                                else
                                {

                                    // cancel case when new quantity less than old quantity
                                    _ret += "CANCELD SELL ";
                                    _ret += std::to_string(std::abs(ask_quantity - bid_ref[ask_price]));
                                    _ret += " @ ";
                                    _ret += (ask_price);
                                    std::cout << _ret << std::endl;
                                }
                            }
                        }
                        else
                        {
                            _ret += " SELL ";
                            _ret += std::to_string(ask_quantity);
                            _ret += " @ ";
                            _ret += (ask_price);
                            std::cout << _mode + _ret << std::endl;
                        }
                    }
                    ask_ref = ask;
                    // bid.clear();
                }

                // ask.clear();
            }
        }
        else if (_isTrade)
        {
            if (data["trade"]["symbol"].get<std::string>() == "KING")
            {
                got_trade = true;
                _file << data.dump() << std::endl;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(start - end);
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}