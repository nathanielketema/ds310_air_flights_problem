# Set and MST 

## Program output of A quizn.air2 problem from the quiz:

----------------------------------------------------------------
1 -> Given airport code, dislay details
2 -> Airports in a airport  (list & count)
3 -> Flights leaving from source (list & count)
4 -> Flights arriving at destination (list & count)
5 -> Flights from source to destination (list & count)
6 -> Flight information ignoring time (fewest flights, cheapest route cost, shortest miles flown)
7 -> All destinations reachable given arrival time(with at most F flights, for under M total fare, within H hours)
8 -> Earliest arrival based on source, destination, and arrival time
9 -> Flights with layover given source, destination, and arrival time (fewest flights, earliest arrival, cheapest trip)
10 -> Minimum Spanning Tree using Prim
0 -> Quit
----------------------------------------------------------------


Enter the number for your choice: 6
Source: DEN
Destination: MIA

Fewest flights from DEN to MIA:

Number of flights: 3
Path: DEN -> LAX -> SEA -> MIA

Cheapest route from DEN to MIA:

Total cost: $1200
Path: DEN -> BOS -> SEA -> MIA

Shortest route (by miles) from DEN to MIA:

Total miles: 1200
Path: DEN -> BOS -> SEA -> MIA

## MST calculation function

```cpp 
void MST()
{
    string start;
    cout << "What is the starting point: ";
    cin >> start;

    vector<string> subM;
    string port;
    cout << "Enter the number of aiports in subset: ";
    int num;
    cin >> num;
    cout << "Enter the aiports subset: ";
    for(int i = 0; i < num; i++)
    {
        cin >> port;
        subM.push_back(port);
    }


    unordered_map< string, int> mapped;
    unordered_map< int, string > reverse;

    int index = 0;
    for(const auto& port : subM)
    {
        mapped[port] = index;
        reverse[index] = port;
        index++;
    }

    vector <Edge> edges;
    set visited;
    visited += mapped[start];
    int total = 0;
    int check = 1;
    while(visited.get_size() != subM.size())
    {
        Edge current;
        string next;
        int local_min = INT_MAX;
        for(const auto& u : visited.list)
        {
            for(const auto& item : subM)
            {
                if(flights.find(reverse[u]) == flights.end() || flights[reverse[u]].find(item) == flights[reverse[u]].end())
                    continue;
                for(const auto& flight : flights[reverse[u]][item])
                {
                    if(flight.cost < local_min && !(mapped[item]^visited))  //checks the cost and if the next edge is already part of our tree
                    {
                        local_min = flight.cost;
                        current.source = reverse[u];
                        current.destination = item;
                        current.cost = local_min;
                        next = item;  //we keep updating this then we keep the lowest and add to our set
                    }
                }
            }
        }
        if(local_min == INT_MAX)
        {
            cout << "There is no MST for this subset!" << endl;
            return;
        }
        total += local_min;
        edges.push_back(current);
        visited += mapped[next];  //mapped is the unordered map that we get when we change the string to an int
        start = next;

        for(const auto& index : visited.list)
        cout << reverse[index] << " ";
        cout << endl;
    }

    cout << "The order for the MST is: ";

    for(const auto& index : visited.list)
        cout << reverse[index] << " ";
    cout << endl << "Total MST cost: " << total << endl;
    for(const auto& edge : edges)
        cout << edge.source << " -> " << edge.destination << " Cost: " << edge.cost << endl;
}
```

## Explnation of the above code

The MST is implemented in the air2.cpp file, so some of the data structures and variables are referenced from that. 

It asks for a subgraph from the user with a starting airport. It then creates a hash table for the airport codes, mapping them to an index for an easier operation and also does the opposite to make it easier to look up what index is referencing an airport. 

It adds the starting airport index to the visited list to start the algorithm, and it goes on until the visited list is equal in size to the given subgraph. 

We set the local min to a large number, basically infinity, and we go to the main for loop. The loop goes through every element in the visited set, and for every element in the given subgraph, it checks, for optimization purposes, if there are any flights between the source and destination. If there are none, then there is no need to proceed; it continues, but if there is a flight, it checks every flight from the given source to destination. This includes all the options to get from source to destination. It then finds the cheapest cost from the available flights and updates the local min and the edges. If after that big loop the local min stays infinity, it means it hasn't found a minimum spanning tree for the given subgraph. If it did succeed updating the local min, then it adds it to the visited and goes back to the while loop.

Finally, it prints the minimum spanning tree and the total cost.


