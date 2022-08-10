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

// Calling The Functions
void VoterMainMenu();
void MainSelectionMenu(bool* Quit, string Selection, string CaseArray[]);
void ViewingCandidateMenu();
void ViewingSelectionMenu(bool* Quit, string Selection, string CaseArray[]);
void ViewResultMenu(); void ResultSelectionMenu(bool* Quit, string Selection, string CaseArray[]);
static int createcandidatedb(const char* s);
static int createcandidatetable(const char* s);
static int createvoterdb(const char* s);
static int createvotertable(const char* s);
static int selectcandidatedata(const char* s);
static int selectbasedonparty(const char* s, string index);
static int selectbasedondivision(const char* s, string division);
static int selectalldivision(const char* s);
static int minmaxvotealldivision(const char* s, int divisionchoosen, int maximumvote, int minimumvote);
static int callcandidatedata(void* NotUsed, int argc, char** argv, char** azColName);
int divisionsizecheck(const char* s);
void viewcandidateinfo(); void VoterDatabase();
static int insertvoterdata(const char* s);
static int partycheck(const char* s, int partysize); vector <string> usedcandidateid(const char* s);
static int usedpartyname(const char* s, int division);
void viewallcandidateinfo();
void viewdivision(string divisionnumber);
void viewparty(string partyindex); void VoteCandidate();
static int counttotalvote(const char* s, int divisionchoosen);
static int callvotedata(void* NotUsed, int argc, char** argv, char** azColName);
static int maxvotecount(const char* s, int divisionchoosen);
static int minvotecount(const char* s, int divisionchoosen);
static int callvoterdata(void* NotUsed, int argc, char** argv, char** azColName);
static int selectvoterdata(const char* s);
static string checkvoterstatus(const char* s, string voterid);
static string checkvoterdivision(const char* s, string voterid);
static int candidateinfovoteselection(const char* s, string division);
vector <string> storevotereid(const char* s);
vector <string> usedcandidateidbasedondivision(const char* s, string division);
static int candidatevotebasedonid(const char* s, string candidateid);
static int votecountupdate(const char* s, string votecount, string candidateid);

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

// Voter's Database
void VoterDatabase()
{
	const char* voter = R"(C:\\StoreData\\Voter.db)"; // Calls The Database

	createvoterdb(voter);
	createvotertable(voter);
	insertvoterdata(voter);
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

	string sql = "CREATE TABLE IF NOT EXISTS VoterTable("
		"Name      TEXT NOT NULL, "
		"CandidateID     TEXT NOT NULL, "
		"Party     TEXT NOT NULL, "
		"Division     TEXT NOT NULL, "
		"Vote     TEXT NOT NULL );"; // A Database Format From sqlite3

	exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageerror);

	return 0;
}

// Creates The Voter Database
static int createvoterdb(const char* s)
{
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open(s, &DB); // Opens The Database

	sqlite3_close(DB); // Close The Database

	return 0;
}

// Creates The Voter Table In The Database
static int createvotertable(const char* s)
{
	sqlite3* DB;
	int exit = 0;
	char* messageerror;

	string sql = "CREATE TABLE IF NOT EXISTS VoterTable("
		"Name      TEXT NOT NULL, "
		"VoterID     TEXT NOT NULL, "
		"Age     TEXT NOT NULL, "
		"Division     TEXT NOT NULL, "
		"Status     TEXT NOT NULL );"; // A Database Format From sqlite3

	exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageerror);

	return 0;
}

// Inserts Voter Information Into The Database
static int insertvoterdata(const char* s)
{
	sqlite3* DB;
	char* messageError;

	bool insertname = false;
	bool insertage = false;
	bool insertdivision = false;

	string voterfirstname;
	string voterlastname;
	string votername;
	string age;
	string voterid;
	string division;
	string status = "N";

	while (!insertname)
	{
		cout << "Enter Your First Name: " << endl;
		getline(cin, voterfirstname); // Receives User Input For Their Name

		srand(time(NULL)); // Set Random Number To Random So That It Will Always Generate Random Numbers

		system("cls"); // Clears the previous displayed text on the console

		if (all_of(begin(voterfirstname), end(voterfirstname), isalpha)) // Check If The User Input Is All Letters
		{
			cout << "Enter Your Last Name: " << endl;
			getline(cin, voterlastname); // Receives User Input For Their Name

			system("cls"); // Clears the previous displayed text on the console

			if (all_of(begin(voterlastname), end(voterlastname), isalpha)) // Check If The User Input Is All Letters
			{
				while (!insertdivision)
				{
					cout << "Enter Your Desired Division: " << endl;
					getline(cin, division); // Receives User Input For Their Dvision

					system("cls"); // Clears the previous displayed text on the console

					if (division.find_first_not_of("1234") == string::npos)
					{
						while (!insertage)
						{
							cout << "Enter Your Age: " << endl;
							getline(cin, age); //Receives User Input For Their Age 

							system("cls"); // Clears the previous displayed text on the console

							if (age.find_first_not_of("0123456789") == string::npos)
							{
								if (stoi(age) < 19)
								{
									cout << "I Am Sorry, You Are Too Young To Be Registered As A Voter\n" << endl; // Display A Text Message
									insertname = true;
									insertage = true;
									insertdivision = true;
								}
								else
								{
									votername = voterfirstname + " " + voterlastname;
									voterid = voterfirstname + voterlastname;

									string sql = ("INSERT INTO VoterTable (Name, VoterID, Age, Division, Status) VALUES('" + votername + "','" + voterid + "','" + age + "','" + division + "','" + status + "');"); // Inserts The User Input Into The Database

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

									insertname = true;
									insertage = true;
									insertdivision = true;
								}
							}
							else // User Input Is Wrong
							{
								cout << "Please Check Again, There Seems To Be An Error." << endl; // Display A Text Message
							}
						}
					}
					else // User Input Is Wrong
					{
						cout << "I Am Sorry, You Entered A Wrong Division!\n" << endl; // Display A Text Message
					}
				}
			}
			else // User Input Is Wrong
			{
				cout << "Please Check Again, There Seems To Be An Error." << endl; // Display A Text Message
			}
		}
		else // User Input Is Wrong
		{
			cout << "Please Check Again, There Seems To Be An Error." << endl; // Display A Text Message
		}
	}

	return 0;
}

// View All Candidate Information
void viewallcandidateinfo()
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
		selectalldivision(division); // Calls selectalldivision Function To Select The Candidate Information From Database And Display It
	}
}

// Select Candidate Data From All Division
static int selectalldivision(const char* s)
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

// View All Candidate Information In Specific Division
void viewdivision(string divisionnumber)
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
		selectbasedondivision(division, divisionnumber); // Calls selectbasedondivision Function To Select The Candidate Information From Database And Display It
	}
}

// Select Candidate Data Based On Division
static int selectbasedondivision(const char* s, string division)
{
	sqlite3* DB;
	char* messageError;

	string sql = "SELECT * FROM CandidateTable WHERE Division = '" + division + "'; "; // Specify Which Data To Select

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

// View All Candidate Information In Specific Party
void viewparty(string partyindex)
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
		selectbasedonparty(division, partyindex); // Calls selectbasedonparty Function To Select The Candidate Information From Database And Display It
	}
}

// Select Candidate Data Based on Party
static int selectbasedonparty(const char* s, string index)
{
	sqlite3* DB;
	char* messageError;
	string party[] = { "Einstein", "Tesla" , "Mozart" };
	string partyname = party[stoi(index) - 1];

	string sql = "SELECT * FROM CandidateTable WHERE Party = '" + partyname + "'ORDER BY Division ASC; "; // Specify Which Data To Select

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

// Select Voter Data
static int selectvoterdata(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "SELECT * FROM VoterTable ORDER BY Division ASC;"; // Specify Which Data To Select

	int exit = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callvoterdata, NULL, &messageError);

	if (exit != SQLITE_OK) // If There Is An Error When Selecting Data
	{
		cerr << "Error in Selecting Data!\n" << endl; // Display Error Message
		sqlite3_free(&messageError); // Remove The Error Message
	}
	else // If There Is No Error When Selecting Data
		cout << "Data Selected Successfully\n" << endl; // Display A Text Message

	return 0;
}

// Select Maximum and Minimum Votes Candidate Data
static int minmaxvotealldivision(const char* s, int divisionchoosen, int maximumvote, int minimumvote)
{
	const char* division = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	sqlite3* DB;
	char* messageError;

	string maxvote = "SELECT Name, CandidateID, Party, Vote FROM CandidateTable WHERE Division = '" + to_string(divisionchoosen) + "' AND Vote = '" + to_string(maximumvote) + "'; "; // Select Max vote data

	string minvote = "SELECT Name, CandidateID, Party, Vote FROM CandidateTable WHERE Division = '" + to_string(divisionchoosen) + "' AND Vote = '" + to_string(minimumvote) + "'; "; // Select Min vote data

	int exit = sqlite3_open(s, &DB); // Opens The Database

	if (divisionsizecheck(division) == 0)
	{
		cout << "Sorry, There Is No Candidate!\n" << endl;
	}
	else
	{
		/* An open database, maxvote to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
		exit = sqlite3_exec(DB, maxvote.c_str(), callcandidatedata, NULL, &messageError);

		if (counttotalvote(division, divisionchoosen) != 0)
		{
			float percentage = (float)((maxvotecount(division, divisionchoosen) * 100) / counttotalvote(division, divisionchoosen));

			cout << "Percentage of Vote: " << percentage << "%\n" << endl;
		}
		else
		{
			cout << "Percentage of Vote: 0%\n" << endl;
		}

		/* An open database, minvote to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
		exit = sqlite3_exec(DB, minvote.c_str(), callcandidatedata, NULL, &messageError);

		if (counttotalvote(division, divisionchoosen) != 0)
		{
			float percentage = (float)((minvotecount(division, divisionchoosen) / counttotalvote(division, divisionchoosen)) * 100);

			cout << "Percentage of Vote: " << percentage << "%\n" << endl;
		}
		else
		{
			cout << "Percentage of Vote: 0%\n" << endl;
		}

		cout << "Total Vote: " << counttotalvote(division, divisionchoosen) << "\n" << endl;
	}

	return 0;
}

// Count the max votes in a division
static int maxvotecount(const char* s, int divisionchoosen)
{
	sqlite3* DB;
	sqlite3_stmt* stmt{};

	char* messageError;

	int maxvote = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT MAX(Vote) FROM CandidateTable WHERE Division = '" + to_string(divisionchoosen) + "'; "; // Specify The Data To Select

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
			maxvote = 0; // Set maxcandidate To 0
			break;
		}

		maxvote = sqlite3_column_int(stmt, 0); // Assign Data To maxcandidate
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return maxvote;
}

// Count the min votes in a division
static int minvotecount(const char* s, int divisionchoosen)
{
	sqlite3* DB;
	sqlite3_stmt* stmt{};

	char* messageError;

	int minvote = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT MIN(Vote) FROM CandidateTable WHERE Division = '" + to_string(divisionchoosen) + "'; "; // Specify The Data To Select

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
			minvote = 0; // Set maxcandidate To 0
			break;
		}

		minvote = sqlite3_column_int(stmt, 0); // Assign Data To maxcandidate
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return minvote;
}

// Count the total votes in a division
static int counttotalvote(const char* s, int divisionchoosen)
{
	sqlite3* DB;
	sqlite3_stmt* stmt{};

	char* messageError;

	int totalvote = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Vote FROM CandidateTable WHERE Division = '" + to_string(divisionchoosen) + "'; "; // Specify The Data To Select

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
			break;
		}

		totalvote = totalvote + sqlite3_column_int(stmt, 0); // Assign Data To totalvote
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return totalvote;
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

// Calls The Voter Data
static int callvoterdata(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << ": " << argv[i] << endl; // Display The Column Name Followed By The Data
	}

	cout << endl; // Create New Line

	return 0;
}

// Calls The Candidate Vote Data
static int callvotedata(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		azColName[i];
		argv[i];
	}

	return 0;
}

// Checks for party name used
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

// Check for voter's status
static string checkvoterstatus(const char* s, string voterid)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	string status;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Status FROM VoterTable WHERE VoterID = '" + voterid + "'; "; // Specify The Data To Select

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
			break;
		}

		status = (const char*)(sqlite3_column_text(stmt, 0)); // Assign Data To status
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return status;
}

// Store Used Candidate ID Based On Division
vector <string> usedcandidateidbasedondivision(const char* s, string division)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	vector <string> candidateid;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT CandidateID FROM CandidateTable WHERE Division = '" + division + "';";

	rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL); // Specify Which Data To Select

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

		if (rc != SQLITE_ROW) // If There Is No Data In Database
		{
			break;
		}
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return candidateid;
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

// Check Division Based On Voter ID
static string checkvoterdivision(const char* s, string voterid)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	string division;


	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Division FROM VoterTable WHERE VoterID = '" + voterid + "'; "; // Specify The Data To Select

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
			break;
		}

		division = (const char*)(sqlite3_column_text(stmt, 0)); // Assign Data To status
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return division;
}

// Select Candidate Information When Voter Is Voter
static int candidateinfovoteselection(const char* s, string division)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	char* messageError;

	int exit = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Name, CandidateID, Party FROM CandidateTable WHERE Division = '" + division + "'; "; // Specify The Data To Select

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

// Store Voter ID
vector <string> storevotereid(const char* s)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	vector <string> voterid;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	rc = sqlite3_prepare_v2(DB, "SELECT VoterID FROM VoterTable;", -1, &stmt, NULL); // Specify Which Data To Select

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}

		string id = (const char*)(sqlite3_column_text(stmt, 0)); // Assign The Data To Name Variable

		voterid.push_back(id); // Insert The Data Into Vector
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return voterid;
}

// Update Voters Status
static int voterstatusupdate(const char* s, string voterid)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE VoterTable SET Status = 'Y' WHERE VoterId = '" + voterid + "'");

	int exit = sqlite3_open(s, &DB);

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

	return 0;
}

// Update Candidates Vote
static int votecountupdate(const char* s, string votecount, string candidateid)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE CandidateTable SET Vote = '" + votecount + "' WHERE CandidateId = '" + candidateid + "'");

	int exit = sqlite3_open(s, &DB);

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

	return 0;
}

// Get The Candidate's Vote Count Based On Candidate ID
static int candidatevotebasedonid(const char* s, string candidateid)
{
	sqlite3* DB;
	sqlite3_stmt* stmt{};

	char* messageError;

	int votecount = 0;

	int rc = sqlite3_open(s, &DB); // Opens The Database

	string sql = "SELECT Vote FROM CandidateTable WHERE CandidateID = '" + candidateid + "'; "; // Specify The Data To Select

	rc = sqlite3_exec(DB, sql.c_str(), callvotedata, NULL, &messageError);

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
			votecount = 0; // Set votecount To 0
			break;
		}

		votecount = sqlite3_column_int(stmt, 0); // Assign Data To votecount
	}
	sqlite3_finalize(stmt); // Finalize the Data Reading

	return votecount;
}

// Vote For Candidate
void VoteCandidate()
{
	const char* voter = R"(C:\\StoreData\\Voter.db)"; // Calls The Database
	const char* candidate = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	bool voteridinput = false;
	bool candidateidinput = false;

	string voterid;
	string candidateid;
	string existingid = "N";

	while (!voteridinput)
	{
		cout << "Please Enter Your Voter ID: " << endl;
		getline(cin, voterid);

		system("cls");

		if (all_of(begin(voterid), end(voterid), isalpha)) // Check If The User Input Is All Letters
		{
			for (int i = 0; i < size(storevotereid(voter)); i++) // For Every Iteration Where i Is Less Than Size of Voter
			{
				if (storevotereid(voter)[i] == voterid) // Check if Voterid Exists
				{
					existingid = voterid;
					break;
				}
			}

			if (existingid == "N") // Check if VoterID Exists
			{
				cout << "No Such Voter ID Existed. Be Sure To Enter A Correct Voter ID Or Be Sure To Be Registered As An Voter Before Voting!!\n" << endl; // Print Out Text
				voteridinput = true;
			}
			else // Voter ID Exists
			{
				if (checkvoterstatus(voter, voterid) == "N") // Check If Voter Status is "N"
				{
					candidateinfovoteselection(candidate, checkvoterdivision(voter, voterid)); // Call candidateinfovoteselection function

					while (!candidateidinput) // While True
					{
						cout << "Please Enter Your Candidate ID: " << endl; // Request User For Candidate ID Input
						getline(cin, candidateid);

						system("cls"); // Clear Console

						vector <string> existingcandidateid = usedcandidateidbasedondivision(candidate, checkvoterdivision(voter, voterid)); // Set usedcandidatebasedondivision function as vector

						if (find(existingcandidateid.begin(), existingcandidateid.end(), candidateid) != existingcandidateid.end()) // If Candidate ID Matches With Existing ID
						{
							for (int j = 0; j < size(usedcandidateidbasedondivision(candidate, checkvoterdivision(voter, voterid))); j++) // Loop To Get Candidate ID Index
							{
								if (usedcandidateidbasedondivision(candidate, checkvoterdivision(voter, voterid))[j] == candidateid) // If candidateid == usedcandidateidbasedondivision fucntion
								{
									votecountupdate(candidate, to_string(candidatevotebasedonid(candidate, candidateid) + 1), candidateid); // call votecountupdate Function To Increment Vote By 1
									voterstatusupdate(voter, voterid); // Update Voter Status To Y

									cout << "You Have Voted For " << candidateid << "\n" << endl; // Display Text

									candidateidinput = true;
									voteridinput = true;
									break;
								}
							}
						}
						else // If Candidate ID Does Not Match
						{
							cout << "This Candidate Does Not Exist!!\n" << endl; // Display Text
							candidateidinput = true;
							voteridinput = true;
							break;
						}
					}
				}
				else // If Voter Status is "Y"
				{
					cout << "You Have Have Already Voted\n" << endl; // Display Text
					voteridinput = true;
				}
			}
		}
		else // If Wrong Input
		{
			cout << "You Have Entered A Wrong Input, Please Try Again!!\n" << endl; // Display Text
			voteridinput = false;
		}
	}
}

// Select Candidate Vote Count
static int selectcandidatedata(const char* s, string candidateid)
{
	sqlite3* DB;
	sqlite3_stmt* stmt;

	char* messageError;

	int votecount;

	string sql = "SELECT Vote FROM CandidateTable WHERE CandidateID = '" + candidateid + "'; "; // Specify Which Data To Select

	int rc = sqlite3_open(s, &DB); // Opens The Database

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	rc = sqlite3_exec(DB, sql.c_str(), callcandidatedata, NULL, &messageError);

	sqlite3_bind_int(stmt, 1, 16); // Bind Data To Integer Value

	for (;;)
	{
		int rc = sqlite3_step(stmt); // Set rc to The Step Of The Data (How Many Rows In The Data)

		if (rc == SQLITE_DONE) // If Reached The End Of The Data
		{
			break;
		}

		votecount = sqlite3_column_int(stmt, 0) + 1; // Assign Data To maxparty
	}

	sqlite3_finalize(stmt); // Finalize the Data Reading

	if (exit != SQLITE_OK) // If There Is An Error When Selecting Data
	{
		cerr << "Error in Selecting Data!\n" << endl; // Display Error Message
		sqlite3_free(&messageError); // Remove The Error Message
	}
	else // If There Is No Error When Selecting Data
		cout << "Data Selected Successfully\n" << endl; // Display A Text Message

	return votecount;
}

// Menu For Option 4
void ViewResultMenu()
{
	bool RQuit = false;
	bool* QuitResult = &RQuit;

	while (!*QuitResult) // While Still Viewing The Menu
	{
		string ResultSelection;
		string	ResultArray[] = { "Viewing All Results From All Divisions\n", "Which Division's Information Would You Like To View?\n[1] Division 1\n[2] Division 2\n[3] Division 3\n[4] Division 4",
			"Going Back To Main Menu\n", "You Have Entered An Invalid Option, Please Try Again!\n" };

		cout << "Please Select Your Result Viewing Options.\n[1] View All Results From All Divisions\n[2] View Results In A Specific Division\n[3] Back To Main Menu\n" << endl;
		getline(cin, ResultSelection);

		system("cls");

		ResultSelectionMenu(QuitResult, ResultSelection, ResultArray);
	}
}

// Menu For 2 Different Result Viewing Options In Option 4
void ResultSelectionMenu(bool* Quit, string Selection, string CaseArray[])
{
	const char* division = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	bool divisionselection = false;
	bool partyselection = false;
	string choosendivision;
	string choosenparty;

	if (Selection == "1") // If User Choose Option 1
	{
		cout << CaseArray[0] << endl; // Display First Index From CaseArray

		for (int i = 1; i < 5; i++) // For Every Index
		{
			minmaxvotealldivision(division, i, maxvotecount(division, i), minvotecount(division, i)); // Call minmaxvotealldivision Function
		}
		*Quit = true;
	}
	else if (Selection == "2") // If User Choose Option 1
	{
		while (!divisionselection)
		{
			cout << CaseArray[1] << endl; // Display Second Index From CaseArray
			cin >> choosendivision;

			system("cls"); // Clear Console

			if (choosendivision == "1") // If User Choose Division 1
			{
				cout << "You Have Chosen Division 1\n" << endl; // Display Text
				minmaxvotealldivision(division, stoi(choosendivision), maxvotecount(division, stoi(choosendivision)), minvotecount(division, stoi(choosendivision))); // Call minmaxvotealldivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "2") // If User Choose Division 2
			{
				cout << "You Have Chosen Division 2\n" << endl; // Display Text
				minmaxvotealldivision(division, stoi(choosendivision), maxvotecount(division, stoi(choosendivision)), minvotecount(division, stoi(choosendivision))); // Call minmaxvotealldivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "3") // If User Choose Division 3
			{
				cout << "You Have Chosen Division 3\n" << endl; // Display Text
				minmaxvotealldivision(division, stoi(choosendivision), maxvotecount(division, stoi(choosendivision)), minvotecount(division, stoi(choosendivision))); // Call minmaxvotealldivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "4") // If User Choose Division 4
			{
				cout << "You Have Chosen Division 4\n" << endl;
				minmaxvotealldivision(division, stoi(choosendivision), maxvotecount(division, stoi(choosendivision)), minvotecount(division, stoi(choosendivision))); // Call minmaxvotealldivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else // If User Choose Wrong Option
			{
				cout << "Invalid Option!\n" << endl;
				divisionselection = false;
			}
		}
		*Quit = true;
	}
	else if (Selection == "3") // If User Choose Option 1
	{
		cout << CaseArray[2] << endl; // Display Third Index From CaseArray
		*Quit = true;
	}
	else // If User Choose Wrong Option
	{
		cout << CaseArray[3] << endl; // Display Fourth Index From CaseArray
		*Quit = false;
	}
}

// Menu For Option 1
void ViewingCandidateMenu()
{
	bool VQuit = false;
	bool* QuitViewing = &VQuit;

	while (!*QuitViewing)
	{
		string ViewingSelection;
		string	ViewingArray[] = { "Viewing All Candidates In All Divisions\n", "Which Division Would You Like To View ?\n[1] Division 1\n[2] Division 2\n[3] Division 3\n[4] Division 4\n",
			"Which Party Would You Like To View?\n[1] Einstein\n[2] Tesla\n[3] Mozart\n", "Going Back To Main Menu\n", "You Have Entered An Invalid Option, Please Try Again!\n" };

		cout << "Please select the following options by typing in the number.\n[1] View All Candidates In All Divisions\n"
			"[2] View The Candidates In Specific Division\n[3] View The Candidates Based On The Party\n[4] Back To Main Menu\n" << endl;
		getline(cin, ViewingSelection);

		system("cls");

		ViewingSelectionMenu(QuitViewing, ViewingSelection, ViewingArray); // Call ViewingSelectionMenu Function
	}
}

// Main Menu
void VoterMainMenu()
{
	bool MNQuit = false;
	bool* Quit = &MNQuit;

	while (!*Quit)
	{
		string Selection;
		string	MainMenuArray[] = { "Viewing Candidates\n", "Registering as Voter\n", "Voting for Candidate\n", "Viewing voting results and summary\n",
			"Exiting Program\n", "You Have Entered An Invalid Option, Please Try Again!\n" };

		cout << "Please select the following options by typing in the number.\n[1] View Candidates\n[2] Register Voter\n"
			"[3] Vote\n[4] View voting results and summary\n[5] Exit\n" << endl;
		getline(cin, Selection);

		system("cls"); // Clear Console

		MainSelectionMenu(Quit, Selection, MainMenuArray); // Call MainSelecionMenu Function
	}
}

// Menu For 3 Different Viewing Candidate Options In Option 1
void ViewingSelectionMenu(bool* Quit, string Selection, string CaseArray[])
{
	const char* division = R"(C:\\StoreData\\Division.db)"; // Calls The Database

	bool divisionselection = false;
	bool partyselection = false;
	string choosendivision;
	string choosenparty;

	if (Selection == "1") // If User Choose Option 1
	{
		cout << CaseArray[0] << endl; // Display First Index Text From CaseArray
		viewcandidateinfo(); // Call viewcandidateinfo Function
		*Quit = true;
	}
	else if (Selection == "2") // If User Choose Option 2
	{
		while (!divisionselection)
		{
			cout << CaseArray[1] << endl; // Display Second Index Text From CaseArray
			cin >> choosendivision;

			system("cls"); // Clear Console

			if (choosendivision == "1") // If User Choose Division 1
			{
				cout << "You Have Chosen Division 1\n" << endl; // Display Text
				viewdivision(choosendivision); // Call viewdivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "2") // If User Choose Division 2
			{
				cout << "You Have Chosen Division 2\n" << endl; // Display Text
				viewdivision(choosendivision); // Call viewdivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "3") // If User Choose Division 3
			{
				cout << "You Have Chosen Division 3\n" << endl; // Display Text
				viewdivision(choosendivision); // Call viewdivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else if (choosendivision == "4") // If User Choose Division 4
			{
				cout << "You Have Chosen Division 4\n" << endl; // Display Text
				viewdivision(choosendivision); // Call viewdivision Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				divisionselection = true;
			}
			else // If User Choose Wrong Option
			{
				cout << "Invalid Option! Try Again\n" << endl; // Display Text
				divisionselection = false;
			}
		}
		*Quit = true;
	}
	else if (Selection == "3") // If User Choose Option 3
	{
		while (!partyselection)
		{
			cout << CaseArray[2] << endl; // Display Third Index Text From CaseArray
			cin >> choosenparty;

			system("cls"); // Clear Console

			if (choosenparty == "1") // If User Choose Party Einstein
			{
				cout << "Party Einstein Selected\n" << endl; // Display Text
				viewparty(choosenparty); // Call viewparty Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				partyselection = true;
			}
			else if (choosenparty == "2") // If User Choose Party Tesla
			{
				cout << "Party Tesla Selected\n" << endl; // Display Text
				viewparty(choosenparty); // Call viewparty Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				partyselection = true;
			}
			else if (choosenparty == "3") // If User Choose Party Mozart
			{
				cout << "Party Mozart Selected\n" << endl; // Display Text
				viewparty(choosenparty); // Call viewparty Function

				cin.clear(); // Clears Input Buffer
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores Input After New Line

				partyselection = true;
			}
			else // If User Choose Wrong Option
			{
				cout << "Invalid Option! Try Again\n" << endl; // Display Text
				partyselection = false;
			}
		}
		*Quit = true;
	}
	else if (Selection == "4") // If User Choose Option 4
	{
		cout << CaseArray[3] << endl; // Display Fourth Index Text From CaseArray
		*Quit = true;
	}
	else // If User Choose Wrong Option
	{
		cout << CaseArray[4] << endl; // Display Fifth Index Text From CaseArray
		*Quit = false;
	}
}

// Menu For 5 Different Option
void MainSelectionMenu(bool* Quit, string Selection, string CaseArray[])
{
	if (Selection == "1") // If User Choose Option 1
	{
		cout << CaseArray[0] << endl; // Display First Index Text From CaseArray
		ViewingCandidateMenu(); // Call ViewingCandidateMenu Function
	}
	else if (Selection == "2") // If User Choose Option 2
	{
		cout << CaseArray[1] << endl; // Display Second Index Text From CaseArray
		VoterDatabase(); // Call voterdatabase Function
	}
	else if (Selection == "3") // If User Choose Option 3
	{
		cout << CaseArray[2] << endl; // Display Third Index Text From CaseArray
		VoteCandidate(); // Call votecandidate Function
	}
	else if (Selection == "4") // If User Choose Option 4
	{
		cout << CaseArray[3] << endl; // Display Fourth Index Text From CaseArray
		ViewResultMenu(); // Call ViewResultMenu Function
	}
	else if (Selection == "5") // If User Choose Option 5
	{
		cout << CaseArray[4] << endl; // Display Fifth Index Text From CaseArray
		*Quit = true;
	}
	else // If User Choose Wrong Option
	{
		cout << CaseArray[5] << endl; // Display Sixth Index Text From CaseArray
	}

}

int main()
{
	VoterMainMenu();

	return 0;
}