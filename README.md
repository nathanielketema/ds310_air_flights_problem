# DS310-Air 

Program will take 2 data files, command line
a) list of airports
b) sample flights

File formats:  

US Airports - comma separated
- FAA     (3 letter code) 
- Name 
- City
- State  (2 letter postal)

Flights - space separated
- source            (airport codes )
- destination      
- departure time    (hhmm)
- arrival time 
- cost              (int)
- miles             (int)
- airline           (no spaces in name)
- flight ID         (no space)

no flights to same source/destination
multiple flights possible between same s/d
all flights under 24 hours


Interactive menu?
---

Look up:

a) Given airport code, dislay details

b) Airports in a state  (list & count)

c) Flights leaving from source (list & count)

d) Flights arriving at destination (list & count)

e) Flights from source to destination (list & count)
