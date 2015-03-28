/****************************************************************************************************
* Programming Tools for  Engineers and Scientists - HW6
* by Golnaz Sarram
* March 20th, 2015

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

struct earthquake {
	string id;
	string date;
	string time;
	string timeZone;
	string earthquake_name;
	double latitude;
	double longitude;
	Mag_type magnitude_type;
	string magnitude_size;
	double depth;
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

//ofstream outputfile;
//ofstream logfile;
//ifstream inputfile;

string   inputfilename = "golnaz.in", outputfilename = "golnaz.out", logfilename = "golnaz.log";

//================================= Function Prototypes ==================================/

bool read_input(string, string, int, int &, int);
bool check_input_header(ifstream &, ofstream &);
bool set_valid_Network_code(station &, string);
bool set_valid_Station_code(station &, string);
bool set_valid_Type_of_band(station &, string);
bool set_valid_Type_of_instrument(station &, string);
bool set_valid_Orientation(station &, string);

void read_input_signals(ifstream &, ofstream &, station entry[MAXvalidentry], int&, int&, int&);
void print_message(ofstream &, const string &);
void print_position(ofstream &, int &);
void open_input(ifstream &, ofstream &, ostream &, string);
void open_file(ofstream &, ostream &, string);
void print_output(ofstream &, ofstream &, station entry[MAXvalidentry], int &, int &, int&);
void set_eventid(earthquake&, ifstream &, string);
void set_date(earthquake &, ofstream &, string &, string &, string &, string &, int &);
void set_time(earthquake &, ofstream &, string, string &, string &, string &);
void set_time_zone(earthquake &, ofstream &, string);
void set_magnitude_size(earthquake &, ofstream &, string);
void set_magnitude_type(earthquake &, ofstream &, string);

string get_Month_Num2namestr(Months);
string get_mag_type2str(Mag_type);
string uppercase(string &);
string get_eventid(earthquake);
string get_date(earthquake &);
string get_time(earthquake &);
string get_magnitude_size(earthquake &);
string get_mag_type2str(earthquake, string);
string get_Net_code2namestr(station &);
string get_Type_of_band2str(station &);
string get_Station_code(station &);
string get_Inst_Type2str(station &);
string get_Orientation(station &);

Months month_num2enum(int);
Mag_type str2Mag_type(string);
Net_code str2Net_code(string);
Band_Type str2Band_Type(string);
Inst_Type Inst_Type_str2enum(string);


//=========================================================================================/

int main() {

	string station_code;
	ifstream inputfile;
	ofstream outputfile, logfile;
	station entry[MAXvalidentry];
	string net_code;

	int valid_entries = 0, invalidEntries = 0, produced_signalnum = 0;

	// Generating the inputfile, outputfile and logfile

	open_input(inputfile, logfile, cout, inputfilename);
	open_file(logfile, cout, "golnaz.log");
	open_file(outputfile, cout, "golnaz.out");

	// Calling the read_input function

	check_input_header(inputfile, outputfile);
	print_message(logfile, "header is read successfully!");

	read_input_signals(inputfile, outputfile, entry, valid_entries, invalidEntries, produced_signalnum);
	//print_output(outputfile, logfile, entry, valid_entries, invalidEntries, produced_signalnum);
	print_message(logfile, "signals are read correctly!");

	inputfile.close();
	outputfile.close();

	//getchar();
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

// Function for printing the errors and results into the outputfile and terminal

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

//************************ set & get functions *************************/

void set_eventid(earthquake& eq_info, ifstream & inputfile, string line) {

	//stringstream eventID (line);
	eq_info.id = line;
	//eq_info.id = line;
}

string get_eventid(earthquake & eq_info) {
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

		if (!isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3]) || !isdigit(date[4])) {
			print_message(logfile, "Error: Date of earthquake is not valid. ");
			//exit (EXIT_FAILURE);
		}

		if (!isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9])) {
			print_message(logfile, "Error: Date of earthquake is not valid. ");
			//exit (EXIT_FAILURE);
		} else {
			if (mm < 0 || mm > 13 || dd < 0 || dd > 32 || yyyy < 1850 || yyyy > 2016) {
				print_message(logfile, "Error: Date digits are not valid. ");
				exit(EXIT_FAILURE);
			}
		}

		if ((date[2] != '/' || date[5] != '/') && (date[2] != '-' || date[5] != '-')) {
			print_message(logfile, "Error: Date format is not valid. ");
			//exit (EXIT_FAILURE);
		}

		eq_info.date = date;

		cout << "date : " << eq_info.date << "\n";

		/*
		Date.year = year;
		Date.month = month;
		Date.day = day;
		*/

	} else {
		print_message(logfile, "Error: Date of earthquake is not valid. ");
		//exit (EXIT_FAILURE);
	}
}

string get_date(earthquake & eq_info) {
	return eq_info.date;
}

//============================= time ===================================/

void set_time(earthquake & eq_info, ofstream & logfile, string time, string & hour, string & minute, string & second) {

	int hr, min;
	float  sec = 0;
	stringstream hr1, min1, sec1;
	//earthquake eq_info;

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

		if (!isdigit(time[0]) || !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4])) {
			print_message(logfile, "Error: time of earthquake is not valid. ");
			exit(EXIT_FAILURE);
		}

		if (!isdigit(time[6]) || !isdigit(time[7]) || !isdigit(time[9]) || !isdigit(time[10]) || !isdigit(time[11])) {
			print_message(logfile, "Error: time of earthquake is not valid. ");
			exit(EXIT_FAILURE);
		}

		if (hr < 0 || hr > 24 || min < 0 || min > 60 || sec < 0.0009 || sec > 59.9999) {
			print_message(logfile, "Error: time digits are not valid. ");
			//exit (EXIT_FAILURE);
		}
		if (time[2] != ':' || time[5] != ':' || time[8] != '.') {
			print_message(logfile, "Error: time format is not valid.");
			//exit (EXIT_FAILURE);
		}

		eq_info.time = time;

		// (Or I could add another struct for time and use atoi(.c_str) command to convert the string into integer
		// and but it needs more set and get functions which I prefered to learn compile and run completely

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

string get_time(earthquake & eq_info) {
	return eq_info.time;
}

//======================== time_zone ============================/

void set_time_zone(earthquake & eq_info, ofstream & logfile, string time_zone) {

	int tzl = 0;
	string str = time_zone;
	const char *cstr = str.c_str();
	tzl = strlen(cstr);
	if ((tzl != 3) || (!isalpha(time_zone[0])) || (!isalpha(time_zone[1])) || (!isalpha(time_zone[2]))) {
		print_message(logfile, "Error: Time_zone is not valid");
		//exit (EXIT_FAILURE);
	} else {
		eq_info.timeZone = time_zone;
	}
}

string get_time_zone(earthquake & eq_info) {
	return eq_info.timeZone;
}

//======================== Mag_type ============================/

// Functions for converting the entries to uppercase and check for validation 

string uppercase(string & s) {
	string result = s;
	for (int i = 0; i < (int)result.size(); i++)
		result[i] = toupper(result[i]);
	return result;
}

void set_magnitude_size(earthquake & eq_info, ofstream & logfile, string magnitude_size) {

	int mag_size;
	stringstream mg;
	mg << magnitude_size;
	mg >> mag_size;

	if (mag_size < 0) {
		print_message(logfile, "Error: The magnitude_size is not valid");
		//exit (EXIT_FAILURE);
	} else {
		eq_info.magnitude_size = magnitude_size;
	}
}

string get_magnitude_size(earthquake & eq_info) {
	return eq_info.magnitude_size;
}

void set_magnitude_type(earthquake & eq_info, ofstream & logfile, string magnitude_type) {

	// cout << magnitude_type;
	string mt = uppercase(magnitude_type);


	if (mt == "ML") { eq_info.magnitude_type = ML; }
	if (mt == "MS") { eq_info.magnitude_type = Ms; }
	if (mt == "MB") { eq_info.magnitude_type = Mb; }
	if (mt == "MW") { eq_info.magnitude_type = Mw; }

	//print_message(logfile, "Error: The magnitude_type is not valid");
	//exit (EXIT_FAILURE);
}

string get_mag_type2str(earthquake eq_info, string magnitude_type) {

	switch (eq_info.magnitude_type) {

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

//****************** Network_code ***********************/

bool set_valid_Network_code(station & entry, string Net_code) {          // mashkukam be in?!

	if (Net_code.compare("CE") == 0) {
		entry.net_code = CE;
		return true;
	}

	if (Net_code.compare("CI") == 0) {
		entry.net_code = CI;
		return true;
	}

	if (Net_code.compare("FA") == 0) {
		entry.net_code = FA;
		return true;
	}

	if (Net_code.compare("NP") == 0) {
		entry.net_code = NP;
		return true;
	}

	if (Net_code.compare("WR") == 0) {
		entry.net_code = WR;
		return true;
	}

	else return false;
}

string get_Net_code2namestr(station & entry) {
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

	string ss = uppercase(nt);                                         // bayad case sensitive bashad ke pas !?

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
				entry.Station_Name = Stati_code;
				return true;
			}
		}
	}

	if (Stati_code.length() == 3) {
		for (int i = 0; i < 4; i++) {
			if (isalpha(Stati_code[i])) {
				if (isupper(Stati_code[i])) {
					entry.Station_Name = Stati_code;
					return true;
				}
			}
		}
	}	

	return false;
}

string get_Station_code(station & entry) {
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

string get_Type_of_band2str(station & entry) {
	switch (entry.band_type) {
	case LongPeriod:   return "L";
	case ShortPeriod:  return "B";
	case Broadband:    return "H";
	}
	// It should never get here!!
	exit(EXIT_FAILURE);
}

Band_Type str2Band_Type(string d) {

	string ss = uppercase(d);

	if (ss == "LONG-PERIOD")  
		return LongPeriod;
	else if (ss == "SHORT_PERIOD")  
		return ShortPeriod;
	else 
		return Broadband;

	// It should never get here!!
	//exit(EXIT_FAILURE);
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

	if (ss == "HIGH-GAIN")  return HighGain;
	else if (ss == "LOW-GAIN")  return LowGain;
	else return Accelerometer;

}

//******************** orientation **********************/

bool set_valid_Orientation(station & entry, string orientation) {

	// It is case insensitive so convert it to the uppercase and compare it

	string ss = uppercase(orientation);
	int length = ss.length();
	int length1 = abs(length);

	if (length1 < 4) {
		if (isdigit(ss[0])) {
			for (int i = 0; i < length1; i++) {
				if (!isdigit(ss[i])) {
					return false;
				}
			}

			entry.orientation = ss;

			return true;
		}
		else if (isalpha(ss[0])) {
			for (int i = 0; i < length1; i++) {
				if (ss[i] != 'N' && ss[i] != 'E' && ss[i] != 'Z') {
					return false;
				}
			}

			entry.orientation = ss;

			return true;
		}
	}
	return false;
}

string get_Orientation(station & entry) {
	return entry.orientation;
}

//************************ Month **************************/

string get_Month_Num2namestr(Months aa) {
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

Months month_num2enum(int a){

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

//**********************************************************/

// Check the header of the input file
bool check_input_header(ifstream & inputfile, ofstream & outputfile) {

	// Declare variable types:
	// Date & Time variables ("mm/dd/yyyy or mm-dd-yyyy hh:mm:ss.fff time_zone"):

	double longitude = 0, latitude = 0, depth = 0;
	int mm;

	string date, day, month, year, time, hour, minute, second;
	string line, time_zone, magnitude_type, magnitude_size;
	string longitude1, latitude1, depth1;
	stringstream longt, lat, dep;
	ofstream logfile;

	earthquake eq_info;

	// Check data validation and storing them
	// First line for event ID

	getline(inputfile, line);

	set_eventid(eq_info, inputfile, line);

	// Second line for date

	inputfile >> date >> time >> time_zone;
	set_date(eq_info, logfile, date, month, day, year, mm);

	set_time(eq_info, logfile, time, hour, minute, second);
	get_time (eq_info);

	set_time_zone(eq_info, logfile, time_zone);

	// Third line for earthquake name

	inputfile.ignore();
	getline(inputfile, line);
	eq_info.earthquake_name = line;

	// Fourth line for epicenter info

	inputfile >> eq_info.latitude;
	inputfile >> eq_info.longitude;
	inputfile >> eq_info.depth;

	inputfile >> magnitude_type;

	cout << "mag_type : " << magnitude_type << "\n";

	set_magnitude_type(eq_info, logfile, magnitude_type);
	//get_mag_type2str(eq_info, magnitude_type);

	inputfile >> magnitude_size;
	set_magnitude_size(eq_info, logfile, magnitude_size);

	// Print the header in the outputfile:

	outputfile << "# " << " " << get_Month_Num2namestr(month_num2enum(mm)) << " " << day << " " << year << " "
		<< eq_info.time << " " << eq_info.timeZone << " " << eq_info.magnitude_type << " " << eq_info.magnitude_size
		<< " " << eq_info.earthquake_name << " " << "[" << eq_info.id << "]" << "("
		<< eq_info.longitude << "," << " " << eq_info.latitude << "," << " " << eq_info.depth << ")" << "\n";

	return 0;
}

// Check the table of reports from input file and return the signals output
void read_input_signals(ifstream & inputfile, ofstream& outputfile, station entry[MAXvalidentry], int  & valid_entries, int & invalidEntries, int & produced_signalnum) {

	int entry_pos = 0;

	ofstream logfile;
	string Net_code, Station_Name, band_Type, inst_type, orientation;
	string string, temp_holder;
	int i = 0;
	// check the stored data validation

	//inputfile >> temp_holder;

	//for (int i = 0; inputfile. != NULL && valid_entries < MAXvalidentry; i++) {             // or wile inputfile.eof()
	while ((inputfile >> Net_code) && valid_entries < MAXvalidentry) {
		int m = -1;

		cout << "reading signals";

		entry_pos++;

		;
		if (!set_valid_Network_code(entry[i], Net_code)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid_Network_code");
			m++;
		}

		cout << entry_pos;

		entry_pos++;
		inputfile >> Station_Name;
		if (!set_valid_Station_code(entry[i], Station_Name)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid_Station_code");
			m++;

		} else {
			entry[i].Station_Name = Station_Name;
		}

		entry_pos++;
		inputfile >> band_Type;
		if (!set_valid_Type_of_band(entry[i], band_Type)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_band");
			m++;

		} else {
			entry[i].band_type = str2Band_Type(band_Type);//band_Type;
		}

		entry_pos++;
		inputfile >> inst_type;
		if (!set_valid_Type_of_instrument(entry[i], inst_type)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. Invalid Type_of_band");
			m++;

		} else { 
			entry[i].inst_type = Inst_Type_str2enum(inst_type);
		}


		entry_pos++;
		inputfile >> orientation;
		if (!set_valid_Orientation(entry[i], orientation)) {
			print_message(logfile, "Error: Entry # ");
			print_position(logfile, entry_pos);
			print_message(logfile, "ignored. as an invalid Orientation");
			m++;

		} else {
			entry[i].orientation = orientation;
		}

		if (m == -1) {

			valid_entries++;
			produced_signalnum = produced_signalnum + orientation.size();

			print_output(outputfile, logfile, entry, valid_entries, invalidEntries, produced_signalnum);

		} else {
			invalidEntries++;
		}
		i++;
	}

	outputfile << valid_entries << "\n";
	print_message(logfile, "invalid entries ignored:");
	print_position(logfile, invalidEntries);
	print_message(logfile, "valid entries read:");
	print_position(logfile, valid_entries);
	print_message(logfile, "signal name produced");
	print_position(logfile, produced_signalnum);
	print_message(logfile, "Finished!");

}

void print_output(ofstream & outputfile, ofstream & logfile, station entry[MAXvalidentry], int & valid_entries, int & invalidEntries, int & produced_signalnum) {

	// print all the signals to the output file

	string orientation;

	for (int i = 0; i < valid_entries; i++) {
		orientation = get_Orientation(entry[i]);

		for (int j = 0; j < orientation.length(); j++) {
			stringstream records;
			records << get_Net_code2namestr(entry[i]) << "." ;
			records << get_Type_of_band2str(entry[i]) << ".";
			records << get_Station_code(entry[i]);
			records << get_Inst_Type2str(entry[i]);
			records << orientation[j] << "\n";

			outputfile << records.str();
			cout << records.str();
		}
	}
}