/****************************************************************************************************
* Programming Tools for  Engineers and Scientists - HW5
* by Golnaz Sarram
* February 28th, 2015

* Github Repoitory: https://github.com/Golnaz15/homework5/blob/master/Project2/Project2/golnaz.cpp

* Homework Description:
* This code will check a collection reports of earthquake event from sesmic stations
* using a arrays, enumerators, and structures. It creats an output file and a log file.
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

using namespace std;

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
bool read_input(string &, Satation_records, int, int &, int);

void print_message(ofstream &, string );
void print_position(ofstream &, ostream &, int &);
void open_input(ifstream, ofstream &, ostream &, string);
void open_file(ofstream &, ostream &, string);

string Month_Num_to_namestring(Months);
string magnitude_type_to_string(Mag_type);
string Network_code_to_name_string(Net_code);
string Type_of_band_to_string(Band_Type);
string Type_of_instrument_string(Inst_Type);
string Orientation_to_string(Orientation);
string uppercase(string &);

Months number_to_months(int);
Mag_type string_to_Mag_type(string);
Net_code string_to_Net_code(string);
Band_Type string_to_Band_Type(string);
Inst_Type string_to_Inst_Type(string);
Orientation string_to_Orientation(string);

//================================================================================================/


int main() {

	// Declare variable types:

	string   inputfilename = "golnaz.in", outputfilename = "golnaz.out", logfilename = "golnaz.log";
	ofstream outputfile;
	ofstream logfile;
	ifstream inputfile;

	string   event_ID;
	int day = 1, year = 1850, mm = 1, tzl = 0, i = 0;        // Date and Time variables in format : ("mm/dd/yyyy or mm-dd-yyyy hh:mm:ss.fff time_zone");
	int hr = 0, min = 0, delimi_position = 0;
	string date, time, m, dy, d, y, minsec, Hr, Min, Sec;
	Months month;

	string readline3;                                        // "Earthquake name"
	string time_zone;
	float  sec = 0;
	char   tz;

	double longitude = 0, latitude = 0, depth = 0;
	string magnitude_type;
	float magnitude_size;                                   // greater than 0 ( > 0) 
	string station_code;

	// Open input file

	open_input(inputfile, logfile, cout, inputfilename);

	// Generating the log files

	open_file(logfile, cout, "golnaz.log");

	// Reading the input Header and check for the data correctness
	// First line for event ID

	inputfile >> event_ID;

	// Reading second line for Datetime and time zone information

	inputfile >> date;

	// Case 1:

	delimi_position = date.find_first_of("/");
	m = date.substr(0, delimi_position);
	stringstream(m) >> mm;
	month << mm;

	dy = date.substr(delimi_position + 1);

	delimi_position = dy.find_first_of("/");
	d = dy.substr(0, delimi_position);
	stringstream(d) >> day;

	y = dy.substr(delimi_position + 1);
	stringstream(y) >> year;

	// Check for date validation

	if (mm != year) {
		if (is_valid_date(mm, day, year) == 0) {
			print_message(logfile, cout, "date of earthquake is not valid");
			exit(0);
		}
		else { cout << mm << "/" << day << "/" << year << "\n"; }
		//return 0;	
	}

	// Case 2 (delimiter is deferent):

	if (mm == year) {
		delimi_position = date.find_first_of("-");
		m = date.substr(0, delimi_position);
		stringstream(m) >> mm;
		month << mm;

		dy = date.substr(delimi_position + 1);

		delimi_position = dy.find_first_of("-");
		d = dy.substr(0, delimi_position);
		stringstream(d) >> day;

		y = dy.substr(delimi_position + 1);
		stringstream(y) >> year;

		// Check for date validation

		if (mm != year) {
			cout << mm << "/" << day << "/" << year << "\n";
			if (!is_valid_date(mm, day, year)) {
				print_message(logfile, cout, "Date of the earthquake is not valid");
				// exit(0);
			}
		}
	}

	// Read time of the event

	cout << "line 389" << "\n";

	inputfile >> time;

	delimi_position = time.find_first_of(":");
	Hr = time.substr(0, delimi_position);
	stringstream(Hr) >> hr;

	minsec = time.substr(delimi_position + 1);

	delimi_position = minsec.find_first_of(":");
	Min = minsec.substr(0, delimi_position);
	stringstream(Min) >> min;

	Sec = minsec.substr(delimi_position + 1);
	stringstream(Sec) >> sec;

	// Check for time validation

	cout << hr << ":" << min << ":" << sec << "\n";
	if (is_valid_time(hr, min, sec) == 0) {
		print_message ( logfile, cout, "Time of earthquake is not valid" );
		exit(0);
	}

	// Read time_zone for the event

	inputfile >> time_zone;
	cout << "\n";
	string str = time_zone;
	const char *cstr = str.c_str();
	cout << cstr;
	tzl = strlen(cstr);
	if (tzl != 3) {
		print_message(logfile, cout, "Time_zone is not valid");
		exit(0);
	}

	// Reading the third line for event name

	inputfile.ignore();
	getline(inputfile, readline3);

	// Reading the fourth line for orintation and magnitude data

	// Orintation entries:
	inputfile >> longitude;
	inputfile >> latitude;
	inputfile >> depth;

	cout << longitude << " " << latitude << " " << depth << "\n";

	// Magnitude properties:
	inputfile >> magnitude_type;
	if (!is_valid_magnitude_type(magnitude_type)) {
		print_message(logfile, cout, "The magnitude_type is not valid");
		// exit(0);
	}

	inputfile >> magnitude_size;
	if (magnitude_size <= 0) {
		print_message(logfile, cout, "The magnitude_size is not valid");
		//exit(0);
	}

	print_message(logfile, cout, "Header read correctly!");

	// Generating the log files
	
	open_file(outputfile, cout, "golnaz.out");

	// Print the header in the outputfile:

	outputfile << "# " << Month_Num_to_namestring(number_to_months(mm)) 
		<< " " << day << " " << year << " " << time << " " << time_zone 
		<< " " << magnitude_type << " " << magnitude_size << " " << readline3 
		<< " " << "[" << event_ID << "]" << "(" << longitude << "," << " " 
		<< latitude << "," << " " << depth << ")" << "\n";

	// Calling the read_input function

	Satation_records Records[MAXSIZE];
	int Num_of_read_entries;
	int entry_pos;
	int Valid_entries;     // aaya bayad har motoghayer ra 2bar tarif konim?

		if (!read_input(logfilename, Records, Valid_entries, Num_of_read_entries, entry_pos)) {
			print_message(logfile, cout, "Problems opening database file");
			return 0;
		}

		int AA = (Num_of_read_entries - Valid_entries);

		outputfile << Valid_entries << "\n";
		print_message(logfile, cout, "Total invalid entries ignored:");
		print_position(logfile, cout, AA);
		print_message(logfile, cout, "Total valid entries read:");
		print_position(logfile, cout, Valid_entries);
		print_message(logfile, cout, "Total signal name produced");
		print_position(logfile, cout, MAXSIZE);
		print_message(logfile, cout, "Finished!");

		for (int i = 0; i < MAXSIZE; i++) {
			
			outputfile << Network_code_to_name_string(Records[i].net_code) << "." << Records[i].Station_Name 
				<< "." << Type_of_instrument_string(Records[i].inst_type) << Orientation_to_string(Records[i].orientation) << endl;
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

string Month_Num_to_namestring(Months aa) {
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

Months number_to_months(int a){

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

string magnitude_type_to_string(Mag_type bb) {
	switch (bb) {
	case ML:  return "ML";
	case Mb:  return "Mb";
	case Ms:  return "Ms";
	case Mw:  return "Mw";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Mag_type string_to_Mag_type(string b){

	string ss = uppercase(b);

	if (ss == "ML")  return ML;
	if (ss == "MB")  return Mb;
	if (ss == "MS")  return Ms;
	if (ss == "MW")  return Mw;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Network_code_to_name_string(Net_code cc) {
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

Net_code string_to_Net_code(string c){

	string ss = uppercase(c);

	if (ss == "CE")  return CE;
	if (ss == "CI")  return CI;
	if (ss == "FA")  return FA;
	if (ss == "NP")  return NP;
	if (ss == "WR")  return WR;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Type_of_band_to_string(Band_Type dd) {
	switch (dd) {
	case LongPeriod:   return "L";
	case ShortPeriod:  return "B";
	case Broadband:    return "H";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Band_Type string_to_Band_Type(string d){

	string ss = uppercase(d);

	if (ss == "L")  return LongPeriod;
	if (ss == "B")  return ShortPeriod;
	if (ss == "H")  return Broadband;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Type_of_instrument_string(Inst_Type ee) {
	switch (ee) {
	case HighGain:       return "H";
	case LowGain:        return "L";
	case Accelerometer:  return "N";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Inst_Type string_to_Inst_Type(string e){

	string ss = uppercase(e);

	if (ss == "H")  return HighGain;
	if (ss == "L")  return LowGain;
	if (ss == "N")  return Accelerometer;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

string Orientation_to_string(Orientation ff) {
	switch (ff) {
	case N:   return "N";
	case E:   return  "E";
	case Z:   return  "Z";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Orientation string_to_Orientation(string f) {                // this part needs correction????

	string ss = uppercase(f);

	if (ss == "N")  return N;
	if (ss == "E")  return E;
	if (ss == "Z")  return Z;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

// Functions for input header inorder to check date and time validation 

bool is_valid_date(int & mm, int & dd, int & yyyy) {
	bool date_f = 1;
	if (mm < 0 || mm > 13) { date_f = 0; }
	if (dd < 0 || dd > 32) { date_f = 0; }
	if (yyyy < 1850 || yyyy > 2016) { date_f = 0; }
	return date_f;
}

bool is_valid_time(int hr, int min, int sec) {
	bool time_f = 1;
	if (hr < 0 || hr > 24) { time_f = 0; }
	if (min < 0 || min > 60) { time_f = 0; }
	if (sec < 0.0009 || sec > 59.9999) { time_f = 0; }
	return time_f;
}

// Functions for converting the entries to uppercase and check for validation 

string uppercase(string & s) {
	string result = s;
	for (int i = 0; i < (int)result.size(); i++)
		result[i] = toupper(result[i]);
	return result;
}

bool is_valid_magnitude_type(string magnitude_type) {
	string ss = uppercase(magnitude_type);
	return((ss == "Ml") || (ss == "Ms") || (ss == "Mb") || (ss == "Mw"));
}

bool is_valid_Network_code(string net_code) {
	string ss = net_code;
	return((ss == "CE") || (ss == "CI") || (ss == "FA") || (ss == "NP") || (ss == "WR"));
}

bool is_valid_Station_code(string Stati_code) {                // 3 capital alphabetic character or 5 numeric characters
	string ss = Stati_code;
	return((ss == "CE"));
}

bool is_valid_Type_of_band(string Band_type) {
	string ss = uppercase(Band_type);
	return((ss == "LONG-PERIOD") || (ss == "SHORT_PERIOD") || (ss == "BROAD-BAND"));
}

bool is_valid_Type_of_instrument(string inst_type) {
	string ss = uppercase(inst_type);
	return((ss == "HIGH-GAIN") || (ss == "LOW-GAIN") || (ss == "ACCELEROMETER"));
}

bool is_valid_Orientation1(string orientation) {
	string ss = uppercase(orientation);
	return((ss == "N") || (ss == "E") || (ss == "Z") || (ss == "NE") || (ss == "NZ") ||
		(ss == "EN") || (ss == "EZ") || (ss == "ZN") || (ss == "ZE") || (ss == "EZN") ||
		(ss == "ENZ") || (ss == "ZNE") || (ss == "ZEN") || (ss == "NZE") || (ss == "NEZ"));
}

bool is_valid_Orientation2(string orientation) {                                               // nneds more think and correction????
	string ss = orientation;
	return((ss == "1") || (ss == "2") || (ss == "3") || (ss == "12") || (ss == "13") || (ss == "21")
		|| (ss == "23") || (ss == "31") || (ss == "32") || (ss == "123") || (ss == "132")
		|| (ss == "213") || (ss == "231") || (ss == "312") || (ss == "321"));
}

// Defining the main struct of event report propertires

struct Satation_records {
	Net_code     net_code;
	string       Station_Name;
	Band_Type    band_type;
	Inst_Type    inst_type;
	Orientation  orientation;
};

const int MAXSIZE = 300;

// read the input -- pass back whether error or normal as result
// stations' ID is in db
bool read_input(string & logfilename, Satation_records db[MAXSIZE], int Valid_entries, int & code, int entry_pos) {

	string net_code;
	string band_type;
	string orientation;
	string inst_type;
	string orientation1;
	int    orientation2;
	ofstream outputfile;
	ofstream logfile;
	ifstream inputfile;

	code = 0;
	Valid_entries = 0;
	entry_pos = 0;

	while (Valid_entries < MAXSIZE) {              // or wile inputfile.eof()

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

		if (!is_valid_Orientation1(orientation)) {
			print_message(logfile, "Entry # ");
			print_position(logfile, cout, entry_pos);
			print_message(logfile, "ignored. Invalid Orientation");
			m++;

		} else {
			if (!is_valid_Orientation2(orientation)) {
				print_message(logfile, "Entry # ");
				print_position(logfile, cout, entry_pos);
				print_message(logfile, "ignored. Invalid Orientation");
				m++;

				db[code].net_code = string_to_Net_code(net_code);
				db[code].Stati_code = string_to_Band_Type(Stati_code);         // baresi lazem darad?
				db[code].band_type = string_to_Band_Type(band_type);
				db[code].inst_type = string_to_Inst_Type(inst_type);
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