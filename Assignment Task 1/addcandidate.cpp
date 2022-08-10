/*
Name: Sze XiJie (101222928), Chin Kai Lun (101221815), Chen Jun Yao (101222889), Ooi Yik Quan (101230222)
Project Team Name: Amadeus
Lab Group: Lab 2 Group 1
Project Title: Election-Voter
Project Description: Enables voters and candidates to join the election and vote with ease
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sqlite3.h>
#include <algorithm>
#include <cctype>

using namespace std;

// Calling all functoions
void mainmenu(); void mainselectionmenu(bool* quit, string selection, string casearray[]);
static int createcandidatedb(const char* s); static int createcandidatetable(const char* s);
static int selectcandidatedata(const char* s); static int callcandidatedata(void* NotUsed, int argc, char** argv, char** azColName);
int divisionsizecheck(const char* s); void viewcandidateinfo(); void divisioncategorize();
static int insertdivisiondata(const char* s, int divisionnumber, int partynumber, int tablesize, string partyname, string candidateidname);
static int partycheck(const char* s, int partysize); vector <string> usedcandidateid(const char* s);
static int usedpartyname(const char* s, int division);

// Divide the candidates into random division
void divisioncategorize()
{
	bool categorize = false;

	const char* division = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	createcandidatedb(division); // Creates The Database If It Is Not Created
	createcandidatetable(division); // Creates The Candidate Table

	srand(time(NULL)); // Set Random Number To Random So That It Will Always Generate Random Numbers

	while (!categorize)
	{
		int randomdivision = ((rand() % 4) + 1); // Generate Random Number From 1 To 4

		if (divisionsizecheck(division) < 12) // Check If The Database Has Reached 12 Candidates
		{
			int partylimit = partycheck(division, randomdivision); // Get The Return Value From partycheck function

			string party;
			string upperletterparty;
			string candidateid;

			string partyname[3] = { "Einstein", "Tesla", "Mozart" }; // Party Name

			switch (randomdivision)
			{
			case 1:
			{
				if (partylimit <= 3) // If The Party In Division 1 Is Less Than 3
				{
					party = partyname[usedpartyname(division, randomdivision)]; // Assign Party Name
					upperletterparty = party;
					transform(upperletterparty.begin(), upperletterparty.end(), upperletterparty.begin(), ::toupper); // Changes The User Input To All UpperCase

					candidateid = upperletterparty.substr(0, 3) + "0" + to_string(randomdivision); // Assign Candidate ID

					insertdivisiondata(division, randomdivision, partylimit, divisionsizecheck(division), party, candidateid); // Call insertdivisiondata Function To Insert Candidate

					categorize = true;

					break;
				}
				else
				{
					categorize = false;
				}
			}
			case 2:
			{
				if (partylimit <= 3) // If The Party In Division 2 Is Less Than 3
				{
					party = partyname[usedpartyname(division, randomdivision)]; // Assign Party Name
					upperletterparty = party;
					transform(upperletterparty.begin(), upperletterparty.end(), upperletterparty.begin(), ::toupper); // Changes The User Input To All UpperCase

					candidateid = upperletterparty.substr(0, 3) + "0" + to_string(randomdivision); // Assign Candidate ID

					insertdivisiondata(division, randomdivision, partylimit, divisionsizecheck(division), party, candidateid); // Call insertdivisiondata Function To Insert Candidate

					categorize = true;

					break;
				}
				else
				{
					categorize = false;
				}
			}
			case 3:
			{
				if (partylimit <= 3) // If The Party In Division 3 Is Less Than 3
				{
					party = partyname[usedpartyname(division, randomdivision)]; // Assign Party Name
					upperletterparty = party;
					transform(upperletterparty.begin(), upperletterparty.end(), upperletterparty.begin(), ::toupper); // Changes The User Input To All UpperCase

					candidateid = upperletterparty.substr(0, 3) + "0" + to_string(randomdivision); // Assign Candidate ID

					insertdivisiondata(division, randomdivision, partylimit, divisionsizecheck(division), party, candidateid); // Call insertdivisiondata Function To Insert Candidate

					categorize = true;

					break;
				}
				else
				{
					categorize = false;
				}
			}
			case 4:
			{
				if (partylimit <= 3) // If The Party In Division 4 Is Less Than 3
				{
					party = partyname[usedpartyname(division, randomdivision)]; // Assign Party Name
					upperletterparty = party;
					transform(upperletterparty.begin(), upperletterparty.end(), upperletterparty.begin(), ::toupper); // Changes The User Input To All UpperCase

					candidateid = upperletterparty.substr(0, 3) + "0" + to_string(randomdivision); // Assign Candidate ID

					insertdivisiondata(division, randomdivision, partylimit, divisionsizecheck(division), party, candidateid); // Call insertdivisiondata Function To Insert Candidate

					categorize = true;

					break;
				}
				else
				{
					categorize = false;
				}
			}
			default:
				break;
			}
		}
		else
		{
			cout << "I Am Sorry, Candidate Registration Is Over.\n" << endl;  // Display A Tex Message If The Candidate Has Reached 12 
			categorize = true;
		}
	}
}

// View Candidate Information
void viewcandidateinfo()
{
	const char* division = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	if (divisionsizecheck(division) == 0)
	{
		cout << "There Is No Candidate." << endl; // Display A Text Message When There Is No Candidate In The Database
	}
	else
	{
		createcandidatedb(division); // Creates The Database If It Is Not Created
		createcandidatetable(division); // Creates The Candidate Table
		selectcandidatedata(division); // Calls selectcandidatedata Function To Select The Candidate Information From Database And Display It
	}
}

// Creates The Database
static int createcandidatedb(const char* s)
{
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open(s, &DB); // Opens The Database

	sqlite3_close(DB); // Close The Database

	return 0;
}

// Creates The Candidate Table In The Database
static int createcandidatetable(const char* s)
{
	sqlite3* DB;
	int exit = 0;
	char* messageerror;

	string sql = "CREATE TABLE IF NOT EXISTS CandidateTable("
		"Name      TEXT NOT NULL, "
		"CandidateID     TEXT NOT NULL, "
		"Party  TEXT NOT NULL, "
		"Division     TEXT NOT NULL, "
		"Vote     TEXT NOT NULL );"; // A Database Format From sqlite3

	exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageerror);

	return 0;
}

// Inserts Candidate Information Into The Database
static int insertdivisiondata(const char* s, int divisionnumber, int partynumber, int tablesize, string partyname, string candidateidname)
{
	sqlite3* DB;
	char* messageError;

	bool insertname = false;

	string candidatename;
	string candidateid = candidateidname;
	string party = partyname;
	string division = to_string(divisionnumber);
	string votecount = "0";

	while (!insertname)
	{
		cout << "Enter Your Name (With No Space): " << endl;
		getline(cin, candidatename); // Receives User Input For Their Name

		srand(time(NULL)); // Set Random Number To Random So That It Will Always Generate Random Numbers

		system("cls"); // Clears the previous displayed text on the console

		if (all_of(begin(candidatename), end(candidatename), isalpha)) // Check If The User Input Is All Letters
		{
			insertname = true;
		}
		else // User Input Is Wrong
		{
			cout << "Please Check Again, There Seems To Be An Error." << endl; // Display A Text Message
			insertname = false;
		}
	}

	string sql = ("INSERT INTO CandidateTable (Name, CandidateID, Party, Division, Vote) VALUES('" + candidatename + "','" + candidateid + "','" + party + "','" + division + "','" + votecount + "');"); // Inserts The User Input Into The Database

	int exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK) // If There Is An Error When Inserting Data
	{
		cerr << "Error In Inserting Data!\n" << endl; // Display Error Message
		sqlite3_free(&messageError); // Remove The Error Message
	}
	else // If There Is No Error When Inserting Data
		cout << "Data Inserted Successfully!\n" << endl; // Display A Text Message

	return 0;
}

// Select Candidate Data
static int selectcandidatedata(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "SELECT * FROM CandidateTable ORDER BY Division ASC;"; // Specify Which Data To Select

	int exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callcandidatedata, NULL, &messageError);

	if (exit != SQLITE_OK) // If There Is An Error When Selecting Data
	{
		cerr << "Error in Selecting Data!\n" << endl; // Display Error Message
		sqlite3_free(&messageError); // Remove The Error Message
	}
	else // If There Is No Error When Selecting Data
		cout << "Data Selected Successfully\n" << endl; // Display A Text Message

	return 0;
}

// Calls The Candidate Data To Be Displayed
static int callcandidatedata(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << ": " << argv[i] << endl; // Display The Column Name Followed By The Data
	}

	cout << endl; // Create New Line

	return 0;
}

static int usedpartyname(const char* s, int division)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	int partynumber;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Count(Party) FROM CandidateTable WHERE Division = '" + to_string(division) + "'; "; // Specify The Data To Select

	rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL); // Specify Which Data To Select

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}


		if (rc != SQLITE_ROW) // If There Is No Data In Database
		{
			partynumber = 0; // Set maxcandidate To 0
			break;
		}

		partynumber = sqlite3_column_int(stmt, 0); // Assign Data To maxcandidate
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return partynumber;
}

// Store Used Candidate ID
vector <string> usedcandidateid(const char* s)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	string id;

	vector <string> candidateid;
	int i = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	rc = sqlite3_prepare_v2(DB, "SELECT CandidateID FROM CandidateTable;", -1, &stmt, NULL); // Specify Which Data To Select

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}

		string name = (const char*)(sqlite3_column_text(stmt, 0)); // Assign The Data To Name Variable

		candidateid.push_back(name); // Insert The Data Into Vector
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return candidateid;
}

// Check Size of Candidate Table
static int divisionsizecheck(const char* s)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	int maxcandidate = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	rc = sqlite3_prepare_v2(DB, "SELECT Count(Party) FROM CandidateTable;", -1, &stmt, NULL); // Specify The Data To Select

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}


		if (rc != SQLITE_ROW) // If There Is No Data In Database
		{
			maxcandidate = 0; // Set maxcandidate To 0
			break;
		}

		maxcandidate = sqlite3_column_int(stmt, 0); // Assign Data To maxcandidate
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return maxcandidate;
}

// Check Party Size
static int partycheck(const char* s, int division)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	int maxparty = 0;

	string sql = "SELECT Count(Party) FROM CandidateTable WHERE Division = '" + to_string(division) + "'; "; // Specify The Data To Select

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/

	int rc = sqlite3_open(s, &DB); // Opens The Database

	rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL); // Gets The Data Following The Specified Data To Select

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}

		maxparty = sqlite3_column_int(stmt, 0) + 1; // Assign Data To maxparty
	}

	sqlite3_finalize(stmt); // Finalize the Data Reading

	return maxparty;
}

// mainmenu() is the function that stores the codes for the main menu screen
void mainmenu()
{
	bool mnquit = false;
	bool* quit = &mnquit;

	// While the user did not quit the program
	while (!*quit)
	{
		// Prompt the user to select an option
		string selection;
		string mainmenuarray[] = { "Adding Candidate\n", "Viewing Candidates\n", "Exiting Program", "Your Selected Option Is Invalid, Please Try Again!\n" }; // Array For Storing Text

		cout << "Please enter the respective numbers to select the option!\n[1] Add Candidate\n[2] View Candidates\n[3] Exit\n" << endl;
		getline(cin, selection); // Gets User Input

		system("cls"); // Clears the previous displayed text on the console

		mainselectionmenu(quit, selection, mainmenuarray); // Calls mainselectionmenu function
	}
}

// Displays the option for the user
void mainselectionmenu(bool* quit, string selection, string casearray[])
{
	if (selection == "1") // If the user selected option 1
	{
		cout << casearray[0] << endl; // Display the text for option 1
		divisioncategorize();  // Calls registrationmenu function
	}
	else if (selection == "2") // If the user selected option 2
	{
		cout << casearray[1] << endl; // Display the text for option 2
		viewcandidateinfo(); // Calls candidatedisplay function
	}
	else if (selection == "3")// If the user selected option 3
	{
		*quit = true; // Quit the program
		cout << casearray[2] << endl; // Display text for option 3
	}
	else // If user input anything else
	{
		cout << casearray[3] << endl; // Display text for wrong input
	}
}

int main()
{
	mainmenu();
}