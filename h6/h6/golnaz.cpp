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

static const int MAXvalidentry = 300;

enum Band_Type {
	LongPeriod,
	ShortPeriod,
	Broadband
};

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

enum Inst_Type {
	HighGain,
	LowGain,
	Accelerometer
};

/*

enum Orientation {
N,
E,
Z,
};

*/

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
	string depth;
};

// Defining the main struct of event report propertires

struct station {
	Net_code     net_code;
	string       Station_Name;
	Band_Type    band_type;
	Inst_Type    inst_type;
	string  orientation;
};

//struct Date {
//    string day;
//    Months month;
//    string year;
//};

// Global Variables are

//ofstream outputifle;
//ofstream logfile;
//ifstream inputfile;

string   inputfilename = "golnaz.in", outputiflename = "golnaz.out", logfilename = "golnaz.log";

//================================= Function Prototypes ==================================/

bool is_valid_date(int &, int &, int &);
bool is_valid_time(int, int, int);
bool is_valid_magnitude_type(string);
bool is_valid_Network_code(string);
bool is_valid_Station_code(string);
bool is_valid_Type_of_band(string);
bool is_valid_Type_of_instrument(string, Inst_Type inst_type);
bool is_valid_Orientation(string);
bool read_input(string, string, int, int &, int);
bool check_input_header(ifstream &, ofstream &);
bool check_input_signals(ifstream &, station entry[MAXvalidentry], int, int, int);

void print_message(ofstream &, const string &);
void print_position(ofstream &, ostream &, int &);
void open_input(ifstream &, ofstream &, ostream &, string);
void open_file(ofstream &, ostream &, string);
void print_output(ofstream &, ofstream &, station, int, int, int);

string get_Month_Num2namestr(Months);
string get_mag_type2str(Mag_type);
string get_Net_code2namestr(Net_code);
string get_Type_of_band2str(Band_Type);
string get_Inst_Type2str(Inst_Type);
//string get_Orientation2str(Orientation);
string uppercase(string &);

Months month_num2enum(int);
Mag_type str2Mag_type(string);
Net_code str2Net_code(string);
Band_Type str2Band_Type(string);
Inst_Type Inst_Type_str2enum (string);
//Orientation str2Orientation(string);

//=========================================================================================/

int main() {

	string station_code;
	ifstream inputfile;
	ofstream outputfile,logfile;
	station entry[MAXvalidentry];
	string net_code;

	int valid_entries = 0, invalidEntries = 0, produced_signalnum = 0;

	// Generating the inputfile, outputifle and logfile

	open_input(inputfile, logfile, cout, inputfilename);

	open_file(logfile, cout, "golnaz.log");
	open_file(outputfile, cout, "golnaz.out");

	// Calling the read_input function

	check_input_header(inputfile, outputfile);
	print_message ( logfile, "header is read successfully!" );

	check_input_signals(inputfile, entry, valid_entries, invalidEntries, produced_signalnum);
	print_output(outputfile, logfile, entry[MAXvalidentry], valid_entries, invalidEntries, produced_signalnum);
	print_message ( logfile, "signals are read correctly!" );

	inputfile.close();
	outputfile.close();

	getchar();
	return 0;
}

//================================ Declare functions =====================================/

// Function for openning inputfile and logfile

void open_file(ofstream & file, ostream & stream, string filename) {

	file.open(filename.c_str());

	if (file.fail()) {
		print_message(file, "Error: cannot open the file. ");
		exit(EXIT_FAILURE);
	}
}

void open_input(ifstream &inputfile, ofstream & logfile, ostream & stream, string inputfilename) {

	// Prompt the user for an input file
	cout << "Enter input file name: ";
	cin >> inputfilename;

	inputfile.open(inputfilename.c_str());

	if (inputfile.fail()) {
		print_message(logfile, "Error: reading the file. ");
		exit(EXIT_FAILURE);
	}
}

// Function for printing the errors and results into the outputifle and terminal

void print_message(ofstream & logfile, const string & statement) {
	logfile << statement << "\n";
	cout << statement << "\n";
	return;
}

void print_position(ofstream & logfile, int & position) {
	logfile << (position) << "\n";
	cout << (position) << "\n";
	return;
}

void set_eventid (earthquake & eq_info, ifstream & inputfile, string line) {
	getline(inputfile, line);
	stringstream eventID (line);
	eventID >> eq_info.id;
	eq_info.id = line;
}

string get_eventid (earthquake & eq_info) {
	return eq_info.id;
}

//============================= Date ===================================/

// Functions for input header inorder to check date and time validation 

void set_date(earthquake & eq_info, ofstream & logfile, string & date, string & month, string & day, string & year, int & mm) {

	int dd, yyyy;
	stringstream month1, day1, year1;

	// Check for the date format (it must be mm/dd/year or mm-dd-year and 10 digits)

	if (date.length() == 10) {

		month = date.substr(0, 2);
		month1 << month;
		month1 >> mm;

		string day = date.substr(3, 2);
		day1 << day;
		day1 >> dd;
		string year = date.substr(6, 4);

		year1 << year;
		year1 >> yyyy;

		//cout << "m : " << mm << "d :" << dd << "y : " << yyyy <<"\n";
		// Meanwhile month, day and year should be valid numbers

		if (!isdigit (date[0]) || !isdigit (date[1]) || !isdigit (date[3]) ||!isdigit (date[4])) {
			print_message(logfile, "Error: Date of earthquake is not valid. ");
			exit (EXIT_FAILURE);
		}

		if (!isdigit (date[6]) || !isdigit (date[7]) || !isdigit (date[8]) || !isdigit (date[9])) {
			print_message(logfile, "Error: Date of earthquake is not valid. ");
			//exit (EXIT_FAILURE);
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

		// (tekrari bar axesh ro paieen daram:) eq_info.date = date;

		/*
		Date.year = year;
		Date.month = month;
		Date.day = day;
		*/

	} else {
		print_message(logfile, "Error: Date of earthquake is not valid. ");
		exit (EXIT_FAILURE);
	}
}

string get_date (earthquake & eq_info) {
	return eq_info.date;
}

//============================= time ===================================/

void set_time(ofstream & logfile, string time, string & hour, string & minute, string & second) {

	int hr, min;
	float  sec = 0;
	stringstream hr1,min1,sec1;
	earthquake eq_info;

	// Check for the time format (must be hh:mm:ss.fff and 12 digits)

	if (time.length() == 12) {

		hour = time.substr(0, 2);
		hr1 << hour;
		hr1 >> hr;
		minute = time.substr(3, 2);
		min1 << minute;
		min1 >> min;
		second = time.substr(6, 2);
		sec1 << second;
		sec1 >> sec;

		//cout << "hr : " << hr << "min :" << min << "sec : " << sec <<"\n";
		//  Meanwhile the hour, minute, second should be valid numbers

		if (!isdigit (time[0]) || !isdigit (time[1]) || !isdigit (time[3]) ||!isdigit (time[4])) {
			print_message(logfile, "Error: time of earthquake is not valid. ");
			exit (EXIT_FAILURE);
		}

		if (!isdigit (time[6]) || !isdigit (time[7]) || !isdigit (time[9]) || !isdigit (time[10]) || !isdigit (time[11])) {
			print_message(logfile, "Error: time of earthquake is not valid. ");
			exit (EXIT_FAILURE);
		}

		if (hr < 0 || hr > 24 || min < 0 || min > 60 || sec < 0.0009 || sec > 59.9999 ) {
			print_message(logfile, "Error: time digits are not valid. ");
			//exit (EXIT_FAILURE);
		}
		if (time[2] != ':' || time[5] != ':' || time[8] != '.') {
			print_message(logfile, "Error: time format is not valid.");
			//exit (EXIT_FAILURE);
		}

		// (tekrari bar axesh ro paieen daram:) eq_info.time = time;

		/*
		Time.hr = hr';
		Time.min = min;
		Time.sec = sec;
		*/

	} else {
		print_message(logfile, "Error: time of earthquake is not valid.");
		//exit (EXIT_FAILURE);
	}
}

string get_time (earthquake & eq_info) {
	return eq_info.time;
}

void set_time_zone(ofstream & logfile, string time_zone) {

	int tzl = 0;
	string str = time_zone;
	const char *cstr = str.c_str();
	tzl = strlen(cstr);
	if ((tzl != 3) || (!isalpha (time_zone[0])) || (!isalpha (time_zone[1])) || (!isalpha (time_zone[2]))) {
		print_message(logfile, "Error: Time_zone is not valid");
		//exit (EXIT_FAILURE);
		
		// (tekrari bar axesh ro paieen daram:) eq_info.timeZone = time_zone;
	}
}

string get_time_zone (earthquake & eq_info) {
	return eq_info.timeZone;
}

//============================= Mag_type ===================================/

// Functions for converting the entries to uppercase and check for validation 

string uppercase(string & s) {
	string result = s;
	for (int i = 0; i < (int)result.size(); i++)
		result[i] = toupper(result[i]);
	return result;
}

void set_magnitude(earthquake & eq_info, ofstream & logfile, string magnitude_type, string magnitude_size) {

	int mag_size;
	stringstream mg;
	mg << magnitude_size;
	mg >> mag_size;

	if ( mag_size < 0 ) {
		print_message(logfile, "Error: The magnitude_size is not valid");
		//exit (EXIT_FAILURE);
	} else { 
		eq_info.magnitude_size = magnitude_size;
	}

	cout << magnitude_type;
	string mt = uppercase(magnitude_type);
	if ((mt != "ML") && (mt != "MS") && (mt != "MB") && (mt != "MW")) {
		print_message(logfile, "Error: The magnitude_type is not valid");
		//exit (EXIT_FAILURE);
	} else {
		eq_info.magnitude_type = magnitude_type;
	}
}

string get_magnitude_size (earthquake & eq_info) {
	return eq_info.timeZone;
}

string get_mag_type2str(Mag_type mg) {
	switch (mg) {
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

string get_magnitude_type (earthquake & eq_info) {
	return eq_info.magnitude_type;
}

//****************** Network_code ***********************/

bool set_valid_Network_code(station & entry, Net_code net_code, string Net_code) {

	if (Net_code.compare("CE") == 0 ) {
		entry.net_code = CE;
		return true;
	}

	if (Net_code.compare("CI") == 0 ) {
		entry.net_code = CI ;
		return true;
	}

	if (Net_code.compare("FA") == 0 ) {
		entry.net_code = FA;
		return true;
	}

	if (Net_code.compare("NP") == 0 ) {
		entry.net_code = NP;
		return true;
	}

	if (Net_code.compare("WR") == 0 ) {
		entry.net_code = WR;
		return true;
	}

	else return false;
}

string get_Net_code2namestr(station & entry, Net_code net_code) {
	switch (entry.net_code) {
	case CE:   return "CE";
	case CI:   return "CI";
	case FA:   return "FA";
	case NP:   return "NP";
	case WR:   return "WR";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Net_code str2Net_code(string nt){

	string ss = uppercase(nt);

	if (ss == "CE")  return CE;
	if (ss == "CI")  return CI;
	if (ss == "FA")  return FA;
	if (ss == "NP")  return NP;
	if (ss == "WR")  return WR;

	// It should never get here!!
	exit(EXIT_FAILURE);
}

//********************** station_code ***********************/

bool set_valid_Station_code(station & entry, string Stati_code) { 

	// 3 capital alphabetic character or 5 numeric characters

	if (Stati_code.length() == 5) {
		for (int i = 0; i < 4; i++) {
			if (isdigit(Stati_code[i])) {
				return true;
			}
		}
	}

	if (Stati_code.length() == 3) {
		for (int i = 0; i < 4; i++) {
			if (isalpha(Stati_code[i])) {
				if (isupper(Stati_code[i])) {
					return true;
				}
			}
		}
	}
	entry.Station_Name = Stati_code;

	return false;
}

string get_Station_code (station & entry) {
	return entry.Station_Name;
}

//********************** Type_of_band ***********************/

bool set_valid_Type_of_band(station & entry, string Band_type) {

	string ss = uppercase(Band_type);
	if (ss == "LONG-PERIOD") {
		entry.band_type = LongPeriod;
		return true;
	}
	if (ss == "SHORT_PERIOD") {
		entry.band_type = ShortPeriod;
		return true;
	}
	if (ss == "BROADBAND") {
		entry.band_type = Broadband;
		return true;
	}
	return false;
}

string get_Type_of_band2str (station & entry) {
	switch (entry.band_type) {
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


string get_Type_of_band (earthquake & eq_info) {
	return entry.band_type;
}

//********************* Type_of_instrument ***********************/

bool set_valid_Type_of_instrument(station & entry, string instrumenType) {

	string ss = uppercase(instrumenType);
	if (ss == "HIGH-GAIN") {
		entry.inst_type = HighGain;
		return true;
	}
	if (ss == "LOW-GAIN") {
		entry.inst_type = LowGain;
		return true;
	}
	if (ss == "ACCELEROMETER") {
		entry.inst_type = Accelerometer;
		return true;
	}
	return false;
}

string get_Inst_Type2str(station & entry) {
	switch (entry.inst_type) {
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

//******************** orientation ***********************/

bool set_valid_Orientation (station & entry, string orientation) {

	// It is case insensitive so convert it to the uppercase and compare it

	string ss = uppercase(orientation);
	int length = ss.length();
	int length1 = abs (length);

	if (length1 < 4) {
		if (isdigit(ss[0])) {
			for (int i = 0; i < length1; i++) {
				if (!isdigit(ss[i])) {
					return false;
				}
			}

			return true;
		} else if (isalpha(ss[0])) {
			for (int i = 0; i < length1; i++) {
				if (ss[i] != 'N' && ss[i] != 'E' && ss[i] != 'Z') {
					return false;
				}
			}
			return true;
		}
	}
	return false;
} 

string get_Orientation (station & entry) {
	return entry.orientation;
}

//************************ Month **************************/

string get_Month_Num2namestr (Months aa) {
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

// Check the header of the input file
bool check_input_header(ifstream &inputfile, ofstream &outputifle) {

	// Declare variable types:
	// Date & Time variables("mm/dd/yyyy or mm-dd-yyyy hh:mm:ss.fff time_zone"):

	double longitude = 0, latitude = 0, depth = 0;
	int mm;

	string date, day, month, year, time, hour, minute, second; 
	string line, time_zone, magnitude_type, magnitude_size;
	string longitude1, latitude1, depth1;
	stringstream longt, lat, dep;
	ofstream logfile;

	// First line for event ID

	earthquake eq_info;
	//Date date_elements;

	getline(inputfile, line);
	stringstream eventID (line);
	eventID >> eq_info.id;

	getline(inputfile, line);
	stringstream datetime (line);
	datetime >> eq_info.date;
	date = eq_info.date;

	datetime >> eq_info.time;
	time = eq_info.time;

	datetime >> eq_info.timeZone;
	time_zone = eq_info.timeZone;

	getline(inputfile, line);
	eq_info.earthquake_name = line;

	getline(inputfile, line);
	stringstream epicenter (line);

	epicenter >> eq_info.longtidue;
	longitude1 = eq_info.longtidue;
	longt << longitude1;
	longt >> longitude;

	epicenter >> eq_info.latitude;
	latitude1 = eq_info.latitude;
	lat << latitude1;
	lat >> latitude;

	epicenter >> eq_info.depth;
	depth1 = eq_info.depth;
	dep << depth1;
	dep >> depth;

	epicenter >> eq_info.magnitude_type;
	magnitude_type = eq_info.magnitude_type;

	epicenter >> eq_info.magnitude_size;
	magnitude_size = eq_info.magnitude_size;

	// check the stored data validation

	//string get_Month_Num2namestr, day, year;

	set_date(eq_info, logfile, date, month, day, year, mm);
	set_time(eq_info, logfile, time, hour, minute, second);
	set_time_zone(eq_info, logfile, time_zone);
	set_magnitude(eq_info, logfile, eq_info.magnitude_type, eq_info.magnitude_size);

	// Print the header in the outputifle:

	outputifle << "# " << " " << get_Month_Num2namestr(month_num2enum(mm)) << " " << day << " " << year << " " 
		<< time << " " << time_zone << " " << magnitude_type << " " << magnitude_size 
		<< " " << eq_info.earthquake_name << " " << "[" << eq_info.id << "]" << "(" 
		<< longitude << "," << " " << latitude << "," << " " << depth << ")" << "\n";

	return 0;
}

// Check the table of reports from input file and return the signals output
bool check_input_signals(ifstream & inputfile, station entry[MAXvalidentry], int valid_entries, int invalidEntries, int produced_signalnum) {

	int entry_pos = 0;

	ofstream logfile;
	ofstream outputfile;
	string net_code, Station_Name, band_Type, inst_type, orientation;
	string string;

	// check the stored data validation

	while (valid_entries < MAXvalidentry) {             // or wile inputfile.eof()

		int m = -1;

		cout << "reading signals";

		inputfile >> net_code;

		entry_pos++;

		if (!is_valid_Network_code(net_code)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid_Network_code");
			m++;
			return true;
		}

		cout << entry_pos;

		entry_pos++;
		inputfile >> Station_Name;
		if (!is_valid_Station_code(Station_Name)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid_Station_code");
			m++;
			return true;
		}

		entry_pos++;
		inputfile >> band_Type;
		if ( !is_valid_Type_of_band(band_Type)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_band");
			m++;
			return true;
		}

		entry_pos++;
		inputfile >> inst_type;
		if ( !is_valid_Type_of_instrument(inst_type)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_band");
			m++;
			return true;
		}

		entry_pos++;
		inputfile >> orientation;
		if ( !is_valid_Orientation(orientation)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. as an invalid Orientation");
			m++;
			return true;
		}

		if (m == -1) {

			valid_entries++;
			produced_signalnum = produced_signalnum + orientation.size(); 
			
			entry[valid_entries].net_code     =  str2Net_code(net_code);
			entry[valid_entries].Station_Name =  Station_Name;
			entry[valid_entries].band_type    =  str2Band_Type(band_Type);
			entry[valid_entries].inst_type    =  Inst_Type_str2enum(inst_type);
			entry[valid_entries].orientation  =  orientation;

			return true;

		} else {
		invalidEntries++;
		}
		return false;
	}
}

void print_output(ofstream & outputfile, ofstream & logfile, station entry[MAXvalidentry], int valid_entries, int invalidEntries, int produced_signalnum) {

	string orientation;
	int length = orientation.length();
	int length1 = abs (length);


	// print all the signals to the output file
			for (int i = 0; i < valid_entries; i++) {
				for (int j = 0; j < length1; j++) {

					stringstream records;
					records << entry[valid_entries].net_code << ".";
					records << entry[valid_entries].Station_Name << ".";
					records << entry[valid_entries].band_type;
					records << entry[valid_entries].inst_type;
					records << entry[valid_entries].orientation[j] << endl;

					outputfile << records.str();
					cout << records.str();
				}
			}

			outputfile << valid_entries << "\n";
			print_message(logfile, "invalid entries ignored:");
			print_position(logfile, invalidEntries);
			print_message(logfile, "valid entries read:");
			print_position(logfile, valid_entries);
			print_message(logfile, "signal name produced");
			print_position(logfile, cout, produced_signalnum);
			print_message(logfile, "Finished!");

			outputfile.close();
}