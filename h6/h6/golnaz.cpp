/****************************************************************************************************
* Programming Tools for  Engineers and Scientists - HW5
* by Golnaz Sarram
* March 15th, 2015

* Github Repoitory: https://github.com/Golnaz15/homework5/blob/master/Project2/Project2/golnaz.cpp

* Homework Description:
* This code will check a collection reports of earthquake event from sesmic stations
* using arrays, enumerators, and structures. It creats an output file and a log file.
****************************************************************************************************/

// Calling external libraries

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ostream>
#include <cstdlib>
#include <sstream>
#include <istream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <numeric>
#include <cstring>
#include <cctype>

using namespace std;

// Global Variables are

ofstream outputfile;
ofstream logfile;
ifstream inputfile;
const int MAXSIZE = 300;
string   inputfilename = "golnaz.in", outputfilename = "golnaz.out", logfilename = "golnaz.log";

//======================================= Function Prototypes =====================================/

bool is_valid_date(int &, int &, int &);
bool is_valid_time(int, int, int);
bool is_valid_magnitude_type(string);
bool is_valid_Network_code(string);
bool is_valid_Station_code(string);
bool is_valid_Type_of_band(string);
bool is_valid_Type_of_instrument(string);
bool is_valid_Orientation1(string);
bool is_valid_Orientation2(string);
bool read_input(string &, station, int, int &, int);

void print_message(ofstream &, string );
void print_position(ofstream &, ostream &, int &);
void open_input(ifstream, ofstream &, ostream &, string);
void open_file(ofstream &, ostream &, string);

string Month_Num2namestr(Months);
string mag_type2str(Mag_type);
string Netcode2namestr(Net_code);
string Type_of_band_to_string(Band_Type);
string Inst_Type2str(Inst_Type);
string Orientation_to_string(Orientation);
string uppercase(string &);

Months month_num2enum(int);
Mag_type str2Mag_type(string);
Net_code str2Net_code(string);
Band_Type string_to_Band_Type(string, Band_Type);
Inst_Type Inst_Type_str2enum (string, Inst_Type);
Orientation string_to_Orientation(string);

//================================================================================================/


int main() {

	string station_code;

	// Generating the inputfile, outputfile and logfile

	open_input(inputfile, logfile, cout, inputfilename);
	open_file(logfile, cout, "golnaz.log");
	open_file(outputfile, cout, "golnaz.out");

	inputFile.close();
    outputFile.close();
	
	// Reading the input Header and check for the data correctness


	// Calling the read_input function

	station Records[MAXSIZE];
	int Num_of_read_entries;
	int entry_pos;
	int Valid_entries;     // aaya bayad har motoghayer ra 2bar tarif konim?

	if (!read_input(logfilename, Records, Valid_entries, Num_of_read_entries, entry_pos)) {
		print_message(logfile, "Problems opening database file");
		return 0;
	}

	int AA = (Num_of_read_entries - Valid_entries);

	outputfile << Valid_entries << "\n";
	print_message(logfile, "Total invalid entries ignored:");
	print_position(logfile, cout, AA);
	print_message(logfile, "Total valid entries read:");
	print_position(logfile, cout, Valid_entries);
	print_message(logfile, "Total signal name produced");
	print_position(logfile, cout, MAXSIZE);
	print_message(logfile, "Finished!");

	for (int i = 0; i < MAXSIZE; i++) {

		outputfile << Netcode2namestr(Records[i].net_code) << "." << Records[i].Station_Name 
			<< "." << Inst_Type2str(Records[i].inst_type) << Orientation_to_string(Records[i].orientation) << endl;
	}

	outputfile.close();

	return 0;
}

//======================================== Declare functions ==========================================/

// Function for printing the errors into the logfile and terminal

void open_input(ifstream inputfile, ofstream & logfile, ostream & stream, string inputfilename) {

	// Prompt the user for an input file
	cout << "Enter input file name: ";
	cin >> inputfilename;

	inputfile.open(inputfilename.c_str());

	if (inputfile.fail()) {
		print_message(logfile, "Error: reading the file. ");
		exit(EXIT_FAILURE);
	}
}

void open_file(ofstream & file, ostream & stream, string filename) {

	file.open(filename.c_str());

	if (file.fail()) {
		print_message(file, "Error: cannot open the file. ");
		exit(EXIT_FAILURE);
	}
}

void print_message(ofstream & logfile, string statement) {
	logfile << statement << "\n";
	cout << statement << "\n";
	return;
}

// Function for printing the results into the outputfile and terminal

void print_position(ofstream & logfile, ostream & stream, int & position) {
	logfile << (position) << "\n";
	cout << (position) << "\n";
	return;
}

// Define enumerator types

enum Months {
	January = 1, February, March, April, May, June, July, 
	August, September, October, November, December 
};

enum Mag_type {                                  // case insensitive
	ML = 1,
	Ms,
	Mb,
	Mw
};

enum Net_code {                                  // case sensitive
	CE,
	CI,
	FA,
	NP,
	WR
};

enum Band_Type {
	LongPeriod,
	ShortPeriod,
	Broadband
};

enum Inst_Type {
	HighGain,
	LowGain,
	Accelerometer
};

enum Orientation {
	N,
	E,
	Z,
};

// Functions for input header inorder to check date and time validation 

void check_date(ofstream & logfile, string date, string & month, string & day, string & year) {

	int mm, dd, yyyy;

	// Check for the date format (it must be mm/dd/year or mm-dd-year and 10 digits)

	if (date.length() == 10) {

		month = date.substr(0, 2);
		stringstream(mm) >> month;
		day = date.substr(3, 2);
		stringstream(dd) >> day;
		year = date.substr(6, 4);
		stringstream(yyyy) >> year;

		// meanwhile month, day and year should be valid numbers

		if (!isdigit (mm || dd || yyyy)) {
			print_message(logfile, "Error: Date of earthquake is not valid. ");
			exit (EXIT_FAILURE);
		} else {
			if (mm < 0 || mm > 13 || dd < 0 || dd > 32 || yyyy < 1850 || yyyy > 2016) {
				print_message(logfile, "Error: Date digits are not valid. ");
				exit (EXIT_FAILURE);
			}
		}

		if ((date[2] != '/' || date[5] != '/') && (date[2] != '-' || date[5] != '-')) {
			print_message(logfile, "Error: Date format is not valid. ");
			exit (EXIT_FAILURE);
		}
	} else {
		print_message(logfile, "Error: Date of earthquake is not valid. ");
		exit (EXIT_FAILURE);
	}

}

void check_time(ofstream & logfile, string time, string & hour, string & minute, string & second) {

	int hr, min, sec;

	// Check for the time format (must be hh:mm:ss.fff and 12 digits)

	if (time.length() == 12) {

		hour = time.substr(0, 2);
		stringstream(hr) >> hour;
		minute = time.substr(3, 2);
		stringstream(min) >> minute;
		second = time.substr(6, 2);
		stringstream(sec) >> second;

		//  Meanwhile the hour, minute, second should be valid numbers

		if (!isdigit(hr || min || sec)) {
			print_message(logfile, "Error: Time of earthquake is not valid.");
		} else {
			if (hr < 0 || hr > 24 || min < 0 || min > 60 || sec < 0.0009 || sec > 59.9999 ) {
				print_message(logfile, "Error: Time digits are not valid. ");
				exit (EXIT_FAILURE);
			}
		}
		if (time[2] != ':' || time[5] != ':' || time[8] != '.') {
			print_message(logfile, "Error: Time format is not valid.");
			exit (EXIT_FAILURE);
		} else {
			print_message(logfile, "Error: Time of earthquake is not valid.");
			exit (EXIT_FAILURE);
		}
	}
}

void check_time_zone(ofstream & logfile, string time_zone) {
	
	int tzl = 0;
	string str = time_zone;
	const char *cstr = str.c_str();
	tzl = strlen(cstr);
	if ((tzl != 3) || (!isalpha (time_zone[0])) || (!isalpha (time_zone[1])) || (!isalpha (time_zone[2]))) {
		print_message(logfile, "Error: Time_zone is not valid");
		exit (EXIT_FAILURE);
	}
}

// Functions for converting the entries to uppercase and check for validation 

string uppercase(string & s) {
	string result = s;
	for (int i = 0; i < (int)result.size(); i++)
		result[i] = toupper(result[i]);
	return result;
}

void check_magnitude(ofstream & logfile, string magnitude_type, float magnitude_size) {

	if ( magnitude_size < 0 ) {
		print_message(logfile, "Error: The magnitude_size is not valid");
		exit (EXIT_FAILURE);
	}
	string mt = uppercase(magnitude_type);
	if ((mt == "ML") || (mt == "MS") || (mt == "MB") || (mt == "MW"));
	print_message(logfile, "Error: The magnitude_type is not valid");
	exit (EXIT_FAILURE);
}

bool is_valid_Network_code(string net_code) {

	if (net_code.compare("CE") == 0 ) return true;
	if (net_code.compare("CI") == 0 ) return true;
	if (net_code.compare("FA") == 0 ) return true;
	if (net_code.compare("NP") == 0 ) return true;
	if (net_code.compare("WR") == 0 ) return true;
	else return false;
}

bool is_valid_Station_code(string Stati_code) {                // 3 capital alphabetic character or 5 numeric characters

	if (Stati_code.length() == 5 && isdigit(Stati_code[0,5])) {
		return true;
	}

	if (Stati_code.length() == 3 && isalpha(Stati_code[0,3])) {
		if (isupper(Stati_code[0,3])) {
			return true;
		}
	}
	return false;
}

bool is_valid_Type_of_band(string Band_type, Band_Type band_type) {

	string ss = uppercase(Band_type);
	if (ss == "LONG-PERIOD") {
		band_type = LongPeriod;
		return true;
	}
	if (ss == "SHORT_PERIOD") {
		band_type = ShortPeriod;
		return true;
	}
	if (ss == "BROADBAND") {
		band_type = Broadband;
		return true;
	}
	return false;
}

bool is_valid_Type_of_instrument(string instrumenType, Inst_Type inst_type ) {

	string ss = uppercase(instrumenType);
	if (ss == "HIGH-GAIN") {
		inst_type = HighGain;
		return true;
	}
	if (ss == "LOW-GAIN") {
		inst_type = LowGain;
		return true;
	}
	if (ss == "ACCELEROMETER") {
		inst_type = Accelerometer;
		return true;
	}
	return false;
}

bool is_valid_Orientation (string orientation) {

	// it is case insensitive so convert it to the uppercase and compare it
	string ss = uppercase(orientation);

	if (ss.length() < 4) {
		if (isdigit(ss[0])) {
            for (int i = 0; i < ss.length(); i++) {
                if (!isdigit(ss[i])) {
                    return false;
                }
            }

            return true;
        } else if (isalpha(ss[0])) {
            for (int i = 0; i < ss.length(); i++) {
                if (ss[i] != 'N' && ss[i] != 'E' && ss[i] != 'Z') {
                    return false;
                }
            }

            return true;
        }
    }

    return false;

} 

string Month_Num2namestr (Months aa) {
	switch (aa) {
	case January:   return "January";
	case February:  return "February";
	case March:     return "March";
	case April:     return "April";
	case May:       return "May";
	case June:      return "June";
	case July:      return "July";
	case August:    return "August";
	case September: return "September";
	case October:   return "October";
	case November:  return "November";
	case December:  return "December";
	}
	exit(EXIT_FAILURE);
}

Months month_num2enum (int a){

	if (a == 1)  return January;
	if (a == 2)  return February;
	if (a == 3)  return March;
	if (a == 4)  return April;
	if (a == 5)  return May;
	if (a == 6)  return June;
	if (a == 7)  return July;
	if (a == 8)  return August;
	if (a == 9)  return September;
	if (a == 10) return October;
	if (a == 11) return November;
	if (a == 12) return December;
	// It should never get here!!
	exit(EXIT_FAILURE);
}

string mag_type2str(Mag_type bb) {
	switch (bb) {
	case ML:  return "Ml";
	case Mb:  return "Mb";
	case Ms:  return "Ms";
	case Mw:  return "Mw";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Mag_type str2Mag_type(string b){

	string ss = uppercase(b);

	if (ss == "ML")  return ML;
	if (ss == "MB")  return Mb;
	if (ss == "MS")  return Ms;
	if (ss == "MW")  return Mw;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Net_code2namestr(Net_code cc) {
	switch (cc) {
	case CE:   return "CE";
	case CI:   return "CI";
	case FA:   return "FA";
	case NP:   return "NP";
	case WR:   return "WR";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Net_code str2Net_code(string c){

	string ss = uppercase(c);

	if (ss == "CE")  return CE;
	if (ss == "CI")  return CI;
	if (ss == "FA")  return FA;
	if (ss == "NP")  return NP;
	if (ss == "WR")  return WR;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Type_of_band2str (Band_Type dd) {
	switch (dd) {
	case LongPeriod:   return "L";
	case ShortPeriod:  return "B";
	case Broadband:    return "H";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Band_Type str2Band_Type(string d){

	string ss = uppercase(d);

	if (ss == "L")  return LongPeriod;
	if (ss == "B")  return ShortPeriod;
	if (ss == "H")  return Broadband;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Inst_Type2str(Inst_Type ee) {
	switch (ee) {
	case HighGain:       return "H";
	case LowGain:        return "L";
	case Accelerometer:  return "N";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Inst_Type Inst_Type_str2enum (string e){

	string ss = uppercase(e);

	if (ss == "H")  return HighGain;
	if (ss == "L")  return LowGain;
	if (ss == "N")  return Accelerometer;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Orientation2str(Orientation ff) {
	switch (ff) {
	case N:   return "N";
	case E:   return  "E";
	case Z:   return  "Z";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Orientation str2Orientation(string f) {                // this part needs correction????

	string ss = uppercase(f);

	if (ss == "N")  return N;
	if (ss == "E")  return E;
	if (ss == "Z")  return Z;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

// Defining the earthquake struct of event report propertires

struct earthquake {
	string id;
	string date;
	string time;
	string timeZone;
	string earthquake_name;
	string latitude;
	string longtidue;
	string magnitude_type;
	string magnitude_size;
	string Enumber;
};

struct Date {
	string day;
	Months month;
	string year;
};

// Defining the main struct of event report propertires

struct station {
	Net_code     net_code;
	string       Station_Name;
	Band_Type    band_type;
	Inst_Type    inst_type;
	Orientation  orientation;
};

// Check the header of the input file
bool check_input_header(ifstream &inputFile, ofstream &outputFile) {
	// (string & logfilename, station db[MAXSIZE], int Valid_entries, int & code, int entry_pos) {

	// Declare variable types:

	int dd = 1, yyyy = 1850, mm = 1, i = 0;        // Date and Time variables in format : ("mm/dd/yyyy or mm-dd-yyyy hh:mm:ss.fff time_zone");
	int hr = 0, min = 0;
	float  sec = 0, magnitude_size;
	double longitude = 0, latitude = 0, depth = 0;

	string date, day, month, year, time, hour, minute, second;
	string line, time_zone, magnitude_type;
	
	// First line for event ID

	earthquake eq_info;
	Date date_elements;

	getline(inputFile, line);
    stringstream eventID (line);
    eventID >> eq_info.id;
    
    getline(inputFile, line);
    stringstream datetime (line);
    datetime >> eq_info.date;
    datetime >> eq_info.time;
    datetime >> eq_info.timeZone;

    getline(inputFile, line);
    eq_info.earthquake_name = line;

    getline(inputFile, line);
    stringstream epicenter (line);
    epicenter >> eq_info.latitude;
    epicenter >> eq_info.longtidue;
    epicenter >> eq_info.Enumber;
    epicenter >> eq_info.magnitude_type;
    epicenter >> eq_info.magnitude_size;

	// check the stored data validation

	string Month_Num2namestr, day, year;

	check_date(logfile, eq_info.date, Month_Num2namestr, date_elements.day, date_elements.year);
	check_time(logfile, time, hour, minute, second);
	check_time_zone(logfile, time_zone);
	check_magnitude(logfile, magnitude_type, magnitude_size);

	print_message(logfile, "Header read correctly!");

	// Print the header in the outputfile:

	outputfile << "# " << " " << Month_Num2namestr << " " << day << " " << year << " " 
		<< time << " " << time_zone << " " << magnitude_type << " " << magnitude_size 
		<< " " << eq_info.earthquake_name << " " << "[" << eq_info.id << "]" << "(" 
		<< longitude << "," << " " << latitude << "," << " " << depth << ")" << "\n";
}

// Check the table of reports from input file and return the signals output
bool check_input_signals(ifstream & inputFile, ofstream & outputFile) {
	
	station Records[MAXSIZE];
	int Num_of_read_entries = 0;
	int entry_pos = 0;
	int Valid_entries;     
	int code = 0;
	int Valid_entries = 0;
	int entry_pos = 0;

	string net_code;
	string band_type;
	string inst_type;
	string orientation;

	int AA = (Num_of_read_entries - Valid_entries);

	outputfile << Valid_entries << "\n";
	print_message(logfile, "Total invalid entries ignored:");
	print_position(logfile, cout, AA);
	print_message(logfile, "Total valid entries read:");
	print_position(logfile, cout, Valid_entries);
	print_message(logfile, "Total signal name produced");
	print_position(logfile, cout, MAXSIZE);
	print_message(logfile, "Finished!");

	for (int i = 0; i < MAXSIZE; i++) {

		outputfile << Netcode2namestr(Records[i].net_code) << "." << Records[i].Station_Name 
			<< "." << Inst_Type2str(Records[i].inst_type) << Orientation_to_string(Records[i].orientation) << endl;
	}

	while (inputFile >> net_code && Valid_entries < MAXSIZE) {              // or wile inputfile.eof()

		int m = -1;

		if (!(inputfile >> net_code >> db[code].Station_Name >> band_type
			>> inst_type >> orientation))
			break;

		entry_pos++;
		if (!is_valid_Network_code(net_code)) {
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid_Network_code");
			m++;
		}

		entry_pos++;
		if (!is_valid_Station_code(net_code)) {                        // baresi lazem darad: 3 capital alphabetic character or 5 numeric characters
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid_Stati_code");
			m++;
		}
		else {
			stringstream str1;
			int orl;
			str1 << orientation.substr(0);
			str1 >> orl;
			//int orl = orientation.substr(0);
			if (orl < 10000 || orl > 99999) {
				print_message(logfile, "Entry # ");
				print_position(logfile, cout, entry_pos);
				print_message(logfile, "ignored. Invalid_Stati_code");
				m++;
			}
		}

		entry_pos++;
		if (!is_valid_Type_of_band(band_type)) {
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_band");
			m++;
		}

		entry_pos++;
		if (!is_valid_Type_of_instrument(inst_type)) {
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_instrument");
			m++;
		}

		if (!is_valid_Orientation (orientation)) {
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid Orientation");
			m++;

		}
				db[code].net_code = str2Net_code(net_code);
				db[code].Stati_code = str2Band_Type(Stati_code);         // baresi lazem darad?
				db[code].band_type = str2Band_Type(band_type);
				db[code].inst_type = Inst_Type_str2enum (inst_type);
				db[code].orientation = string_to_Orientation(orientation);
			}
		}
		if (m = -1) {
			Valid_entries++;
		}
		code++;
	}
	return true;
}