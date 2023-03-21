// Program to implement a very simplified list of tweets 
// from a single simulated Twitter account
// Tweets can be added, deleted, and liked

#include <iostream>
using namespace std;

const int MSGSIZE = 100;	// Maximum size for a tweet message
const int CAPACITY = 10;	// Maximum number of tweets

// Structure used to define a tweet entry
struct Tweet
{
	int id;
	char msg[MSGSIZE];
	int likes;
};


/*
* Prints out an entire timeline to the screen
* timeline = timeline of tweets to be printed
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
*/
void displayTimeline(const Tweet timeline[], int usedSize, int selected);


/*
* Edits currently selected tweet
* with a new message entered by the user.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doEditTweet(Tweet timeline[], int usedSize, int selected);


/*
* Adds a like to the currently selected tweet.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doLikeTweet(Tweet timeline[], int usedSize, int selected);


/*
* Deleted currently selected tweet.
* timeline = timeline in from which the entry is to be deleted
* usedSize = number of tweets in the timeline
* If 'selected' represents a valid array position:
*   the selected tweet will be deleted
*   usedSize will be updated to reflect the updated number of tweets in the timeline
*   selected will be updated to -1
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected);


/*
* If there is room in the timeline for new tweets, then this gets
* a new tweet from the user and adds it to the timeline.
* timeline = timeline in which the tweet is to be added
* usedSize = number of tweets in the timeline
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int doAddTweet(Tweet timeline[], int& usedSize);


/*
* Adds a new tweet to the list
* timeline = timeline in which the entry is to be added
* usedSize = number of tweets in the timeline
* message = tweet message to be added
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int addTweet(Tweet timeline[], int& usedSize, const char message[]);


/*
* Returns the next available ID number
* timeline = timeline in which to find the highest ID
* usedSize = number of tweets in the timeline
* If timeline is empty, returns 100;
* otherwise, returns 1 + highest ID number in the timeline
*/
int getNextId(Tweet timeline[], int usedSize);


/*
* Gets a tweet id from the user. Searches a timeline to try
* to find the specified tweet by its id.
* timeline = timeline to be searched
* usedSize = number of tweets in the timeline
* If the tweet is found, the position number of where the tweet
* is stored in the timeline will be returned.  If the tweet is
* not found, a 'not found message' will be printed, and
* the value -1 will be returned.
* If timeline is empty, an 'empty' message will be printed, and
* the value -1 will be returned.
*/
int selectTweet(const Tweet timeline[], int usedSize);



int main()
{
	Tweet timeline[CAPACITY];	// Twitter timeline
	int choice;					// User's menu choice
	int usedSize = 0;			// Num of tweets in the timeline
	int selected = -1;			// Currently selected tweet
	int tmp;					// Temporary variable

	// Add some starter entries for testing purposes
	selected = addTweet(timeline, usedSize, "Where do they get the seeds to plant seedless watermelons?");
	selected = addTweet(timeline, usedSize, "Waffles are just pancakes with convenient boxes to hold your syrup.");
	selected = addTweet(timeline, usedSize, "Last night I even struck up a conversation with a spider. Turns out he's a web designer.");

	do
	{
		cout << "1. Display Timeline\n";
		cout << "2. Select Tweet\n";
		cout << "3. Add New Tweet\n";
		cout << "4. Edit Selected Tweet\n";
		cout << "5. Like Selected Tweet\n";
		cout << "6. Delete Tweet\n";
		cout << "7. Exit\n";
		cout << endl;
		cout << "Select: ";

		// Get the numeric entry from the menu
		cin >> choice;

		// Corrects issues where user might enter a non-integer value
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Select: ";
			cin >> choice;
		}

		// Makes the 'enter' key that was pressed after the numeric entry be ignored
		// Should be used after getting any numeric input from the keyboard
		cin.ignore();

		switch (choice)
		{
		case 1:
			displayTimeline(timeline, usedSize, selected);
			break;
		case 2:
			tmp = selectTweet(timeline, usedSize);
			// if selected tweet exists, update selected variable;
			// otherwise leave it unchanged
			if (tmp > -1)
				selected = tmp;
			break;
		case 3:
			tmp = doAddTweet(timeline, usedSize);
			// if tweet was added, make it be the selected tweet;
			// otherwise leave it unchanged
			if (tmp > -1)
			{
				selected = tmp;
			}
			break;
		case 4:
			doEditTweet(timeline, usedSize, selected);
			break;
		case 5:
			doLikeTweet(timeline, usedSize, selected);
			break;
		case 6:
			doDeleteTweet(timeline, usedSize, selected);
			break;
		}

	} while (choice != 7);

	return 0;
}


//This function is responsible for adding tweets entered by the user. 
int doAddTweet(Tweet timeline[], int& usedSize)
{
	//The program checks to make sure there is still room in the array and if not then the program tells the user that they have reached the limit and returns an error.
	if (usedSize >= CAPACITY)
	{
		cout << "\nERROR: Timeline is full.\n";
		return -1;
	}
	else
	{
		//If there is still room left, the program will prompt the user to enter a new tweet and automatically assign an ID.
		cout << "\nEnter tweet: \n";
		cin.getline(timeline[usedSize].msg, MSGSIZE);
		timeline[usedSize].likes = 0;
		timeline[usedSize].id = getNextId(timeline, usedSize);
		usedSize++;
		cout << endl;

	}	
}

//This function enables the user to edit a tweet they have previously written. The user enters the ID of the tweet they wish to edit and are prompted to rewrite the tweet.
void doEditTweet(Tweet timeline[], int usedSize, int selected)
{
	int pos = 0;
	//This checks to make sure the ID is valid and prompts the user to edit if it is and loops back to prompt the user to enter a new ID.
	if (selected >= 100 && selected < getNextId(timeline, usedSize))
	{
		for (int i = 0; i < CAPACITY; i++)
		{
			if (timeline[i].id == selected)
			{
				pos = i;
			}
		}

		cin.getline(timeline[pos].msg, MSGSIZE);
	}
	else
	{
		//If the ID does not exist or is invalid, then the user is notified.
		cout << "\nERROR: No tweet is selected.\n";
	}
}

//This function incriments a like counter every time it is run.
void doLikeTweet(Tweet timeline[], int usedSize, int selected)
{
	int pos = 0;
	//This checks to make sure the ID is valid and prompts the user to edit if it is and loops back to prompt the user to enter a new ID.
	if (selected >= 100 && selected < getNextId(timeline, usedSize))
	{
		for (int i = 0; i < CAPACITY; i++)
		{
			if (timeline[i].id == selected)
			{
				pos = i;
			}
		}
		//Incriments the like counter of the selected tweet.
		timeline[pos].likes++;
	}
	else
	{
		//If the ID does not exist or is invalid, then the user is notified.
		cout << "\nERROR: No tweet is selected.\n";
	}
}


void displayTimeline(const Tweet timeline[], int usedSize, int selected)
{
	//Format of display timeline for tweets
	printf("%s\n", "\nsel\tID\tLikes\tTweet\n");

	//Iterate through all of the tweets in the timeline array 
	for (int i = 0; i < usedSize; i++)
	{

		if (timeline[i].id > 0)
		{
			if (timeline[i].id == selected) //If the ID is selected, print an arrow
			{
				printf("-->");
			}

			//For all tweets, print the ID, likes, and the message of the tweet
			printf("\t%d", timeline[i].id);
			printf("\t%d", timeline[i].likes);
			printf("\t%s\n", timeline[i].msg);
		}

	}
	cout << "\n" << endl;
}

//Initializes some hard coded tweets for testing purposes.
int addTweet(Tweet timeline[], int& usedSize, const char message[])
{
	//If the number of tweets is more than what's acceptable (10), then don't add any
	if (usedSize >= CAPACITY)
	{
		return -1;
	}
	else // Otherwise, add the hard-coded messages
	{
		for (int i = 0; i < MSGSIZE; i++) //Iterate through the message character by character and add to the message part of respective tweet
		{
			timeline[usedSize].msg[i] = message[i];
		}
		timeline[usedSize].likes = 0; //Set likes to 0
		timeline[usedSize].id = getNextId(timeline, usedSize); //Add ID using the getNextId function
		usedSize++; //Add "1" to the usedSize variable for one more variable has been added 
	}
}


int getNextId(Tweet timeline[], int usedSize)
{
	int nextID = 0;

	//If there are no tweets in the array, return 100 (which equals to first position in the array)
	if (usedSize <= 0)
	{
		return 100;
	}
	for (int i = 0; i < usedSize; i++) //Iterate through all of the tweets and add one to the next ID for the next tweet
	{
		nextID = timeline[i].id + 1;
	}
	return nextID;
}


void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected)
{
	//Variable for storing the position of the tweet to be deleted
	int pos = 0;

	//If the selected tweet has a valid ID that is in between 100 and the highest ID possible,
	if (selected >= 100 && selected < getNextId(timeline, usedSize))
	{
		//Then look for the selected Tweet's posiiton in the array
		for (int i = 0; i < CAPACITY; i++)
		{
			if (timeline[i].id == selected)
			{
				pos = i;
			}
		}

		//Afterwards, move all of the positions of the tweet info. forward 
		//so that the selected tweet will no longer be included
		for (int i = pos; i < CAPACITY; i++)
		{
			timeline[i] = timeline[i + 1];
		}
		//Reduce size by 1 and set the selected tweet to none
		usedSize--;
		selected = -1;
		cout << "\nTweet was deleted.\n\n";
	}
	else //If the tweet's ID is invalid, then return this statement and ask for an ID again
	{
		cout << "\nERROR: No tweet is selected.\n\n";
	}
}


int selectTweet(const Tweet timeline[], int usedSize)
{
	int userSelect = 0;

	if (usedSize == 0)
	{
		cout << "\nTimeline is empty\n\n";
		return -1;
	}

	// User will select the tweet they want
	cout << "Enter ID: ";
	cin >> userSelect;

	//If the user enters an invalid ID, then continue to ask for a valid one
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Enter ID: ";
		cin >> userSelect;
	}

	//Iterate through the tweets and if the ID selected matches, then return the ID of the selected tweet
	for (int i = 0; i < usedSize; i++)
	{
		if (timeline[i].id == userSelect)
		{
			return userSelect;
		}
	}
	
	//If ID is invalid, then print that the ID was not found and return a -1 (to signify that it wasn't a valid ID)
	cout << "\nID was not found.\n\n";
	return -1;
}