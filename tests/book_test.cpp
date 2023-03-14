
#include <gtest/gtest.h>
#include <iostream>
#include "../src/Book.cpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(book_test, getBookName)
{
    // Expect book name is ABC
    Book testBook("ABC");
    EXPECT_TRUE("ABC" == testBook.getBookName());
}

TEST(book_test, getCurrentQueue)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    bid["50.15"] = 1000;
    ask["20.15"] = 999;
    Book testBook("ABC");
    testBook.pushToQueue(bid, ask);
    auto ret = testBook.getCurrentQueue();
    EXPECT_TRUE(1000 == ret.begin()->first["50.15"]);
    EXPECT_TRUE(999 == ret.begin()->second["20.15"]);
}

// Test case: new buy
TEST(book_test, new_buy_case)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> askQueueEmpty;
    bid["50.15"] = 1000;
    Book testBook("ABC");
    std::string ret = testBook.doDiffAndBuildIntention(bid, askQueueEmpty);
    std::string expected = "PASSIVE BUY 1000 @ 50.15\n";
    EXPECT_TRUE(expected == ret);
}
// Test case: new sell
TEST(book_test, new_ask_case)
{

    std::map<std::string, int> ask;
    std::map<std::string, int> bidQueueEmpty;
    ask["20.15"] = 1999;
    Book testBook("ABC");

    std::string expected = "PASSIVE SELL 1999 @ 20.15\n";
    std::string ret = testBook.doDiffAndBuildIntention(bidQueueEmpty, ask);
    EXPECT_TRUE(expected == ret);
}
// Test case: aggressive sell
TEST(book_test, aggresive_sell_case)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> tradeInfo;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    tradeInfo["50.13"] = 200;
    bid["50.13"] = 200;
    askNext["50.13"] = 220;
    Book testBook("ABC");
    testBook.setNewBook(bid, ask);
    testBook.setTradeQuantity(tradeInfo);
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "AGGRESSIVE SELL 420 @ 50.13\n";
    EXPECT_TRUE(expected == ret);
}
// Test case: aggressive buy
TEST(book_test, aggresive_buy_case)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> tradeInfo;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    tradeInfo["50.13"] = 200;
    ask["50.13"] = 200;
    bidNext["50.13"] = 220;
    Book testBook("ABC");
    testBook.setNewBook(bid, ask);
    testBook.setTradeQuantity(tradeInfo);
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "AGGRESSIVE BUY 420 @ 50.13\n";
    EXPECT_TRUE(expected == ret);
}

// Test case: aggressive sell with multiple trade event before
TEST(book_test, aggresive_buy_with_multiple_trade_event_before)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> tradeInfo;
    std::map<std::string, int> tradeEvent;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    Book testBook("ABC");
    tradeEvent["trade"] = 1;
    tradeInfo["50.13"] = 220;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 330;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 105;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 345;
    testBook.setTradeQuantity(tradeInfo);
    ask["50.13"] = 1245;
    bid["50.99"] = 200;
    testBook.setNewBook(bid, ask);
    bidNext["50.99"] = 200;
    askNext["50.13"] = 245;
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "AGGRESSIVE BUY 1000 @ 50.13\n";
    EXPECT_TRUE(expected == ret);
}

// Test case: aggressive sell with multiple trade event before
TEST(book_test, aggresive_sell_with_multiple_trade_event_before)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> tradeInfo;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    Book testBook("ABC");
    tradeInfo["50.13"] = 220;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 330;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 105;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo["50.13"] = 345;
    testBook.setTradeQuantity(tradeInfo);
    bid["50.13"] = 1245;
    ask["50.99"] = 200;
    testBook.setNewBook(bid, ask);
    askNext["50.99"] = 200;
    bidNext["50.13"] = 245;
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "AGGRESSIVE SELL 1000 @ 50.13\n";
    EXPECT_TRUE(expected == ret);
}
// Test case: cancel buy
TEST(book_test, cancel_buy)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    Book testBook("ABC");
    bid["50.13"] = 1245;
    ask["50.99"] = 200;
    testBook.setNewBook(bid, ask);
    bidNext["50.13"] = 1000;
    askNext["50.99"] = 200;
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "CANCELED BUY 245 @ 50.13\n";
    EXPECT_TRUE(expected == ret);
}
// Test case: cancel sell
TEST(book_test, cancel_sell)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> askNext;
    Book testBook("ABC");
    bid["50.13"] = 1245;
    ask["50.99"] = 200;
    testBook.setNewBook(bid, ask);
    bidNext["50.13"] = 1245;
    askNext["50.99"] = 50;
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, askNext);
    std::string expected = "CANCELED SELL 150 @ 50.99\n";
    EXPECT_TRUE(expected == ret);
}
// Test case: trade different prices
TEST(book_test, trade_different_prices)
{

    std::map<std::string, int> bid;
    std::map<std::string, int> ask;
    std::map<std::string, int> bidNext;
    std::map<std::string, int> tradeInfo;

    Book testBook("ABC");
    bid["50.13"] = 1000;
    bid["50.15"] = 1000;
    ask["50.99"] = 200;
    testBook.setNewBook(bid, ask);
    tradeInfo["50.13"] = 200;
    testBook.setTradeQuantity(tradeInfo);
    tradeInfo.clear();
    tradeInfo["50.15"] = 200;
    testBook.setTradeQuantity(tradeInfo);

    bidNext["50.13"] = 800;
    bidNext["50.15"] = 800;
    std::string ret = testBook.doDiffAndBuildIntention(bidNext, ask);
    std::string expected = "AGGRESSIVE SELL 200 @ 50.13\nAGGRESSIVE SELL 200 @ 50.15\n";
    EXPECT_TRUE(expected == ret);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}