#include <iostream>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <vector>
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

mapState stateTable;
mapAirport airports;
flightGraph flights;

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

    int count = 0;
    for (const auto& flight : stateTable[state])
    {
        cout << flight << endl;
        count++;
    }
    cout << "There are " << count << " states in " << state << endl;
}

void flights_leaving(string source)
{
    if (flights.find(source) == flights.end()) {
        cout << source << " not found!!" << endl;
        return;
    }

    int count = 0;
    for (const auto& dest : flights[source]) {
        cout << dest.flightID << endl;
        count++;
    }

        cout << "Total: " << count << endl;
}

void flights_arriving(string destination)
{
    int count = 0;
    bool found = false;
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
        cout << "No flights found arriving at airport: " << destination << endl;
    }

    cout << "Total: " << count << endl;
}

void flights_source_to_destination(string source, string destination)
{
        int count = 0;
        for(const auto& flight : flights[source][destination])
        {
                cout << flight.flightID << endl;

                count++;
        }
        cout << "Total: " << count << endl;
}
