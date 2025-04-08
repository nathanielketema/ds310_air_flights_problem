#include <iostream>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <set>
#include <algorithm> // for reverse
#include <climits>   // for INT_MAX
#include <sstream>   // for stringstream
using namespace std;

struct AirportDetails {
    string FAA;
    string name;
    string city;
    string state;
};

struct FlightDetails {
    string source;
    string destination;
    int depTime;
    int arrTime;
    int cost;
    int miles;
    string airline;
    string flightID;
};

typedef unordered_map< string, vector<string> > mapState;
typedef unordered_map< string, AirportDetails > mapAirport;
typedef unordered_map< string, vector<FlightDetails> > flightGraph;

void displayMenu();
void storeListOfAiports(ifstream &file);
void storeSampleFlights(ifstream &file);
void display_using_airport_code(string code);
void airports_in_state(string state);
void flights_leaving(string source);
void flights_arriving(string destination);
void flights_source_to_destination(string source, string destination);
void fewest_flights(string source, string destination);
void cheapest_route(string source, string destination); 
void shortest_miles(string source, string destination);
void reachable_destinations(string source, int startTime, int maxFlights, int maxFare, int maxHours);
void earliest_arrival(string source, string destination, int startTime);
void layover_routes(string source, string destination, string middle, int startTime);

mapState stateTable;
mapAirport airports;
flightGraph flights;

struct PathNode {
    string airport;
    vector<string> path;
    int totalCost;
    int totalMiles;
    int numFlights;
    
    PathNode(string ap) : airport(ap), totalCost(0), totalMiles(0), numFlights(0) {
        path.push_back(ap);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Error: files not provided!" << endl;
        return 1;
    }

    ifstream listOfAirports, sampleFlights;
    listOfAirports.open(argv[1]);
    sampleFlights.open(argv[2]);


    storeListOfAiports(listOfAirports);
    storeSampleFlights(sampleFlights);
    displayMenu();

    return 0;
}

void displayMenu()
{
    int choice;
    cout << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "1 -> Given airport code, dislay details" << endl;
    cout << "2 -> Airports in a airport  (list & count)" << endl;
    cout << "3 -> Flights leaving from source (list & count)" << endl;
    cout << "4 -> Flights arriving at destination (list & count)" << endl;
    cout << "5 -> Flights from source to destination (list & count)" << endl;
    cout << "6 -> Flight information ignoring time (fewest flights, cheapest route cost, shortest miles flown)" << endl;
    cout << "7 -> All destinations reachable given arrival time(with at most F flights, for under M total fare, within H hours)" << endl;
    cout << "8 -> Earliest arrival based on source, destination, and arrival time"<< endl;
    cout << "9 -> Flights with layover given source, destination, and arrival time (fewest flights, earliest arrival, cheapest trip)" << endl;
    cout << "0 -> Quit" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << endl;

    do {
        cout << endl << "Enter the number for your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string airportCode;
                cout << "Enter airport code: ";
                cin >> airportCode;
                display_using_airport_code(airportCode);
                break;
            }
            case 2: {
                string state;
                cout << "State: ";
                cin >> state;
                airports_in_state(state);
                break;
            }
            case 3: {
                string source;
                cout << "Source: ";
                cin >> source;
                flights_leaving(source);
                break;
            }
            case 4: {
                string destination;
                cout << "Destination: ";
                cin >> destination;
                flights_arriving(destination);
                break;
            }
            case 5: {
                string source, destination;
                cout << "Source: ";
                cin >> source;
                cout << "Destination: ";
                cin >> destination;
                flights_source_to_destination(source, destination);
                break;
            }
            case 6: {
                string source, destination;
                cout << "Source: ";
                cin >> source;
                cout << "Destination: ";
                cin >> destination;
                fewest_flights(source, destination);
                cheapest_route(source, destination); 
                shortest_miles(source, destination);
                break; 
            }
            case 7: {
                string source;
                int startTime = 0, maxFlights = INT_MAX, maxFare = INT_MAX, maxHours = INT_MAX;
                string input;
                
                cout << "Source: ";
                cin >> source;
                
                cout << "Starting Time (24hr format, e.g. 1430 for 2:30pm, or press Enter to skip): ";
                cin.ignore(); // Clear newline
                getline(cin, input);
                if (!input.empty()) {
                    startTime = stoi(input);
                }
                
                cout << "Maximum number of flights (or press Enter for unlimited): ";
                getline(cin, input);
                if (!input.empty()) {
                    maxFlights = stoi(input);
                }
                
                cout << "Maximum total fare ($ or press Enter for unlimited): ";
                getline(cin, input);
                if (!input.empty()) {
                    maxFare = stoi(input);
                }
                
                cout << "Maximum hours (or press Enter for unlimited): ";
                getline(cin, input);
                if (!input.empty()) {
                    maxHours = stoi(input);
                }
                
                reachable_destinations(source, startTime, maxFlights, maxFare, maxHours);
                break; 
            }
            case 8: {
                string source, destination;
                int startTime;
                cout << "Source: ";
                cin >> source;
                cout << "Destination: ";
                cin >> destination;
                cout << "Starting Time (24hr format, e.g. 1430 for 2:30pm): ";
                cin >> startTime;
                earliest_arrival(source, destination, startTime);
                break; 
            }
            case 9: {
                string source, destination, middle;
                int startTime;
                cout << "Source: ";
                cin >> source;
                cout << "Middle (required layover): ";
                cin >> middle;
                cout << "Destination: ";
                cin >> destination;
                cout << "Starting Time (24hr format, e.g. 1430 for 2:30pm): ";
                cin >> startTime;
                layover_routes(source, destination, middle, startTime);
                break; 
            }
            case 0:
                cout << "Program exiting..." << endl;
                exit(1);
                break;
            default:
                cout << "Incorrect input choice!" << endl;
        }
    } while (true);
}

void storeListOfAiports(ifstream &file)
{
    string word, key, value;
    AirportDetails data;

    // Airport code to AirportDetails
    while (getline(file, word, ',')) {
        data.FAA = word;
        getline(file, word, ',');
        data.name = word;
        getline(file, word, ',');
        data.state = word;
        getline(file, word);
        data.city = word;

        airports[data.FAA] = data;
    }

    // State to Airport Code
    for (const auto& airport : airports) {
        stateTable[airport.second.state].push_back(airport.first);
    }
}

void storeSampleFlights(ifstream &file)
{
        string word;
        FlightDetails flight;

        while(getline(file, word, ' ')) {
                flight.source = word;

                getline(file, flight.destination, ' ');

                getline(file, word, ' ');
                flight.depTime = stoi(word);

                getline(file, word, ' ');
                flight.arrTime = stoi(word);

                getline(file, word, ' ');
                flight.cost = stoi(word);

                getline(file, word, ' ');
                flight.miles = stoi(word);

                getline(file, flight.airline, ' ');
                getline(file, flight.flightID);

                flights[flight.source].push_back(flight);
        }
}

void display_using_airport_code(string code)
{
    if (airports.find(code) == airports.end()) {
        cout << "Airport not found!!!" << endl;
        return;
    }
    cout << endl;

    cout << "Name: " << airports[code].name << endl;
    cout << "City: " << airports[code].city << endl;
    cout << "State: " << airports[code].state << endl;
}

void airports_in_state(string state)
{
    if (stateTable.find(state) == stateTable.end()) {
        cout << "State not found!!!" << endl;
        return;
    }
    cout << endl;

    int count = 0;
    for (const auto& flight : stateTable[state])
    {
        cout << flight << endl;
        count++;
    }
    cout << endl << "There are " << count << " states in " << state << endl;
}

void flights_leaving(string source)
{
    if (flights.find(source) == flights.end()) {
        cout << source << " not found!!" << endl;
        return;
    }
    cout << endl;

    int count = 0;
    for (const auto& dest : flights[source]) {
        cout << dest.flightID << endl;
        count++;
    }

    cout << endl << "There are " << count << " flights leaving " << source << endl;
}

void flights_arriving(string destination)
{
    int count = 0;
    bool found = false;
    cout << endl;
    for (const auto& entry : flights) {
        const string& source = entry.first;
        const vector<FlightDetails>& flightList = entry.second;
        for (const auto& flight : flightList) {
            if (flight.destination == destination) {
                cout << flight.flightID << endl;
                found = true;
                count++;
            }
        }
    }

    if (!found) {
        cout << endl << "No flights found arriving at airport: " << destination << endl;
    }

    cout << endl << "There are " << count << " flights arriving at " << destination << endl;
}

void flights_source_to_destination(string source, string destination)
{
    if (flights.find(source) == flights.end()) {
        cout << "Source not found!!!" << endl;
        return;
    }
    cout << endl;

    int count = 0;
    for (const auto& flight : flights[source]) {
        if (flight.destination == destination) {
            cout << flight.flightID << endl;
            count++;
        }
    }

    cout << endl << "There are " << count << " flights from " << source << " to " << destination << endl;
}

void fewest_flights(string source, string destination)
{
    if (airports.find(source) == airports.end() || airports.find(destination) == airports.end()) {
        cout << "Source or destination airport not found!" << endl;
        return;
    }
    
    // BFS to find the shortest path (fewest flights)
    queue<PathNode> q;
    set<string> visited;
    
    PathNode start(source);
    q.push(start);
    visited.insert(source);
    
    bool found = false;
    PathNode result = start;
    
    while (!q.empty() && !found) {
        PathNode current = q.front();
        q.pop();
        
        if (current.airport == destination) {
            found = true;
            result = current;
            break;
        }
        
        // Check all flights from current airport
        if (flights.find(current.airport) != flights.end()) {
            for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                 flight_it != flights[current.airport].end(); ++flight_it) {
                const FlightDetails& flight = *flight_it;
                if (visited.find(flight.destination) == visited.end()) {
                    PathNode next = current;
                    next.airport = flight.destination;
                    next.path.push_back(flight.destination);
                    next.numFlights++;
                    next.totalCost += flight.cost;
                    next.totalMiles += flight.miles;
                    
                    q.push(next);
                    visited.insert(flight.destination);
                }
            }
        }
    }
    
    cout << endl;
    if (found) {
        cout << "Fewest flights from " << source << " to " << destination << ":" << endl;
        cout << endl << "Number of flights: " << result.numFlights << endl;
        cout << "Path: ";
        for (size_t i = 0; i < result.path.size(); i++) {
            cout << result.path[i];
            if (i < result.path.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "No route found from " << source << " to " << destination << endl;
    }
}

void cheapest_route(string source, string destination)
{
    if (airports.find(source) == airports.end() || airports.find(destination) == airports.end()) {
        cout << "Source or destination airport not found!" << endl;
        return;
    }
    
    // Dijkstra's algorithm to find the cheapest route
    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    set< pair<int, string> > pq; // priority queue (cost, airport)
    
    // Initialize distances
    for (mapAirport::const_iterator it = airports.begin(); it != airports.end(); ++it) {
        dist[it->first] = numeric_limits<int>::max();
    }
    dist[source] = 0;
    pq.insert(make_pair(0, source));
    
    while (!pq.empty()) {
        pair<int, string> current = *pq.begin();
        int current_cost = current.first;
        string current_airport = current.second;
        pq.erase(pq.begin());
        
        if (current_airport == destination) break;
        
        if (flights.find(current_airport) != flights.end()) {
            for (vector<FlightDetails>::const_iterator flight_it = flights[current_airport].begin(); 
                 flight_it != flights[current_airport].end(); ++flight_it) {
                const FlightDetails& flight = *flight_it;
                int new_cost = current_cost + flight.cost;
                
                if (new_cost < dist[flight.destination]) {
                    // Can't erase directly by value, need to find the iterator first
                    set< pair<int, string> >::iterator it = pq.find(make_pair(dist[flight.destination], flight.destination));
                    if (it != pq.end()) {
                        pq.erase(it);
                    }
                    dist[flight.destination] = new_cost;
                    prev[flight.destination] = current_airport;
                    pq.insert(make_pair(new_cost, flight.destination));
                }
            }
        }
    }
    
    cout << endl;
    if (dist[destination] == numeric_limits<int>::max()) {
        cout << "No route found from " << source << " to " << destination << endl;
    } else {
        // Reconstruct path
        vector<string> path;
        string current = destination;
        
        while (current != source) {
            path.push_back(current);
            current = prev[current];
        }
        path.push_back(source);
        
        reverse(path.begin(), path.end());
        
        cout << "Cheapest route from " << source << " to " << destination << ":" << endl;
        cout << endl << "Total cost: $" << dist[destination] << endl;
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
}

void shortest_miles(string source, string destination)
{
    if (airports.find(source) == airports.end() || airports.find(destination) == airports.end()) {
        cout << "Source or destination airport not found!" << endl;
        return;
    }
    
    // Dijkstra's algorithm to find the shortest route by miles
    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    set< pair<int, string> > pq; // priority queue (miles, airport)
    
    // Initialize distances
    for (mapAirport::const_iterator it = airports.begin(); it != airports.end(); ++it) {
        dist[it->first] = numeric_limits<int>::max();
    }
    dist[source] = 0;
    pq.insert(make_pair(0, source));
    
    while (!pq.empty()) {
        pair<int, string> current = *pq.begin();
        int current_miles = current.first;
        string current_airport = current.second;
        pq.erase(pq.begin());
        
        if (current_airport == destination) break;
        
        if (flights.find(current_airport) != flights.end()) {
            for (vector<FlightDetails>::const_iterator flight_it = flights[current_airport].begin(); 
                 flight_it != flights[current_airport].end(); ++flight_it) {
                const FlightDetails& flight = *flight_it;
                int new_miles = current_miles + flight.miles;
                
                if (new_miles < dist[flight.destination]) {
                    // Can't erase directly by value, need to find the iterator first
                    set< pair<int, string> >::iterator it = pq.find(make_pair(dist[flight.destination], flight.destination));
                    if (it != pq.end()) {
                        pq.erase(it);
                    }
                    dist[flight.destination] = new_miles;
                    prev[flight.destination] = current_airport;
                    pq.insert(make_pair(new_miles, flight.destination));
                }
            }
        }
    }
    
    cout << endl;
    if (dist[destination] == numeric_limits<int>::max()) {
        cout << "No route found from " << source << " to " << destination << endl;
    } else {
        // Reconstruct path
        vector<string> path;
        string current = destination;
        
        while (current != source) {
            path.push_back(current);
            current = prev[current];
        }
        path.push_back(source);
        
        reverse(path.begin(), path.end());
        
        cout << "Shortest route (by miles) from " << source << " to " << destination << ":" << endl;
        cout << endl << "Total miles: " << dist[destination] << endl;
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
}

void reachable_destinations(string source, int startTime, int maxFlights, int maxFare, int maxHours)
{
    if (airports.find(source) == airports.end()) {
        cout << "Source airport not found!" << endl;
        return;
    }
    
    // Show special message for unlimited/default values
    stringstream ss;
    
    string startTimeStr;
    if (startTime == 0) {
        startTimeStr = "any time";
    } else {
        ss << startTime;
        startTimeStr = ss.str();
        ss.str(""); // Clear stringstream
    }
    
    string maxFlightsStr;
    if (maxFlights == INT_MAX) {
        maxFlightsStr = "unlimited";
    } else {
        ss << maxFlights;
        maxFlightsStr = ss.str();
        ss.str(""); // Clear stringstream
    }
    
    string maxFareStr;
    if (maxFare == INT_MAX) {
        maxFareStr = "unlimited";
    } else {
        ss << "$" << maxFare;
        maxFareStr = ss.str();
        ss.str(""); // Clear stringstream
    }
    
    string maxHoursStr;
    if (maxHours == INT_MAX) {
        maxHoursStr = "unlimited";
    } else {
        ss << maxHours;
        maxHoursStr = ss.str();
        ss.str(""); // Clear stringstream
    }
    
    // Convert maxHours to minutes to make time calculations easier
    int maxMinutes = (maxHours == INT_MAX) ? INT_MAX : maxHours * 60;
    
    // Store reachable destinations with their details
    set<string> reachableDestinations;
    unordered_map<string, vector<string> > paths;
    unordered_map<string, int> bestCosts;
    unordered_map<string, int> bestTimes;
    unordered_map<string, int> flightCounts;
    
    // Using BFS to find reachable destinations within constraints
    queue<PathNode> q;
    PathNode startNode(source);
    startNode.totalCost = 0;
    startNode.numFlights = 0;
    q.push(startNode);
    
    while (!q.empty()) {
        PathNode current = q.front();
        q.pop();
        
        // Skip if we've exceeded any constraints
        if (current.numFlights > maxFlights || current.totalCost > maxFare) {
            continue;
        }
        
        // For all outgoing flights from current airport
        if (flights.find(current.airport) != flights.end()) {
            for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                 flight_it != flights[current.airport].end(); ++flight_it) {
                const FlightDetails& flight = *flight_it;
                
                // Time calculations
                int flightDeptTime = flight.depTime;
                int flightArrTime = flight.arrTime;
                
                // Skip flights that depart before the starting time at the first airport
                // Only check this if startTime is not the default value (0)
                if (current.airport == source && startTime != 0 && flightDeptTime < startTime) {
                    continue;
                }
                
                // Calculate time difference in minutes
                int deptHour = flightDeptTime / 100;
                int deptMin = flightDeptTime % 100;
                int arrHour = flightArrTime / 100;
                int arrMin = flightArrTime % 100;
                
                // Calculate flight time in minutes
                int flightTimeMinutes = (arrHour * 60 + arrMin) - (deptHour * 60 + deptMin);
                // Handle overnight flights
                if (flightTimeMinutes < 0) {
                    flightTimeMinutes += 24 * 60; // Add a day in minutes
                }
                
                // Calculate total time so far
                int totalTime = 0;
                if (current.airport == source) {
                    if (startTime != 0) {
                        int startHour = startTime / 100;
                        int startMin = startTime % 100;
                        // Time waiting at source + flight time
                        totalTime = ((deptHour - startHour) * 60 + (deptMin - startMin)) + flightTimeMinutes;
                        if (totalTime < 0) {
                            totalTime += 24 * 60; // Add a day if waiting crosses midnight
                        }
                    } else {
                        // If no start time specified, only count flight time
                        totalTime = flightTimeMinutes;
                    }
                } else {
                    // Add flight time to accumulated time
                    totalTime = bestTimes[current.airport] + flightTimeMinutes;
                    
                    // Add connection time (minimum 1 minute)
                    int lastArrivalTime = 0;
                    if (!current.path.empty() && current.path.size() > 1) {
                        string prevAirport = current.path[current.path.size() - 2];
                        for (vector<FlightDetails>::const_iterator f_it = flights[prevAirport].begin(); 
                             f_it != flights[prevAirport].end(); ++f_it) {
                            const FlightDetails& f = *f_it;
                            if (f.destination == current.airport) {
                                lastArrivalTime = f.arrTime;
                                break;
                            }
                        }
                    }
                    
                    if (lastArrivalTime > 0) {
                        int lastArrHour = lastArrivalTime / 100;
                        int lastArrMin = lastArrivalTime % 100;
                        
                        int connectionTimeMinutes = (deptHour * 60 + deptMin) - (lastArrHour * 60 + lastArrMin);
                        if (connectionTimeMinutes < 0) {
                            connectionTimeMinutes += 24 * 60; // Add a day if connection crosses midnight
                        }
                        
                        totalTime += connectionTimeMinutes;
                    }
                }
                
                // Skip if total time exceeds max hours
                if (totalTime > maxMinutes) {
                    continue;
                }
                
                // Create new path node
                PathNode next = current;
                next.airport = flight.destination;
                next.path.push_back(flight.destination);
                next.numFlights = current.numFlights + 1;
                next.totalCost = current.totalCost + flight.cost;
                
                // Add/update destination if it meets all constraints
                if ((reachableDestinations.find(flight.destination) == reachableDestinations.end()) || 
                    (bestCosts[flight.destination] > next.totalCost) ||
                    (flightCounts[flight.destination] > next.numFlights) ||
                    (bestTimes[flight.destination] > totalTime)) {
                    
                    // Update best values
                    reachableDestinations.insert(flight.destination);
                    bestCosts[flight.destination] = next.totalCost;
                    bestTimes[flight.destination] = totalTime;
                    flightCounts[flight.destination] = next.numFlights;
                    
                    // Save path
                    vector<string> pathVector;
                    for (size_t i = 0; i < next.path.size(); i++) {
                        pathVector.push_back(next.path[i]);
                    }
                    paths[flight.destination] = pathVector;
                    
                    // Continue exploring from this destination if we haven't reached max flights
                    if (next.numFlights < maxFlights) {
                        q.push(next);
                    }
                }
            }
        }
    }
    
    // Display results
    cout << endl;
    cout << "Reachable destinations from " << source << " starting at " << startTimeStr << ":" << endl;
    cout << "Constraints: Max flights = " << maxFlightsStr << ", Max fare = " << maxFareStr << ", Max hours = " << maxHoursStr << endl;
    cout << endl;
    
    if (reachableDestinations.empty()) {
        cout << "No destinations reachable within the given constraints." << endl;
        return;
    }
    
    cout << "Found " << reachableDestinations.size() << " reachable destinations:" << endl;
    cout << endl;
    cout << "Destination\tFlights\tCost\tTime(hrs)" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for (set<string>::const_iterator it = reachableDestinations.begin(); it != reachableDestinations.end(); ++it) {
        const string& dest = *it;
        cout << dest << "\t\t" << flightCounts[dest] << "\t$" << bestCosts[dest] 
             << "\t" << (bestTimes[dest] / 60) << "h " << (bestTimes[dest] % 60) << "m" << endl;
        
        cout << "Path: ";
        for (size_t i = 0; i < paths[dest].size(); i++) {
            cout << paths[dest][i];
            if (i < paths[dest].size() - 1) cout << " -> ";
        }
        cout << endl << endl;
    }
}

void earliest_arrival(string source, string destination, int startTime)
{
    if (airports.find(source) == airports.end() || airports.find(destination) == airports.end()) {
        cout << "Source or destination airport not found!" << endl;
        return;
    }
    
    // Structure to store time, airport code, and path
    struct TimePathNode {
        string airport;
        int arrivalTime;      // Arrival time at current airport
        vector<string> path;  // Path of airports
        vector<int> depTimes; // Departure times
        vector<int> arrTimes; // Arrival times
        vector<string> flightIDs; // Flight IDs
        
        TimePathNode(string ap, int time) 
            : airport(ap), arrivalTime(time) {
            path.push_back(ap);
        }
    };
    
    // Custom comparison for priority queue - earliest arrival time first
    struct CompareArrivalTime {
        bool operator()(const TimePathNode& a, const TimePathNode& b) {
            return a.arrivalTime > b.arrivalTime;
        }
    };
    
    // Using priority queue with custom comparison
    priority_queue<TimePathNode, vector<TimePathNode>, CompareArrivalTime> pq;
    set<string> visited;
    
    // Start node representing being at source airport at startTime
    TimePathNode startNode(source, startTime);
    pq.push(startNode);
    
    // To store the result
    bool found = false;
    TimePathNode result = startNode;
    
    while (!pq.empty() && !found) {
        TimePathNode current = pq.top();
        pq.pop();
        
        // If we've reached the destination, we have the earliest arrival
        if (current.airport == destination) {
            found = true;
            result = current;
            break;
        }
        
        // Skip if we've already processed this airport (with earlier arrival)
        if (visited.find(current.airport) != visited.end()) {
            continue;
        }
        
        visited.insert(current.airport);
        
        // Check all outgoing flights from current airport
        if (flights.find(current.airport) != flights.end()) {
            for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                 flight_it != flights[current.airport].end(); ++flight_it) {
                const FlightDetails& flight = *flight_it;
                
                // Skip flights that depart before our arrival at this airport
                int currentTime = current.arrivalTime;
                if (flight.depTime < currentTime) {
                    // Assuming no overnight waiting (would need to add a day's worth of time)
                    continue;
                }
                
                // Calculate minimum connection time (1 minute)
                int minConnectionTime = 1; // minutes
                int currentHour = currentTime / 100;
                int currentMin = currentTime % 100;
                int departureHour = flight.depTime / 100;
                int departureMin = flight.depTime % 100;
                
                int connectionTimeMinutes = (departureHour - currentHour) * 60 + (departureMin - currentMin);
                
                // Skip if connection time is too short
                if (connectionTimeMinutes < minConnectionTime) {
                    continue;
                }
                
                // Create new TimePathNode for this flight
                TimePathNode next = current;
                next.airport = flight.destination;
                next.arrivalTime = flight.arrTime;
                next.path.push_back(flight.destination);
                next.depTimes.push_back(flight.depTime);
                next.arrTimes.push_back(flight.arrTime);
                next.flightIDs.push_back(flight.flightID);
                
                pq.push(next);
            }
        }
    }
    
    cout << endl;
    if (found) {
        cout << "Earliest arrival from " << source << " to " << destination 
             << " when starting at " << startTime << ":" << endl << endl;
        
        cout << "Arrival time: " << result.arrivalTime << endl;
        
        cout << "Itinerary:" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "From\tTo\tDeparture\tArrival\t\tFlight" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (size_t i = 0; i < result.path.size() - 1; i++) {
            cout << result.path[i] << "\t" 
                 << result.path[i+1] << "\t" 
                 << result.depTimes[i] << "\t\t" 
                 << result.arrTimes[i] << "\t\t";
            
            // Find and print the flight ID
            vector<FlightDetails>& flightsFromSource = flights[result.path[i]];
            for (vector<FlightDetails>::const_iterator f_it = flightsFromSource.begin(); 
                 f_it != flightsFromSource.end(); ++f_it) {
                const FlightDetails& flight = *f_it;
                if (flight.destination == result.path[i+1] && 
                    flight.depTime == result.depTimes[i] && 
                    flight.arrTime == result.arrTimes[i]) {
                    cout << flight.flightID;
                    break;
                }
            }
            cout << endl;
        }
        
        cout << "------------------------------------------------------------" << endl;
        cout << "Total legs: " << (result.path.size() - 1) << endl;
    } else {
        cout << "No route found from " << source << " to " << destination 
             << " when starting at " << startTime << endl;
    }
}

// Helper function to format time from 24-hour format to a more readable format
string formatTime(int time) {
    int hours = time / 100;
    int minutes = time % 100;
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
    return string(buffer);
}

// Helper struct to store route information
struct RouteResult {
    vector<string> airports;
    vector<int> departures;
    vector<int> arrivals;
    vector<string> flightIDs;
    int totalFlights;
    int finalArrival;
    int totalCost;
    
    RouteResult() : totalFlights(0), finalArrival(0), totalCost(0) {}
    
    void printItinerary() const {
        cout << "------------------------------------------------------------" << endl;
        cout << "From\tTo\tDeparture\tArrival\t\tFlight" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (size_t i = 0; i < airports.size() - 1; i++) {
            cout << airports[i] << "\t" 
                 << airports[i+1] << "\t" 
                 << formatTime(departures[i]) << "\t\t" 
                 << formatTime(arrivals[i]) << "\t\t"
                 << flightIDs[i] << endl;
        }
        
        cout << "------------------------------------------------------------" << endl;
        cout << "Total flights: " << totalFlights << endl;
        cout << "Total cost: $" << totalCost << endl;
        cout << "Final arrival time: " << formatTime(finalArrival) << endl;
    }
};

void layover_routes(string source, string destination, string middle, int startTime)
{
    // Check if all airports exist
    if (airports.find(source) == airports.end() || 
        airports.find(destination) == airports.end() || 
        airports.find(middle) == airports.end()) {
        cout << "One or more airports not found!" << endl;
        return;
    }
    
    // Check if source, middle, and destination are different
    if (source == middle || source == destination || middle == destination) {
        cout << "Source, middle, and destination must be different airports!" << endl;
        return;
    }
    
    cout << endl << "Finding routes from " << source << " to " << destination 
         << " with required layover at " << middle 
         << " starting at " << formatTime(startTime) << "..." << endl << endl;
    
    // First, find routes from source to middle
    cout << "Step 1: Finding routes from " << source << " to " << middle << endl;
    
    // Structure to store time, airport code, and path (similar to earliest_arrival)
    struct TimePathNode {
        string airport;
        int arrivalTime;      // Arrival time at current airport
        vector<string> path;  // Path of airports
        vector<int> depTimes; // Departure times
        vector<int> arrTimes; // Arrival times
        vector<string> flightIDs; // Flight IDs
        int totalCost;        // Total cost
        
        TimePathNode(string ap, int time) 
            : airport(ap), arrivalTime(time), totalCost(0) {
            path.push_back(ap);
        }
    };
    
    // Custom comparison for priority queue - earliest arrival time first
    struct CompareArrivalTime {
        bool operator()(const TimePathNode& a, const TimePathNode& b) {
            return a.arrivalTime > b.arrivalTime;
        }
    };
    
    // Custom comparison for priority queue - fewest flights first
    struct CompareFlights {
        bool operator()(const TimePathNode& a, const TimePathNode& b) {
            if (a.path.size() != b.path.size())
                return a.path.size() > b.path.size();
            return a.arrivalTime > b.arrivalTime; // Break ties with arrival time
        }
    };
    
    // Custom comparison for priority queue - cheapest cost first
    struct CompareCost {
        bool operator()(const TimePathNode& a, const TimePathNode& b) {
            if (a.totalCost != b.totalCost)
                return a.totalCost > b.totalCost;
            return a.arrivalTime > b.arrivalTime; // Break ties with arrival time
        }
    };
    
    // Find route from source to middle
    TimePathNode startNode(source, startTime);
    
    // Find the best routes based on different criteria
    RouteResult fewestFlightsRoute;
    RouteResult earliestArrivalRoute;
    RouteResult cheapestRoute;
    
    // Find route with fewest flights from source to middle
    {
        priority_queue<TimePathNode, vector<TimePathNode>, CompareFlights> pq;
        set<string> visited;
        pq.push(startNode);
        bool found = false;
        
        while (!pq.empty() && !found) {
            TimePathNode current = pq.top();
            pq.pop();
            
            if (current.airport == middle) {
                // Save result to intermediate source->middle
                TimePathNode sourceToMiddle = current;
                
                // Now find route from middle to destination
                priority_queue<TimePathNode, vector<TimePathNode>, CompareFlights> pq2;
                set<string> visited2;
                TimePathNode middleNode(middle, current.arrivalTime);
                pq2.push(middleNode);
                bool found2 = false;
                
                while (!pq2.empty() && !found2) {
                    TimePathNode current2 = pq2.top();
                    pq2.pop();
                    
                    if (current2.airport == destination) {
                        // We've found a complete route
                        found2 = true;
                        found = true;
                        
                        // Combine the routes
                        fewestFlightsRoute.airports = sourceToMiddle.path;
                        fewestFlightsRoute.departures = sourceToMiddle.depTimes;
                        fewestFlightsRoute.arrivals = sourceToMiddle.arrTimes;
                        fewestFlightsRoute.flightIDs = sourceToMiddle.flightIDs;
                        fewestFlightsRoute.totalCost = sourceToMiddle.totalCost;
                        
                        // Add middle->destination (skipping first node which is middle)
                        for (size_t i = 1; i < current2.path.size(); i++) {
                            fewestFlightsRoute.airports.push_back(current2.path[i]);
                        }
                        for (size_t i = 0; i < current2.depTimes.size(); i++) {
                            fewestFlightsRoute.departures.push_back(current2.depTimes[i]);
                            fewestFlightsRoute.arrivals.push_back(current2.arrTimes[i]);
                            fewestFlightsRoute.flightIDs.push_back(current2.flightIDs[i]);
                        }
                        
                        fewestFlightsRoute.totalFlights = fewestFlightsRoute.departures.size();
                        fewestFlightsRoute.finalArrival = current2.arrivalTime;
                        fewestFlightsRoute.totalCost += current2.totalCost;
                        break;
                    }
                    
                    if (visited2.find(current2.airport) != visited2.end()) {
                        continue;
                    }
                    
                    visited2.insert(current2.airport);
                    
                    // Check all outgoing flights
                    if (flights.find(current2.airport) != flights.end()) {
                        for (vector<FlightDetails>::const_iterator flight_it = flights[current2.airport].begin(); 
                             flight_it != flights[current2.airport].end(); ++flight_it) {
                            const FlightDetails& flight = *flight_it;
                            
                            // Handle flights that depart before our arrival by assuming next day departure
                            int currentTime = current2.arrivalTime;
                            int adjustedDepTime = flight.depTime;
                            bool isNextDay = false;
                            
                            if (flight.depTime < currentTime) {
                                // Assume this flight departs the next day
                                isNextDay = true;
                            }
                            
                            // Calculate connection time (minimum 1 minute)
                            int minConnectionTime = 1; // minutes
                            int currentHour = currentTime / 100;
                            int currentMin = currentTime % 100;
                            int departureHour = adjustedDepTime / 100;
                            int departureMin = adjustedDepTime % 100;
                            
                            int connectionTimeMinutes;
                            if (isNextDay) {
                                // Add 24 hours (1440 minutes) for next day
                                connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                            } else {
                                connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                            }
                            
                            // Skip if connection time is too short
                            if (connectionTimeMinutes < minConnectionTime) {
                                continue;
                            }
                            
                            // Create new path node
                            TimePathNode next = current2;
                            next.airport = flight.destination;
                            next.arrivalTime = flight.arrTime;
                            if (isNextDay && flight.arrTime < flight.depTime) {
                                // If it's an overnight flight after an overnight connection, add another day
                                // This is a simplification but should work for most cases
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            } else if (isNextDay) {
                                // If it's just an overnight connection but not an overnight flight
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            }
                            next.path.push_back(flight.destination);
                            next.depTimes.push_back(flight.depTime);
                            next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                            next.flightIDs.push_back(flight.flightID);
                            next.totalCost += flight.cost;
                            
                            pq2.push(next);
                        }
                    }
                }
                
                // If we found a complete route, break out of the outer loop too
                if (found) break;
            }
            
            if (visited.find(current.airport) != visited.end()) {
                continue;
            }
            
            visited.insert(current.airport);
            
            // Check all outgoing flights
            if (flights.find(current.airport) != flights.end()) {
                for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                     flight_it != flights[current.airport].end(); ++flight_it) {
                    const FlightDetails& flight = *flight_it;
                    
                    // Handle flights that depart before our arrival by assuming next day departure
                    int currentTime = current.arrivalTime;
                    int adjustedDepTime = flight.depTime;
                    bool isNextDay = false;
                    
                    if (flight.depTime < currentTime) {
                        // Assume this flight departs the next day
                        isNextDay = true;
                    }
                    
                    // Calculate connection time (minimum 1 minute)
                    int minConnectionTime = 1; // minutes
                    int currentHour = currentTime / 100;
                    int currentMin = currentTime % 100;
                    int departureHour = adjustedDepTime / 100;
                    int departureMin = adjustedDepTime % 100;
                    
                    int connectionTimeMinutes;
                    if (isNextDay) {
                        // Add 24 hours (1440 minutes) for next day
                        connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                    } else {
                        connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                    }
                    
                    // Skip if connection time is too short
                    if (connectionTimeMinutes < minConnectionTime) {
                        continue;
                    }
                    
                    // Create new path node
                    TimePathNode next = current;
                    next.airport = flight.destination;
                    next.arrivalTime = flight.arrTime;
                    if (isNextDay && flight.arrTime < flight.depTime) {
                        // If it's an overnight flight after an overnight connection, add another day
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    } else if (isNextDay) {
                        // If it's just an overnight connection but not an overnight flight
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    }
                    next.path.push_back(flight.destination);
                    next.depTimes.push_back(flight.depTime);
                    next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                    next.flightIDs.push_back(flight.flightID);
                    next.totalCost += flight.cost;
                    
                    pq.push(next);
                }
            }
        }
    }
    
    // Find route with earliest arrival from source to middle
    {
        priority_queue<TimePathNode, vector<TimePathNode>, CompareArrivalTime> pq;
        set<string> visited;
        pq.push(startNode);
        bool found = false;
        
        while (!pq.empty() && !found) {
            TimePathNode current = pq.top();
            pq.pop();
            
            if (current.airport == middle) {
                // Save result to intermediate source->middle
                TimePathNode sourceToMiddle = current;
                
                // Now find route from middle to destination
                priority_queue<TimePathNode, vector<TimePathNode>, CompareArrivalTime> pq2;
                set<string> visited2;
                TimePathNode middleNode(middle, current.arrivalTime);
                pq2.push(middleNode);
                bool found2 = false;
                
                while (!pq2.empty() && !found2) {
                    TimePathNode current2 = pq2.top();
                    pq2.pop();
                    
                    if (current2.airport == destination) {
                        // We've found a complete route
                        found2 = true;
                        found = true;
                        
                        // Combine the routes
                        earliestArrivalRoute.airports = sourceToMiddle.path;
                        earliestArrivalRoute.departures = sourceToMiddle.depTimes;
                        earliestArrivalRoute.arrivals = sourceToMiddle.arrTimes;
                        earliestArrivalRoute.flightIDs = sourceToMiddle.flightIDs;
                        earliestArrivalRoute.totalCost = sourceToMiddle.totalCost;
                        
                        // Add middle->destination (skipping first node which is middle)
                        for (size_t i = 1; i < current2.path.size(); i++) {
                            earliestArrivalRoute.airports.push_back(current2.path[i]);
                        }
                        for (size_t i = 0; i < current2.depTimes.size(); i++) {
                            earliestArrivalRoute.departures.push_back(current2.depTimes[i]);
                            earliestArrivalRoute.arrivals.push_back(current2.arrTimes[i]);
                            earliestArrivalRoute.flightIDs.push_back(current2.flightIDs[i]);
                        }
                        
                        earliestArrivalRoute.totalFlights = earliestArrivalRoute.departures.size();
                        earliestArrivalRoute.finalArrival = current2.arrivalTime;
                        earliestArrivalRoute.totalCost += current2.totalCost;
                        break;
                    }
                    
                    if (visited2.find(current2.airport) != visited2.end()) {
                        continue;
                    }
                    
                    visited2.insert(current2.airport);
                    
                    // Check all outgoing flights
                    if (flights.find(current2.airport) != flights.end()) {
                        for (vector<FlightDetails>::const_iterator flight_it = flights[current2.airport].begin(); 
                             flight_it != flights[current2.airport].end(); ++flight_it) {
                            const FlightDetails& flight = *flight_it;
                            
                            // Handle flights that depart before our arrival by assuming next day departure
                            int currentTime = current2.arrivalTime;
                            int adjustedDepTime = flight.depTime;
                            bool isNextDay = false;
                            
                            if (flight.depTime < currentTime) {
                                // Assume this flight departs the next day
                                isNextDay = true;
                            }
                            
                            // Calculate connection time (minimum 1 minute)
                            int minConnectionTime = 1; // minutes
                            int currentHour = currentTime / 100;
                            int currentMin = currentTime % 100;
                            int departureHour = adjustedDepTime / 100;
                            int departureMin = adjustedDepTime % 100;
                            
                            int connectionTimeMinutes;
                            if (isNextDay) {
                                // Add 24 hours (1440 minutes) for next day
                                connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                            } else {
                                connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                            }
                            
                            // Skip if connection time is too short
                            if (connectionTimeMinutes < minConnectionTime) {
                                continue;
                            }
                            
                            // Create new path node
                            TimePathNode next = current2;
                            next.airport = flight.destination;
                            next.arrivalTime = flight.arrTime;
                            if (isNextDay && flight.arrTime < flight.depTime) {
                                // If it's an overnight flight after an overnight connection, add another day
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            } else if (isNextDay) {
                                // If it's just an overnight connection but not an overnight flight
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            }
                            next.path.push_back(flight.destination);
                            next.depTimes.push_back(flight.depTime);
                            next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                            next.flightIDs.push_back(flight.flightID);
                            next.totalCost += flight.cost;
                            
                            pq2.push(next);
                        }
                    }
                }
                
                // If we found a complete route, break out of the outer loop too
                if (found) break;
            }
            
            if (visited.find(current.airport) != visited.end()) {
                continue;
            }
            
            visited.insert(current.airport);
            
            // Check all outgoing flights
            if (flights.find(current.airport) != flights.end()) {
                for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                     flight_it != flights[current.airport].end(); ++flight_it) {
                    const FlightDetails& flight = *flight_it;
                    
                    // Handle flights that depart before our arrival by assuming next day departure
                    int currentTime = current.arrivalTime;
                    int adjustedDepTime = flight.depTime;
                    bool isNextDay = false;
                    
                    if (flight.depTime < currentTime) {
                        // Assume this flight departs the next day
                        isNextDay = true;
                    }
                    
                    // Calculate connection time (minimum 1 minute)
                    int minConnectionTime = 1; // minutes
                    int currentHour = currentTime / 100;
                    int currentMin = currentTime % 100;
                    int departureHour = adjustedDepTime / 100;
                    int departureMin = adjustedDepTime % 100;
                    
                    int connectionTimeMinutes;
                    if (isNextDay) {
                        // Add 24 hours (1440 minutes) for next day
                        connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                    } else {
                        connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                    }
                    
                    // Skip if connection time is too short
                    if (connectionTimeMinutes < minConnectionTime) {
                        continue;
                    }
                    
                    // Create new path node
                    TimePathNode next = current;
                    next.airport = flight.destination;
                    next.arrivalTime = flight.arrTime;
                    if (isNextDay && flight.arrTime < flight.depTime) {
                        // If it's an overnight flight after an overnight connection, add another day
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    } else if (isNextDay) {
                        // If it's just an overnight connection but not an overnight flight
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    }
                    next.path.push_back(flight.destination);
                    next.depTimes.push_back(flight.depTime);
                    next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                    next.flightIDs.push_back(flight.flightID);
                    next.totalCost += flight.cost;
                    
                    pq.push(next);
                }
            }
        }
    }
    
    // Find cheapest route from source to middle
    {
        priority_queue<TimePathNode, vector<TimePathNode>, CompareCost> pq;
        set<string> visited;
        pq.push(startNode);
        bool found = false;
        
        while (!pq.empty() && !found) {
            TimePathNode current = pq.top();
            pq.pop();
            
            if (current.airport == middle) {
                // Save result to intermediate source->middle
                TimePathNode sourceToMiddle = current;
                
                // Now find route from middle to destination
                priority_queue<TimePathNode, vector<TimePathNode>, CompareCost> pq2;
                set<string> visited2;
                TimePathNode middleNode(middle, current.arrivalTime);
                pq2.push(middleNode);
                bool found2 = false;
                
                while (!pq2.empty() && !found2) {
                    TimePathNode current2 = pq2.top();
                    pq2.pop();
                    
                    if (current2.airport == destination) {
                        // We've found a complete route
                        found2 = true;
                        found = true;
                        
                        // Combine the routes
                        cheapestRoute.airports = sourceToMiddle.path;
                        cheapestRoute.departures = sourceToMiddle.depTimes;
                        cheapestRoute.arrivals = sourceToMiddle.arrTimes;
                        cheapestRoute.flightIDs = sourceToMiddle.flightIDs;
                        cheapestRoute.totalCost = sourceToMiddle.totalCost;
                        
                        // Add middle->destination (skipping first node which is middle)
                        for (size_t i = 1; i < current2.path.size(); i++) {
                            cheapestRoute.airports.push_back(current2.path[i]);
                        }
                        for (size_t i = 0; i < current2.depTimes.size(); i++) {
                            cheapestRoute.departures.push_back(current2.depTimes[i]);
                            cheapestRoute.arrivals.push_back(current2.arrTimes[i]);
                            cheapestRoute.flightIDs.push_back(current2.flightIDs[i]);
                        }
                        
                        cheapestRoute.totalFlights = cheapestRoute.departures.size();
                        cheapestRoute.finalArrival = current2.arrivalTime;
                        cheapestRoute.totalCost += current2.totalCost;
                        break;
                    }
                    
                    if (visited2.find(current2.airport) != visited2.end()) {
                        continue;
                    }
                    
                    visited2.insert(current2.airport);
                    
                    // Check all outgoing flights
                    if (flights.find(current2.airport) != flights.end()) {
                        for (vector<FlightDetails>::const_iterator flight_it = flights[current2.airport].begin(); 
                             flight_it != flights[current2.airport].end(); ++flight_it) {
                            const FlightDetails& flight = *flight_it;
                            
                            // Handle flights that depart before our arrival by assuming next day departure
                            int currentTime = current2.arrivalTime;
                            int adjustedDepTime = flight.depTime;
                            bool isNextDay = false;
                            
                            if (flight.depTime < currentTime) {
                                // Assume this flight departs the next day
                                isNextDay = true;
                            }
                            
                            // Calculate connection time (minimum 1 minute)
                            int minConnectionTime = 1; // minutes
                            int currentHour = currentTime / 100;
                            int currentMin = currentTime % 100;
                            int departureHour = adjustedDepTime / 100;
                            int departureMin = adjustedDepTime % 100;
                            
                            int connectionTimeMinutes;
                            if (isNextDay) {
                                // Add 24 hours (1440 minutes) for next day
                                connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                            } else {
                                connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                            }
                            
                            // Skip if connection time is too short
                            if (connectionTimeMinutes < minConnectionTime) {
                                continue;
                            }
                            
                            // Create new path node
                            TimePathNode next = current2;
                            next.airport = flight.destination;
                            next.arrivalTime = flight.arrTime;
                            if (isNextDay && flight.arrTime < flight.depTime) {
                                // If it's an overnight flight after an overnight connection, add another day
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            } else if (isNextDay) {
                                // If it's just an overnight connection but not an overnight flight
                                next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                            }
                            next.path.push_back(flight.destination);
                            next.depTimes.push_back(flight.depTime);
                            next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                            next.flightIDs.push_back(flight.flightID);
                            next.totalCost += flight.cost;
                            
                            pq2.push(next);
                        }
                    }
                }
                
                // If we found a complete route, break out of the outer loop too
                if (found) break;
            }
            
            if (visited.find(current.airport) != visited.end()) {
                continue;
            }
            
            visited.insert(current.airport);
            
            // Check all outgoing flights
            if (flights.find(current.airport) != flights.end()) {
                for (vector<FlightDetails>::const_iterator flight_it = flights[current.airport].begin(); 
                     flight_it != flights[current.airport].end(); ++flight_it) {
                    const FlightDetails& flight = *flight_it;
                    
                    // Handle flights that depart before our arrival by assuming next day departure
                    int currentTime = current.arrivalTime;
                    int adjustedDepTime = flight.depTime;
                    bool isNextDay = false;
                    
                    if (flight.depTime < currentTime) {
                        // Assume this flight departs the next day
                        isNextDay = true;
                    }
                    
                    // Calculate connection time (minimum 1 minute)
                    int minConnectionTime = 1; // minutes
                    int currentHour = currentTime / 100;
                    int currentMin = currentTime % 100;
                    int departureHour = adjustedDepTime / 100;
                    int departureMin = adjustedDepTime % 100;
                    
                    int connectionTimeMinutes;
                    if (isNextDay) {
                        // Add 24 hours (1440 minutes) for next day
                        connectionTimeMinutes = (departureHour * 60 + departureMin) + (24 * 60) - (currentHour * 60 + currentMin);
                    } else {
                        connectionTimeMinutes = (departureHour * 60 + departureMin) - (currentHour * 60 + currentMin);
                    }
                    
                    // Skip if connection time is too short
                    if (connectionTimeMinutes < minConnectionTime) {
                        continue;
                    }
                    
                    // Create new path node
                    TimePathNode next = current;
                    next.airport = flight.destination;
                    next.arrivalTime = flight.arrTime;
                    if (isNextDay && flight.arrTime < flight.depTime) {
                        // If it's an overnight flight after an overnight connection, add another day
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    } else if (isNextDay) {
                        // If it's just an overnight connection but not an overnight flight
                        next.arrivalTime = (next.arrivalTime + 2400) % 2400;
                    }
                    next.path.push_back(flight.destination);
                    next.depTimes.push_back(flight.depTime);
                    next.arrTimes.push_back(next.arrivalTime); // Use adjusted arrival time
                    next.flightIDs.push_back(flight.flightID);
                    next.totalCost += flight.cost;
                    
                    pq.push(next);
                }
            }
        }
    }
    
    // Display results
    bool anyRouteFound = false;
    
    if (fewestFlightsRoute.totalFlights > 0) {
        anyRouteFound = true;
        cout << endl << "Route with fewest flights:" << endl;
        fewestFlightsRoute.printItinerary();
    }
    
    if (earliestArrivalRoute.totalFlights > 0) {
        anyRouteFound = true;
        cout << endl << "Route with earliest arrival:" << endl;
        earliestArrivalRoute.printItinerary();
    }
    
    if (cheapestRoute.totalFlights > 0) {
        anyRouteFound = true;
        cout << endl << "Cheapest route:" << endl;
        cheapestRoute.printItinerary();
    }
    
    if (!anyRouteFound) {
        cout << "No valid routes found from " << source << " to " << destination 
             << " with layover at " << middle << " starting at " << formatTime(startTime) << endl;
    }
}
