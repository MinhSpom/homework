# Marvel homework

## Building system: 
+ wsl2 Fedora
+ gcc (GCC) 12.2.1 20220819 (Red Hat 12.2.1-2)
+ Visual Studio Code

### Build command
clone this repo
```
cd homework
mkdir build & cd build
cmake ..
```

## Idea overview:

```mermaid
sequenceDiagram
    Main->>Main: Validate input
    Main->>Main: Parse line to json object
    Main->>Main: Create new book if not exist
    Main->>Main: Extract needed info
    Main->>Main: Push info to Book's queue
    Main->>Threadpool: Push Book tasks
    Threadpool->>Threadpool: Start to process Book's queue


```

There are two types of info pushed to queue.
+ New book update
+ Trade event

By doing this,  every event can be in time order. There is a case that has many trade events before book update shows up, to resolve this issue, trade queue is created, which is updated whenever trade event happens and cleared after intention building finishes. 

```mermaid
sequenceDiagram
    Thread->>Book: Issue process queue
    loop Daily query
    Book->>Book: Categorizing event 
    Book->>Book: Build intention  
    Book->>Book: Write result to file 
    end
```



Say that we have current bid{price, quantity}, ask{price, quantity}, new bid{price, quantity}, ask{price, quantity} and trade.


When building intention, logic is as below table:

*More: new/old quantity greater than old/new*\
*Less: new/old quantity less than old/new*\
*Equal: No difference*\
*None: does not have price*\
*Trade yes: price exists in current trade queue*\
*Trade no: price does not exist in current trade queue*

| Trade         | Bid new       | Ask new       | Bid old       | Ask old       | Intention         |
| ------------- | ------------- | ------------- | ------------- | ------------- | -------------     |
| No            | More          | None          | Less          | None          | PASSIVE BUY       |
| No            | More          | None          | None          | None          | PASSIVE BUY       |
| No            | None          | More          | None          | Less          | PASSIVE SELL      |
| No            | None          | More          | None          | None          | PASSIVE SELL      |
| Yes           | None          | Less          | None          | More          | AGGRESSIVE BUY    |
| Yes           | None          | Equal         | None          | Equal         | AGGRESSIVE BUY    |
| Yes           | More          | None          | None          | More          | AGGRESSIVE BUY    |
| Yes           | None          | None          | None          | More          | AGGRESSIVE BUY    |
| Yes           | Less          | None          | More          | None          | AGGRESSIVE SELL   |
| Yes           | Equal         | None          | None          | Equal         | AGGRESSIVE SELL   |
| Yes           | None          | More          | More          | None          | AGGRESSIVE SELL   |
| Yes           | None          | None          | More          | None          | AGGRESSIVE SELL   |
| No            | Less          | None          | More          | None          | CANCELED BUY      |
| No            | None          | None          | More          | None          | CANCELED BUY      |
| No            | None          | Less          | None          | More          | CANCELED SELL     |
| No            | None          | None          | None          | More          | CANCELED SELL     |




## Performance report:
```
Process file time: 2221 milliseconds
Execution time for VNT.BK is: 0 microseconds
Execution time for SLHN is: 0 microseconds
Execution time for VNT-F is: 0 microseconds
Execution time for FORD-F.BK is: 0 microseconds
Execution time for FORD-W3-F is: 0 microseconds
Execution time for CSGN is: 0 microseconds
Execution time for AIY is: 0 microseconds
Execution time for VNT-R.BK is: 0 microseconds
Execution time for TLMK_RG is: 0
Execution time for FORD-R.BK microseconds is: 0 microseconds
Execution time for FORD-W3 is: 0 microseconds
Execution time for VNT-R is: 0 microseconds
Execution time for FORD.BK is: 1 microseconds
Execution time for VNT is: 1 microseconds
Execution time for VNT-F.BK is: 0 microseconds
Execution time for CLN is: 0 microseconds
Execution time for UBSG is: 11 microseconds
Execution time for FORD-W3-R is: 0 microseconds
Execution time for ACES is: 0 microseconds
Execution time for 005930 is: 0 microseconds
Execution time for QUEEN is: 11 microseconds
Execution time for 20 is: 11 microseconds
Execution time for NESN is: 12 microseconds
Execution time for ABBN is: 12 microseconds
Execution time for SMGR is: 12 microseconds
Execution time for TAK is: 13 microseconds
Execution time for GLO is: 13 microseconds
Execution time for CIMB is: 13 microseconds
Execution time for UHR is: 13 microseconds
Execution time for SONG is: 12 microseconds
Execution time for ADEN is: 13 microseconds
Execution time for KING is: 14 microseconds
Execution time for EPMB is: 13 microseconds
Execution time for ZURN is: 15 microseconds
Execution time for REX is: 9 microseconds
Execution time for CFR is: 10 microseconds
Total time: 2244 milliseconds
```
