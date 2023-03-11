#include <iostream>
#include <map>
#include <vector>

#define TRADE_STATUS_ENUM_MAP(X)     \
    X(BID, 1, "bid more")            \
    X(ASK, 2, "ask more")            \
    X(CANCELED, 3, "cancel")         \
    X(MORE, 4, "More")               \
    X(REDUCED, 5, "REDUCED")         \
    X(NOTHING, 6, "Nothing changed") \
    X(NEW, 7, "New added")

#define X(m, n, s) TRADE_##m##_C = n,
typedef enum __attribute__((packed)) trade_status
{
    TRADE_STATUS_ENUM_MAP(X)
} trade_status_e;
#undef X
#define TRADE_SIDE_ENUM_MAP(X) \
    X(BID, 1, "bid side")      \
    X(ASK, 2, "ask side")      \
    X(NON, 3, "Non")

#define X(m, n, s) SIDE_##m##_C = n,
typedef enum __attribute__((packed)) trade_side
{
    TRADE_SIDE_ENUM_MAP(X)
} trade_side_e;
#undef X

class BOOK
{

private:
    std::map<std::string, int> bid_side;
    std::map<std::string, int> ask_side;
    std::map<std::string, int> delta;
    std::string buffer;
    bool _had_trade;
    int _had_trade_cnt = 0;
    std::string name;
    std::vector<std::pair<std::map<std::string, int>, std::map<std::string, int>>> queue;
    std::string _intention;
    // std::ofstream _intention_file;
    void set_trade(bool trade)
    {
        this->_had_trade = trade;
    }
    void make_intention(trade_status_e status, trade_side_e side_order)
    {

        std::string intention = "";
        std::string prefix = "";
        std::string side = "";
        // solve for side
        switch (side_order)
        {
        case SIDE_BID_C:
            side = "BUY ";
            break;
        case SIDE_ASK_C:
            side = "SELL ";
            break;
        default:
            break;
        }
        if (_had_trade)
        {
            // Indicate AGGRESSIVE
            this->_had_trade = false;
            this->_had_trade_cnt++;
            prefix = "AGGRESSIVE ";
        }
        else
        {
            prefix = "AGGRESSIVE ";
            if (this->_had_trade_cnt > 1)
            {
                this->_had_trade_cnt--;
            }
        }
        if (this->delta.empty())
            return;

        switch (status)
        {

        case TRADE_MORE_C:
            prefix = "PASSIVE ";

            break;
        case TRADE_REDUCED_C:
            if (prefix == "AGGRESSIVE ")
            {
                // indicate previous is trade and buy/sell immediately
            }
            else
            {
                // no previous trade, then should be canceled
                prefix = "CANCELED ";
            }

            break;
        case TRADE_NOTHING_C:
            // there is no new book, this mean 2 trades happened in a row
            // ASSUME SELL/BUY all
            if (this->_had_trade_cnt > 1)
            {
                std::string _price = this->delta.begin()->first;
                int _quantity = this->delta.begin()->second;
                if (this->bid_side.count(_price))
                {
                    intention = "AGGRESSIVE BUY " +
                                std::to_string(_quantity) +
                                " @ " + _price;
                    std::cout << intention << std::endl;
                    // update ref map
                    if (this->bid_side[_price] - _quantity == 0)
                    {
                        this->bid_side.erase(_price);
                    }
                    else
                    {
                        this->bid_side[_price] = this->bid_side[_price] - _quantity;
                    }
                }
                else if (this->ask_side.count(_price))
                {
                    intention = "AGGRESSIVE SELL " +
                                std::to_string(this->delta.begin()->second) +
                                " @ " + _price;
                    std::cout << intention << std::endl;
                    if (this->ask_side[_price] - _quantity == 0)
                    {
                        this->ask_side.erase(_price);
                    }
                    else
                    {

                        this->ask_side[_price] = this->ask_side[_price] - _quantity;
                    }
                }
                this->_had_trade_cnt = 0;
                this->delta.clear();
            }
            return;

        case TRADE_NEW_C:
            prefix = "PASSIVE ";

        default:
            break;
        }
        intention = prefix + side + std::to_string(this->delta.begin()->second) + " @ " + this->delta.begin()->first;
        std::cout << intention << std::endl;
    }
    trade_status_e diff(std::map<std::string, int> source, std::map<std::string, int> target)
    {
        trade_status_e _ret = TRADE_NOTHING_C;
        this->delta.clear();
        for (auto const &[key, val] : target)
        {
            // source already has key, check if any update
            if (source.count(key) > 0)
            {
                int delta = val - source[key];
                if (delta > 0)
                {
                    // bid/ask more...
                    this->delta.insert(std::make_pair(key, abs(delta)));
                    _ret = TRADE_MORE_C;
                    // no need to check more
                    break;
                }
                else if (delta < 0)
                {
                    // cancel or
                    this->delta.insert(std::make_pair(key, abs(delta)));
                    _ret = TRADE_REDUCED_C;
                    // no need to check more
                    break;
                }
                else
                {
                    _ret = TRADE_NOTHING_C;
                    // nothing change
                }
            }
            else
            {
                _ret = TRADE_NEW_C;
                this->delta.insert(std::make_pair(key, val)); // new bid/ask
                break;
            }
        }
        // check if source has but target doesnt have, should be CANCELED
        for (auto const &[key, val] : source)
        {
            if (target.count(key) < 1)
            {
                // canceled all here
                _ret = TRADE_REDUCED_C;
                this->delta.insert(std::make_pair(key, val));
            }
        }
        return {_ret};
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
        // This pair can be bid/ask or trade/dump map
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
            trade_status_e trade_status;
            trade_side_e side = SIDE_NON_C;
            if (it.first.count("trade") < 1)
            { // bid side first
                trade_status = this->diff(this->bid_side, it.first);
                side = SIDE_BID_C;
                // if there is a change in bid side then no need to process this second part.
                if (trade_status == TRADE_NOTHING_C)
                {
                    // process for ask side
                    trade_status = this->diff(this->ask_side, it.second);
                    side = SIDE_ASK_C;
                }
                if (trade_status == TRADE_NOTHING_C)
                {
                    std::cout << "error";
                }
                this->bid_side = it.first;
                this->ask_side = it.second;
            }
            else
            {
                this->_had_trade = true;
                this->delta = it.second;
            }

            this->make_intention(trade_status, side);
        }
    }
};
