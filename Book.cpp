#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <fstream>
#include <chrono>

class Book
{

private:
    std::map<std::string, int> bidSide;
    std::map<std::string, int> askSide;
    std::map<std::string, int> tradeQuantity;
    std::string name;
    std::string result;
    std::vector<std::pair<std::map<std::string, int>, std::map<std::string, int>>> queue;

    // find diff, update price<->side
    std::string doDiffAndBuildIntention(std::map<std::string, int> newBid, std::map<std::string, int> newAsk)
    {
        // bid side first
        std::string output = "";
        for (auto &[price, quantity] : newBid)
        {
            // newBid has this price
            if (this->bidSide.count(price) > 0)
            {
                int delta = quantity - this->bidSide[price];
                // bid more
                if (delta > 0)
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        //possible a bid more.
                    }
                    else
                    {
                        output += "PASSIVE BUY " + std::to_string(delta) + " @ " + price + "\n";
                    }
                }
                else if (delta < 0)
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        output += "AGGRESSIVE BUY " + std::to_string(this->tradeQuantity[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        output += "CANCELED BUY" + std::to_string(abs(delta)) + " @ " + price + "\n";
                    }
                }
                else
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        output += "AGGRESSIVE BUY " + std::to_string(this->tradeQuantity[price] + quantity) + " @ " + price + "\n";
                    }
                    else
                    {
                        //nothing to do with this case.
                    }
                }
            }
            else
            {
                if (this->tradeQuantity.count(price) > 0)
                {
                    //this case will be check in ask loop
                }

                else
                {

                    output += "PASSIVE BUY " + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        for (auto const &[price, quantity] : this->bidSide)
        {
            if (newBid.count(price) < 1)
            {

                if (this->tradeQuantity.count(price) > 0)
                {
                    if (newAsk.count(price))
                    {
                        output += "AGGRESSIVE SELL " + std::to_string(quantity + newAsk[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        output += "AGGRESSIVE SELL " + std::to_string(quantity + newBid[price]) + " @ " + price + "\n";
                    }
                }
                else
                {
                    output += "CANCELED BUY " + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        // ask side
        for (auto &[price, quantity] : newAsk)
        {
            // newAsk has this price
            if (this->askSide.count(price) > 0)
            {
                int delta = quantity - this->askSide[price];
                // ask more
                if (delta > 0)
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        //possible an ask more
                    }
                    else
                    {
                        output += "PASSIVE SELL " + std::to_string(delta) + " @ " + price + "\n";
                    }
                }
                else if (delta < 0)
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        output += "AGGRESSIVE SELL " + std::to_string(this->tradeQuantity[price]) + " @ " + price + "\n";
                    }
                    else
                    {
                        output += "CANCELED SELL " + std::to_string(abs(delta)) + " @ " + price + "\n";
                    }
                }
                else
                {
                    if (this->tradeQuantity.count(price) > 0)
                    {
                        output += "AGGRESSIVE BUY " + std::to_string(this->tradeQuantity[price] + quantity) + " @ " + price + "\n";
                    }
                    else
                    {
                        // nothing to do with this case.
                    }
                }
            }
            else
            {
                if (this->tradeQuantity.count(price) > 0)
                {
                    // check before already ...

                }

                else
                {
                    output += "PASSIVE SELL " + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        for (auto const &[price, quantity] : this->askSide)
        {
            if (newAsk.count(price) < 1)
            {
                if (this->tradeQuantity.count(price) > 0)

                {
                    if (newBid.count(price))
                    {
                        output += "AGGRESSIVE BUY " + std::to_string(quantity + newBid[price]) + " @ " + price + "\n";
                    }
                    else
                    {

                        output += "AGGRESSIVE BUY " + std::to_string(quantity + newAsk[price]) + " @ " + price + "\n";
                    }
                }
                else
                {
                    output += "CANCELED SELL" + std::to_string(quantity) + " @ " + price + "\n";
                }
            }
        }
        // std::cout << output;
        this->tradeQuantity.clear();
        return output;
    }

public:
    Book(std::string name)
    {
        this->name = name;
        this->result = "./result/" + name;
    };
    void setNewBook(std::map<std::string, int> bid, std::map<std::string, int> ask)
    {
        this->bidSide = bid;
        this->askSide = ask;
    }
    std::map<std::string, int> getBidSide()
    {
        return this->bidSide;
    }
    std::map<std::string, int> getAskSide()
    {
        return this->askSide;
    }
    std::string getBookName()
    {
        return this->name;
    }
    void pushToQueue(std::map<std::string, int> bid, std::map<std::string, int> ask)
    {
        // This pair can be bid/ask or trade/trade content
        queue.push_back(std::make_pair(bid, ask));
    }
    std::vector<std::pair<std::map<std::string, int>, std::map<std::string, int>>> getCurrentQueue()
    {
        return this->queue;
    }
    void processQueue()
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::ofstream result(this->result, std::ofstream::app);
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
                result << this->doDiffAndBuildIntention(it.first, it.second);
                this->bidSide = it.first;
                this->askSide = it.second;
            }

            else /*Trade event here*/
            {
                // this will update tradeQuantity
                // check if price exists in delta already
                if (tradeQuantity.count(it.second.begin()->first) > 0)
                {
                    tradeQuantity[it.second.begin()->first] += it.second.begin()->second;
                }
                else
                {
                    // insert new
                    tradeQuantity[it.second.begin()->first] = it.second.begin()->second;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Execution time for " << this->getBookName() << " is: " << duration.count() << " microseconds" << std::endl;
    }
};
