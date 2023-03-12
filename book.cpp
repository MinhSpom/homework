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
    std::map<std::string, int> new_bid_side;
    std::map<std::string, int> new_ask_side;
    std::map<std::string, int> trade_quantity;
    std::map<std::string, int> delta_quantity;
    std::map<std::string, TRADE_INTENTION> delta_status;
    std::map<std::string, TRADE_SIDE> delta_side;
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
    void erase_delta()
    {
        this->delta_quantity.clear();
        this->delta_status.clear();
        this->delta_side.clear();
        this->trade_quantity.clear();
    }

    // find diff, update price<->side
    std::map<std::string, TRADE_INTENTION> diff(std::map<std::string, int> new_bid, std::map<std::string, int> new_ask)
    {
        std::map<std::string, TRADE_INTENTION> _ret;
        // bid side first
        for (auto &[key, val] : new_bid)
        {
            // new_bid has this price
            if (this->bid_side.count(key) > 0)
            {
                int delta = val - this->bid_side[key];
                // bid more
                if (delta > 0)
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_MORE));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::BID_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, delta));
                }
                else if (delta < 0)
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_REDUCE));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::BID_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, abs(delta)));
                }
                else
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_EQUAL));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::BID_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, delta));
                }
            }
            else
            {
                _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_NEW));
                this->delta_side.insert(std::make_pair(key, TRADE_SIDE::BID_SIDE));
                this->delta_quantity.insert(std::make_pair(key, val));
            }
        }
        for (auto const &[key, val] : this->bid_side)
        {
            if (new_bid.count(key) < 1)
            {
                _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_REDUCE));
                this->delta_side.insert(std::make_pair(key, TRADE_SIDE::BID_SIDE));
                this->delta_quantity.insert(std::make_pair(key, val));
            }
        }
        // ask side
        for (auto &[key, val] : new_ask)
        {
            // new_ask has this price
            if (this->ask_side.count(key) > 0)
            {
                int delta = val - this->ask_side[key];
                // ask more
                if (delta > 0)
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_MORE));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::ASK_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, delta));
                }
                else if (delta < 0)
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_REDUCE));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::ASK_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, abs(delta)));
                }
                else
                {
                    _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_EQUAL));
                    this->delta_side.insert(std::make_pair(key, TRADE_SIDE::ASK_SIDE));
                    this->delta_quantity.insert(std::make_pair(key, delta));
                }
            }
            else
            {
                _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_NEW));
                this->delta_quantity.insert(std::make_pair(key, val));
                this->delta_side.insert(std::make_pair(key, TRADE_SIDE::ASK_SIDE));
            }
        }
        for (auto const &[key, val] : this->ask_side)
        {
            if (new_ask.count(key) < 1)
            {
                _ret.insert(std::make_pair(key, TRADE_INTENTION::TRADE_REDUCE));
                this->delta_side.insert(std::make_pair(key, TRADE_SIDE::ASK_SIDE));
                this->delta_quantity.insert(std::make_pair(key, val));
            }
        }
        return _ret;
    }
    void build_intention(bool out)
    {
        std::string _ret = "";
        std::string to = "";
        for (auto &[price, quantity] : this->trade_quantity)
        {
            // flush trade first
        }
        for (auto &[price, intention] : this->delta_status)
        {

            // if (this->new_bid_side[key] - this->bid_side[key])
            std::string side = "";

            if (delta_side[price] == TRADE_SIDE::ASK_SIDE)
            {

                side = "SELL ";
            }
            else if (delta_side[price] == TRADE_SIDE::BID_SIDE)
            {
                side = "BUY ";
            }
            switch (intention)
            {
            case TRADE_INTENTION::TRADE_MORE:
                // new quantity > current, check if there is a trade before
                if (this->trade_quantity.count(price) > 0)
                {
                    to += "AGGRESSIVE " + side + std::to_string(this->trade_quantity[price] + this->new_bid_side[price]) + " @ " + price + "\n";
                }
                else
                {
                    to += "PASSIVE " + side + std::to_string(this->delta_quantity[price]) + " @ " + price + "\n";
                }
                break;
            case TRADE_INTENTION::TRADE_REDUCE:
                // new quantity < current, check if there is a trade before
                if (this->trade_quantity.count(price) > 0)
                {
                    to += "AGGRESSIVE " + side + std::to_string(this->trade_quantity[price] + this->new_bid_side[price]) + " @ " + price + "\n";
                }
                else
                {

                    to += "CANCELED " + side + std::to_string(this->delta_quantity[price]) + " @ " + price + "\n";
                }
                break;
            case TRADE_INTENTION::TRADE_NEW:
                to += "PASSIVE " + side + std::to_string(this->delta_quantity[price]) + " @ " + price + "\n";
                break;
            case TRADE_INTENTION::TRADE_EQUAL:
                // new quantity == current, check if there is a trade before
                if (this->trade_quantity.count(price) > 0)
                {
                    to += "AGGRESSIVE " + side + std::to_string(this->trade_quantity[price] + this->new_bid_side[price]) + " @ " + price + "\n";
                }
                break;

            default:
                break;
            }
        }
        std::cout << to;
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
                this->new_bid_side = it.first;
                this->new_ask_side = it.second;
                this->delta_status = this->diff(it.first, it.second);

                if (!this->trade_quantity.empty())
                {
                    // trade before
                }
                this->build_intention(true);
                this->bid_side = this->new_bid_side;
                this->ask_side = this->new_ask_side;
                erase_delta();
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
