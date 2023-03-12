#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <fstream>

enum class TRADE_SIDE
{
    BID_SIDE,
    ASK_SIDE,
    NON_SIDE
};
enum class TRADE_INTENTION
{
    TRADE_MORE,
    TRADE_REDUCE,
    TRADE_NEW,
    TRADE_EQUAL
};

class BOOK
{

private:
    std::map<std::string, int> bid_side;
    std::map<std::string, int> ask_side;
    std::map<std::string, int> trade_quantity;
    std::string buffer;
    bool _had_trade;
    bool _is_done;
    int _had_trade_cnt = 0;
    std::string name;
    std::vector<std::pair<std::map<std::string, int>, std::map<std::string, int>>> queue;
    std::string _intention;
    // std::ofstream _intention_file;
    void set_trade(bool trade)
    {
        this->_had_trade = trade;
    }
    // find diff, update price<->side
    void do_diff(std::map<std::string, int> new_bid, std::map<std::string, int> new_ask)
    {
        // bid side first
        std::string to_c = "";
        for (auto &[price, quantity] : new_bid)
        {
            // new_bid has this price
            if (this->bid_side.count(price) > 0)
            {
                int delta = quantity - this->bid_side[price];
                // bid more
                if (delta > 0)
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        // to_c += "AGGRESSIVE " + std::to_string(this->delta_quantity[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        to_c += "PASSIVE BUY " + std::to_string(delta) + " @ " + price + "\n";
                    }
                }
                else if (delta < 0)
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        to_c += "AGGRESSIVE BUY" + std::to_string(this->trade_quantity[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        to_c += "CANCELED BUY" + std::to_string(abs(delta)) + " @ " + price + "\n";
                    }

                }
                else
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        to_c += "AGGRESSIVE BUY" + std::to_string(this->trade_quantity[price] + quantity) + " @ " + price + "\n";
                    }
                    else
                    {
                        //std::cout << "NO THING\n";
                    }
                }
            }
            else
            {
                if (this->trade_quantity.count(price) > 0)
                {
                    // std::cout << "BID:::SOME THING WRONG, new has, old doesnt, trade has\n";
                    // std::cout <<price +  std::to_string(quantity) + "\n";
                }

                else
                {

                    to_c += "PASSIVE BUY " + std::to_string(quantity) + " @ " + price + "\n";
                }

            }
        }
        for (auto const &[price, quantity] : this->bid_side)
        {
            if (new_bid.count(price) < 1)
            {

                if (this->trade_quantity.count(price) > 0)
                {
                    if (new_ask.count(price))
                    {
                        to_c += "AGGRESSIVE SELL " + std::to_string(quantity + new_ask[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        to_c += "AGGRESSIVE SELL " + std::to_string(quantity + new_bid[price]) + " @ " + price + "\n";
                    }
                }
                else
                {
                    to_c += "CANCELED BUY " + std::to_string(quantity) + " @ " + price + "\n";
                }

            }
        }
        // ask side
        for (auto &[price, quantity] : new_ask)
        {
            // new_ask has this price
            if (this->ask_side.count(price) > 0)
            {
                int delta = quantity - this->ask_side[price];
                // ask more
                if (delta > 0)
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        // to_c += "AGGRESSIVE SELL" + std::to_string(quantity) + " @ " + price + "\n";
                    }
                    else
                    {
                        to_c += "PASSIVE SELL " + std::to_string(delta) + " @ " + price + "\n";
                    }

                }
                else if (delta < 0)
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        to_c += "AGGRESSIVE SELL " + std::to_string(this->trade_quantity[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        to_c += "CANCELED SELL " + std::to_string(abs(delta)) + " @ " + price + "\n";
                    }
                }
                else
                {
                    if (this->trade_quantity.count(price) > 0)
                    {
                        to_c += "AGGRESSIVE BUY " + std::to_string(this->trade_quantity[price] + quantity) + " @ " + price + "\n";
                    }
                    else
                    {
                        //std::cout << "NO THING\n";
                    }
                }
            }
            else
            {
                if (this->trade_quantity.count(price) > 0)
                {
                    //check after ...
                }

                else
                {
                    to_c += "PASSIVE SELL " + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        for (auto const &[price, quantity] : this->ask_side)
        {
            if (new_ask.count(price) < 1)
            {
                if (this->trade_quantity.count(price) > 0)

                {
                    if (new_bid.count(price))
                    {
                        to_c += "AGGRESSIVE BUY " + std::to_string(quantity + new_bid[price]) + " @ " + price + "\n";

                    }
                    else
                    {
                        
                    to_c += "AGGRESSIVE BUY " + std::to_string(quantity + new_ask[price]) + " @ " + price + "\n";
                    }
                    
                }
                else
                {
                    to_c += "CANCELED SELL" + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        std::cout << to_c;
        this->trade_quantity.clear();
    }

public:
    BOOK(std::string name)
    {
        this->name = name;
    };
    void set_new(std::map<std::string, int> bid, std::map<std::string, int> ask)
    {
        this->bid_side = bid;
        this->ask_side = ask;
    }
    bool is_process_done()
    {
        return this->_is_done;
    }
    std::map<std::string, int> get_bid_side()
    {
        return this->bid_side;
    }
    std::map<std::string, int> get_ask_side()
    {
        return this->ask_side;
    }
    std::string get_book_name()
    {
        return this->name;
    }
    bool had_trade()
    {
        return this->_had_trade;
    }

    void push_queue(std::map<std::string, int> bid, std::map<std::string, int> ask)
    {
        // This pair can be bid/ask or trade/trade content
        queue.push_back(std::make_pair(bid, ask));
    }
    void get_current_queue()
    {
        for (auto &it : this->queue)
        {

            std::cout << "BID SIDE=====>\n";
            for (auto const &[key, val] : it.first)
            {
                std::cout << "Key: " << key << std::endl;
                std::cout << "val: " << val << std::endl;
            }
            std::cout << "ASK SIDE=====>\n";
            for (auto const &[key, val] : it.second)
            {
                std::cout << "Key: " << key << std::endl;
                std::cout << "val: " << val << std::endl;
            }
        }
    }
    void process_queue()
    {
        for (auto &it : this->queue)
        {
            // check if book update or trade event
            if (it.first.count("trade") < 1)
            {
                // check if delta has something
                // only need to check 1 of 3 delta, because they are erased at once
                // nothing, so this should be update book only
                // find diff between new bid/ask vs current bid/ask
                // assign new to old
                this->do_diff(it.first, it.second);
                this->bid_side = it.first;
                this->ask_side = it.second;
            }

            else /*Trade event here*/
            {
                // this will update trade_quantity
                // check if price exists in delta already
                if (trade_quantity.count(it.second.begin()->first) > 0)
                {
                    trade_quantity[it.second.begin()->first] += it.second.begin()->second;
                }
                else
                {
                    // insert new
                    trade_quantity[it.second.begin()->first] = it.second.begin()->second;
                }
            }
        }

        // result.close();
        this->_is_done = true;
    }
};
