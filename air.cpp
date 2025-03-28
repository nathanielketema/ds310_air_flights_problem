#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct AirportDetails {
    string name;
    string city;
    string state;
};

struct FlightDetails {
    string depTime;
    string arrTime;
    int cost;
    int miles;
    string airline;
    string flightID;
    string source;
    string destination;
};

typedef unordered_map<string, vector<string>> mapState;
typedef unordered_map<string, vector<AirportDetails>> mapAirport;
typedef unordered_map<string, vector<FlightDetails>> mapFlights;

mapState stateTable;
mapAirport airportTable;
mapFlights flightsTable;

void displayMenu();
void storeListOfAiports(ifstream &file1);
void storeSampleFlights(ifstream &file2);
void display_using_airport_code(string code);
void airports_in_state(string state);
void flights_leaving(string source);
void flights_arriving(string destination);
void flights_source_to_destination(string source, string destination);

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

    if (!listOfAirports || !sampleFlights) {
        cout << "Error: Could not open one or more files!" << endl;
        return 1;
    }

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
    cout << "1 -> Given airport code, display details" << endl;
    cout << "2 -> Airports in a state  (list & count)" << endl;
    cout << "3 -> Flights leaving from source (list & count)" << endl;
    cout << "4 -> Flights arriving at destination (list & count)" << endl;
    cout << "5 -> Flights from source to destination (list & count)" << endl;
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
            case 0:
                cout << "Program exiting..." << endl;
                exit(0);
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

    // Hash Table using airports as keys (map airport code to AirportDetails)
    while (getline(file, word, ',')) {
        key = word; // Airport Code
        getline(file, word, ',');
        data.name = word; // Airport Name
        getline(file, word, ',');
        data.state = word; // State
        getline(file, word);  // City
        data.city = word;

        airportTable[key].push_back(data); // Insert airport data into airportTable
    }

    // Hash Table using state as keys (map state to list of airport codes)
    for (const auto& pair : airportTable) {
        for (const auto& airport : pair.second) {
            stateTable[airport.state].push_back(pair.first);
        }
    }
}

void storeSampleFlights(ifstream &file)
{
    string source, destination, depTime, arrTime, cost, miles, airline, flightID;
    while (file >> source >> destination >> depTime >> arrTime >> cost >> miles >> airline >> flightID) {
        FlightDetails flight = {
            depTime, arrTime, stoi(cost), stoi(miles), airline, flightID, source, destination
        };

        // Store flight in a map keyed by source airport code
        flightsTable[source].push_back(flight);
    }
}

void display_using_airport_code(string code)
{
    int count = 0;
    if (airportTable.find(code) != airportTable.end()) {
        for (const auto& airport : airportTable[code]) {
            cout << "Name: " << airport.name << endl;
            cout << "City: " << airport.city << endl;
            cout << "State: " << airport.state << endl;
            count++;
        }
        if (count == 0) {
            cout << "No details found for airport code: " << code << endl;
        }
    } else {
        cout << "No details found for airport code: " << code << endl;
    }
}

void airports_in_state(string state)
{
    int count = 0;
    if (stateTable.find(state) != stateTable.end()) {  // Check if state exists
        for (const auto& airportCode : stateTable[state]) {
            for (const auto& airport : airportTable[airportCode]) {
                cout << airport.name << " (" << airport.city << ", " << airport.state << ")" << endl;
            }
            count++;
        }
        cout << "There are " << count << " airports in " << state << endl;
    } else {
        cout << "No airports found in " << state << endl;
    }
}

void flights_leaving(string source)
{
    int count = 0;
    if (flightsTable.find(source) != flightsTable.end()) {
        for (const auto& flight : flightsTable[source]) {
            cout << flight.flightID << " -> " << flight.destination << " | " 
                 << "Departs at: " << flight.depTime << " | "
                 << "Arrival at: " << flight.arrTime << " | "
                 << "Cost: $" << flight.cost << " | "
                 << "Miles: " << flight.miles << endl;
            count++;
        }
        cout << "There are " << count << " flights leaving from " << source << endl;
    } else {
        cout << "No flights found from " << source << endl;
    }
}

void flights_arriving(string destination)
{
    int count = 0;
    for (const auto& pair : flightsTable) {
        for (const auto& flight : pair.second) {
            if (flight.destination == destination) {
                cout << flight.flightID << " -> " << flight.source << " | " 
                     << "Departs at: " << flight.depTime << " | "
                     << "Arrival at: " << flight.arrTime << " | "
                     << "Cost: $" << flight.cost << " | "
                     << "Miles: " << flight.miles << endl;
                count++;
            }
        }
    }
    cout << "There are " << count << " flights arriving at " << destination << endl;
}

void flights_source_to_destination(string source, string destination)
{
    int count = 0;
    if (flightsTable.find(source) != flightsTable.end()) {
        for (const auto& flight : flightsTable[source]) {
            if (flight.destination == destination) {
                cout << flight.flightID << " -> " << flight.source << " -> " << flight.destination << " | "
                     << "Departs at: " << flight.depTime << " | "
                     << "Arrival at: " << flight.arrTime << " | "
                     << "Cost: $" << flight.cost << " | "
                     << "Miles: " << flight.miles << endl;
                count++;
            }
        }
    }
    cout << "There are " << count << " flights from " << source << " to " << destination << endl;
}
