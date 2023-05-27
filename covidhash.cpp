#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Purpose: Takes a valid decimal number to be used that converts it to an octal. Input has to be a valid decimal only.
int decToOctal(int dec) {
  int rem, i = 1;
  int octal = 0;
  while (dec != 0) {
    rem = dec % 8;
    dec /= 8;
    octal += rem * i;
    i *= 10;
  }
  return octal;
}

class DataEntry {
private:
  string date;
  string country;
  int c_cases;
  int c_deaths;

public:
  DataEntry(string date, string country, int c_cases, int c_deaths) {
    this->date = date;
    this->country = country;
    this->c_cases = c_cases;
    this->c_deaths = c_deaths;
  }
  string getDate() { return date; }
  void setDate(string s) { this->date = s; }
  string getCountry() { return country; }
  void setCountry(string s) { this->country = s; }
  int getCases() { return c_cases; }
  void setCases(int i) { this->c_cases = i; }
  int getDeaths() { return c_deaths; }
  void setDeaths(int i) { this->c_deaths = i; }
};

class covidDB {
private:
  int size;
  vector<vector<DataEntry>> data;

public:
  covidDB() {
    size = 17;
    data = vector<vector<DataEntry>>(size);
  }

// Purpose: Cretes a data entry on the code.
  bool add(DataEntry d) {
    int hashValue = hash(d.getCountry());               // Row index
    int countryIndex = getCountryIndex(d.getCountry()); //Column Index

    if (countryIndex == -1) {
      data[hashValue].push_back(d);
      return true;
    } else {
      stringstream ss;

      vector<string> latestDate =
          split(data[hashValue][countryIndex].getDate(), '/');
      vector<string> entryDate = split(d.getDate(), '/');

      int latestMonth;
      ss << latestDate[0];
      ss >> latestMonth;
      int entryMonth = stoi(entryDate[0]); // enter the month
      int latestDay = stoi(latestDate[1]); // latest day
      int entryDay = stoi(entryDate[1]); // entered day
      int latestYear = stoi(latestDate[2]); // latest year
      int entryYear = stoi(entryDate[2]); // entered year

      if (entryYear > latestYear ||
          (entryYear == latestYear && entryMonth > latestMonth) ||
          (entryYear == latestYear && entryMonth == latestMonth &&
           entryDay > latestDay)) {
        data[hashValue][countryIndex].setDate(d.getDate());
      }

      data[hashValue][countryIndex].setCases(
          d.getCases() + data[hashValue][countryIndex].getCases());
      data[hashValue][countryIndex].setDeaths(
          d.getDeaths() + data[hashValue][countryIndex].getDeaths());

      return false;
    }
}

  // Purpose: acts as a function to be used for hashing.
  int hash(string s) { 
            int sum = 0; 

            for (int i = 1; i <= s.length(); i++) { 
                int ascii = decToOctal(int(s[i - 1]));
                sum += i * ascii;
            }
            return sum % size; 
        } 

   // Purpose: gets the specific data entry for a country. If it does not     work, return null.
  DataEntry* get(string country) {
            int index = hash(country);
            for (int i = 0; i < data[index].size(); i++) {
                if (data[index][i].getCountry() == country) {
                    return new DataEntry(data[index][i]);
                }
            }
            return NULL;
        }

  // Purpose: gets the index of a certain country to be used in other         functions. if it does not work, returns -1.
  int getCountryIndex(string s) {
            int index = hash(s);
            for (int i = 0; i < data[index].size(); i++) {
                if (data[index][i].getCountry() == s) {
                    return i;
                }
            }
            return -1;
        }

  // Purpose: Deletes a specified country according to the user.
  void remove(string s) {
            int index = hash(s);
            for (int i = 0; i < data[index].size(); i++) {
                if (data[index][i].getCountry() == s) {
                    data[index].erase(data[index].begin() + i);
                }
            }
        }

  // Purpose: Initializes the initial hash table to be used.
  void initHashTable() {
            ifstream file("WHO-COVID-Data.csv");
            string line;
            while(getline(file, line)) {
                vector<string> result = split(line, ',');
                DataEntry entry = DataEntry(result[0], result[1], stoi(result[2]), stoi(result[3]));
                add(entry);
            }
        }

  // Purpose: Splits the string on every specific character and returns those characters as vectors of strings.
  vector<string> split(string str, char del) {
            vector<string> result;
            string temp = "";
            for (int i=0; i<(int)str.size(); i++) {
                if (str[i] != del) {
                    temp += str[i];
                } else {
                    result.push_back(temp);
                    temp = "";
                }
            }
            result.push_back(temp);
            return result;
        }

  // Purpose: Prints out the relevant hash table.
  void printHashTable() {
            cout << endl << "CovidDB" << endl;
            for (int i = 0; i < data.size(); i++) {
                for (int j = 0; j < data[i].size(); j++) {
                    cout << data[i][j].getCountry() << ", "
                    << data[i][j].getDate() << ", "
                    << data[i][j].getCases() << ", " 
                    << data[i][j].getDeaths();
                    cout << endl;
                }
            }
        }
};

// Purpose: Driver Code
int main() {
  int input; // Input value to be used for UI
  covidDB database = covidDB(); // Covid database
  do {
        cout << "Covid Tracker" << endl;
        cout << "Please choose the operation you want: " << endl
        << "1. Create the initial hash table" << endl
        << "2. Add a new data entry" << endl
        << "3. Get a data entry" << endl
        << "4. Remove a data entry" << endl
        << "5. Display hash table" << endl
        << "0. Quit the system" << endl;
        cin >> input;
        switch (input) {
            case 1: {
                database.initHashTable();
                cout << endl << "Initial hash table created." << endl << endl;
                break;
            }
            case 2: {
                string date;
                cout << endl << "Enter date in (MM/DD/YYYY) format: ";
                cin >> date;
                string country;
                cout << "Enter the country: ";
                cin >> country;
                int cases;
                cout << "Enter number of cases: ";
                cin >> cases;
                cout << "Enter number of deaths: ";
                int deaths;
                cin >> deaths;
                DataEntry entry = DataEntry(date, country, cases, deaths);
                database.add(entry);
                cout << endl << "SUCCESS: Data entry added to database." << endl << endl;
                break;
            }
            case 3: {
                string countryToRetrieve;
                cout << endl << "Enter the Country to be retrieved: ";
                cin >> countryToRetrieve;
                DataEntry* retrieved = database.get(countryToRetrieve);
                if (retrieved != NULL) {
                    cout << retrieved->getCountry() << ", "
                    << retrieved->getDate() << ", "
                    << retrieved->getCases() << ", " 
                    << retrieved->getDeaths();  
                } else {
                    cout << "ERR: Country invalid.";
                }
                cout << endl << endl;
                break;
            }
            case 4: {
                string countryToRemove;
                cout << endl << "Enter Country to be removed: ";
                cin >> countryToRemove;
                database.remove(countryToRemove);
                cout << endl << countryToRemove << " removed from database." << endl << endl;
                break;
            }
            case 5: {
                database.printHashTable();
                cout << endl;
                break;
            }
            case 0:
                break;
            default: {
                cout << endl << "ERR: Invalid input, please try again." << endl << endl;
                break;
            }
        }
    } while (input != 0);
  return 0;
}
