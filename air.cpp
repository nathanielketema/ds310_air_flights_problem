#include <iostream>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <vector>
using namespace std;

struct AirportDetails {
    string name;
    string city;
    string state;
};

struct FlightDetais {
    string depTime;
    string arrTime;
    int cost;
    int miles;
    string airline;
    string flightID;
};

typedef unordered_map< string, vector<string> > mapState;
typedef unordered_map< string, vector<AirportDetails> > mapAirport;

void displayMenu();
void storeListOfAiports(ifstream &file);
void storeSampleFlights(ifstream &file);
void display_using_airport_code(string code);
void airports_in_state(string state);
void flights_leaving(string source);
void flights_arriving(string destination);
void flights_source_to_destination(string source, string destination);

mapState stateTable;
mapAirport airportTable;

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
        key = word; // Airport Code
        getline(file, word, ',');
        data.name = word; // Airport Name
        getline(file, word, ',');
        data.state = word; // State
        getline(file, word);  // City
        data.city = word;

        airportTable[key].push_back(data);
    }

    // State to list of airport codes
    for (const auto& pair : airportTable) {
        for (const auto& airport : pair.second) {
            stateTable[airport.state].push_back(pair.first);
        }
    }
}

void storeSampleFlights(ifstream &file)
{
    // store as an adjacency matrix
}

void display_using_airport_code(string code)
{
    int count = 0;
    for (const auto& state : airportTable[code]) {
        cout << "Name: " << state.name << endl;
        cout << "City: " << state.city << endl;
        cout << "State: " << state.state << endl;
    }
}

void airports_in_state(string state)
{
    int count = 0;
    for (const auto& flight : stateTable[state])
    {
        cout << flight << endl;
        count++;
    }
    cout << "There are " << count << " airports in " << state << endl;
}

void flights_leaving(string source)
{
}

void flights_arriving(string destination)
{
}

void flights_source_to_destination(string source, string destination)
{
}
