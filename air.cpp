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
