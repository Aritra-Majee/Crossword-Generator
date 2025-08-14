#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <bits/stdc++.h>
#include <iomanip>
#include <unistd.h>

using namespace std;
using namespace chrono;

const int GRID_SIZE =27;
const int numWords = 6;


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN    "\033[1;36m"
#define BOLD_WHITE   "\033[1;37m"

int HintCounter=0;

const int COLOR_BLACK = 30;
const int COLOR_RED = 31;
const int COLOR_GREEN = 32;
const int COLOR_YELLOW = 33;
const int COLOR_BLUE = 34;
const int COLOR_MAGENTA = 35;
const int COLOR_CYAN = 36;
const int COLOR_WHITE = 37;
const int COLOR_BRIGHT = 1;


void setColor(string colorCode)
{
        if (colorCode == "RED") cout << RED;
        else if (colorCode == "GREEN") cout << GREEN;
        else if (colorCode == "YELLOW") cout << YELLOW;
        else if (colorCode == "BLUE") cout << BLUE;
        else if (colorCode == "MAGENTA") cout << MAGENTA;
        else if (colorCode == "CYAN") cout << CYAN;
        else if (colorCode == "WHITE") cout << WHITE;
        else if (colorCode == "BOLD_RED") cout << BOLD_RED;
        else if (colorCode == "BOLD_GREEN") cout << BOLD_GREEN;
        else if (colorCode == "BOLD_YELLOW") cout << BOLD_YELLOW;
        else if (colorCode == "BOLD_BLUE") cout << BOLD_BLUE;
        else if (colorCode == "BOLD_MAGENTA") cout << BOLD_MAGENTA;
        else if (colorCode == "BOLD_CYAN") cout << BOLD_CYAN;
        else if (colorCode == "BOLD_WHITE") cout << BOLD_WHITE;
        else cout << RESET; // Default to reset if unknown
}

void resetColor()
{
        cout << RESET;
}


class User {
public:
    string name[101];
    float time_u[101];
    int attempts[101];  // New array to store attempts
    int size_u;

    User() {
        size_u = 0;
        time_u[0] = -1;  // Sentinel value
    }

    void insert_min_user(float value, string str, int att) {
        size_u = size_u + 1;
        int i = size_u;
        name[i] = str;
        time_u[i] = value;
        attempts[i] = att;  // Store attempts

        // Heapify up
        while(i > 1) {
            int parent = i/2;
            if(time_u[parent] > time_u[i]) {
                // Swap all three values
                swap(time_u[parent], time_u[i]);
                swap(name[parent], name[i]);
                swap(attempts[parent], attempts[i]);
                i = parent;
            }
            else {
                break;
            }
        }
    }

    void heapify_min(int index, int n) {
        int smallest = index;
        int left = 2 * index;
        int right = 2 * index + 1;

        if(left <= n && time_u[left] < time_u[smallest]) {
            smallest = left;
        }
        if(right <= n && time_u[right] < time_u[smallest]) {
            smallest = right;
        }

        if(smallest != index) {
            // Swap all three values
            swap(time_u[index], time_u[smallest]);
            swap(name[index], name[smallest]);
            swap(attempts[index], attempts[smallest]);
            heapify_min(smallest, n);
        }
    }

    void user_min_sort(int n) {
        while(n > 1) {
            // Swap root with last element
            swap(time_u[1], time_u[n]);
            swap(name[1], name[n]);
            swap(attempts[1], attempts[n]);
            n--;
            heapify_min(1, n);
        }
    }

    void print_score() {
        setColor("BOLD_BLUE");
        cout << "\n\n\t\tSCORE CARD\t\t\n\n";
        cout << "Rank\tName\t\tTime(s)\tAttempts\n";  // Updated header

        for(int i = 1; i <= size_u; i++) {
            cout << i << "\t" << name[i] << "\t\t"
                 << fixed << setprecision(2) << time_u[i]
                 << "\t" << attempts[i] << endl;
        }
        cout << endl;
        resetColor();
    }
};
class Node
{
	public:
		int key;
		string Word;
		string Hint;
		Node *next;
		bool used;
		int index;
		Node(int k,string word,string hint,int ind)
		{
			key=k;
			Word=word;
			Hint=hint;
			next=NULL;
			used=false;
			index=ind;
		}
		Node(int k,string word,string hint)
		{
			key=k;
			Word=word;
			Hint=hint;
			next=NULL;
			used=false;
			index=-1;
		}
		Node()
		{
			next=NULL;
			key=0;
			Word="";
			Hint="";
			used=false;
			index=-1;
		}
};

class HashMap
{
	private:
		Node **htable;
		int size;
	public:
		HashMap(int size)
		{
			this->size=size;
			htable=new Node*[size];
			for (int i=0;i<size;i++)
				htable[i] = NULL;
		}
        int generateKey(string str) {
            int key = 0;
            const int prime = 31;
            for (int i = 0; i < str.length(); i++) {
                key = (key * prime + tolower(str[i])) % INT_MAX;
            }
            return key;
        }

        int HashFunc(string str) {
            return tolower(str[0]) - 'a';
        }
		void Insert(string word, string hint, int ind = -1)
{
    int key = generateKey(word);
    Node* newNode;

    if (ind == -1)
        newNode = new Node(key, word, hint);
    else
        newNode = new Node(key, word, hint, ind);

    int i = HashFunc(word);
    if (htable[i] == NULL)
    {
        htable[i] = newNode;
    }
    else
    {
        Node* ptr = htable[i];
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = newNode;
    }
}

		int SearchWithIndex(string str)
		{
			int i=HashFunc(str);
			int k=generateKey(str);
			Node *ptr=htable[i];
			int val=-1;
			while(ptr!=NULL)
			{
				if(k==ptr->key)
				{
					val=ptr->index;
				}
				ptr=ptr->next;
			}
			ptr=NULL;
			return val;

		}
        bool Search(string str) {
            int i = HashFunc(str);
            int k = generateKey(str);
            Node* ptr = htable[i];

            while (ptr != NULL) {
                if (k == ptr->key && ptr->Word == str) {
                    return true;
                }
                ptr = ptr->next;
            }
            return false;
        }
		Node* getNewWord(char c)
		{
			int i=c-97;
			Node *ptr=htable[i];
			string val="";
			int count=0;
			while(ptr!=NULL)
			{
				count++;
				ptr=ptr->next;
			}
			ptr=htable[i];
			rep:
			int random=rand()%count;
			for(int i=0;i<random;i++)
			{
				ptr=ptr->next;
			}
			if(ptr->used==true)
			{
				goto rep;
			}
			Node *temp=ptr;
			ptr->used=true;
			return temp;
		}
	void clearTable()
        {
        for(int i=0;i<size;++i)
        {
            Node *current=htable[i];
            while(current!=NULL)
            {
                Node* next=current->next;
                delete current;
                current=next;
            }
            htable[i]=NULL;
        }
    }
    void printHashMapWithHints(HashMap& h) {
        setColor("BOLD_CYAN");
        cout << "\nHASH TABLE CONTENTS (Word -> Hint):\n";
        resetColor();

        for (int i = 0; i < 26; ++i) {  // Since your hash table size is 26 (a-z)
            Node* current = h.htable[i];
            if (current != nullptr) {
                setColor("BOLD_YELLOW");
                cout << "Bucket " << char('a' + i) << ":\n";
                resetColor();

                while (current != nullptr) {
                    cout << "  " << current->Word << " -> " << current->Hint << endl;
                    current = current->next;
                }
            }
        }
        cout << endl;
    }

};


void printGrid(char grid[GRID_SIZE][GRID_SIZE])
{
    for (int i=0;i<GRID_SIZE;++i)
	{
        for (int j=0;j<GRID_SIZE;++j)
	{
                if(grid[i][j]>='A' && grid[i][j]<='Z' || grid[i][j]>='a' && grid[i][j]<='z')
                {
            	        setColor("BOLD_GREEN");
            	        cout<<grid[i][j]<< " ";
            	        resetColor();

	        }
	        else
                {
			cout<<grid[i][j]<< " ";
		}

        }
        cout << endl;
    }
    cout << endl;
}

void CopyGrid(char grid[GRID_SIZE][GRID_SIZE],char grid2[GRID_SIZE][GRID_SIZE])
{
	for (int i=0;i<GRID_SIZE;++i)
	{
                for (int j=0;j<GRID_SIZE;++j)
	        {
                        if(grid[i][j]>='A' && grid[i][j]<='Z' || grid[i][j]>='a' && grid[i][j]<='z')
                        {
                    	        grid2[i][j]='-';
	                }
                }
    }
}

bool CompareGrids(char grid[GRID_SIZE][GRID_SIZE],char grid2[GRID_SIZE][GRID_SIZE])
{
	bool flag=true;
	for (int i=0; i<GRID_SIZE;++i)
	{
                for (int j=0;j< GRID_SIZE;++j)
	        {
                        if(grid[i][j]!=grid2[i][j])
                        {
                    	        flag=false;
	        	}
                }
        }
	return flag;
}

bool isValid(int row,int col)
{
    return (row>=0 && row<GRID_SIZE && col>=0 && col<GRID_SIZE);
}
bool HorizontalUpdate(char grid[][GRID_SIZE],string str,char grid2[][GRID_SIZE],int row,int col,int index)
{
    if(index==str.length()-2)
	{
		if(grid2[row][col]==grid[row][col] && grid2[row][col+1]==grid[row][col+1])
		{
			return false;
		}
                grid2[row][col]=grid[row][col];
                grid2[row][col+1]=grid[row][col+1];
                resetColor();
                return true;
    }
	if(isValid(row,col) && grid[row][col]==str[index])
	{
                char temp=grid2[row][col];
                setColor("BOLD_GREEN");
                grid2[row][col]=grid[row][col];
                if(HorizontalUpdate(grid,str,grid2,row,col+1,index+1))
	            {
                    return true;
                }
                resetColor();
	        grid2[row][col]=temp;
        }
    return false;
}
bool VerticalUpdate(char grid[][GRID_SIZE],string str,char grid2[][GRID_SIZE],int row,int col,int index)
{
        if (index==str.length()-2)
	{
		if(grid2[row][col]==grid[row][col] && grid2[row+1][col]==grid[row+1][col])
		{
			return false;
		}
		grid2[row][col]=grid[row][col];
		grid2[row+1][col]=grid[row+1][col];
		resetColor();
    	        return true;
	}
        if(isValid(row,col) && grid[row][col]==str[index])
	{
		char temp=grid2[row][col];
		setColor("BOLD_GREEN");
                grid2[row][col]=grid[row][col];
                if(VerticalUpdate(grid,str,grid2,row+1,col,index+1))
		{
                return true;
                }
                resetColor();
    	        grid2[row][col]=temp;
	}
        return false;
}
bool HorizontalVerticalUpdate(char grid[][GRID_SIZE],string str,char grid2[][GRID_SIZE],int row, int col,int index)
{
        if(HorizontalUpdate(grid,str,grid2,row,col,index))
        {
            return true;
        }
        if(VerticalUpdate(grid,str,grid2,row,col,index))
        {
            return true;
        }
        return false;
}

void updateDisplayGrid(char grid[GRID_SIZE][GRID_SIZE],char grid2[GRID_SIZE][GRID_SIZE],string str)
{
    bool found=false;
    for (int i=0;i<GRID_SIZE;++i)
	{
        for (int j=0;j<GRID_SIZE;++j)
	{
                if(HorizontalVerticalUpdate(grid,str,grid2,i,j,0))
		{
                        found=true;
                        break;
                }
        }
        if(found)
	{
            break;
        }
    }
}

void EmptyHints(string *hints2,int n,int *index)
{
	for(int i=0;i<n;i++)
	{
		hints2[i]="";
		index[i]=-1;
	}
	HintCounter=0;
}

void HintsDisplayer(string *hints2,int *index)
{
	setColor("BOLD_BLUE");
	cout<<"HINTS : \n";
	resetColor();
	for(int i=0;i<HintCounter;i++)
	{
		if(index[i]!=-1)
		{
			setColor("BLUE");
			cout<<i+1<<". "<<hints2[i]<<endl;
			resetColor();
		}
		else
		{
			cout<<i+1<<". "<<hints2[i]<<endl;
		}

	}

}

void generateCrossword(char grid[GRID_SIZE][GRID_SIZE],string *words,string *hints,int n,HashMap h,HashMap h2,string *hints2)
{

	srand(time(0));

	bool isHorizontal=false;
	int randomIndex = rand() % n;

	string currentWord = words[randomIndex];
	hints2[HintCounter] = hints[randomIndex];

	h2.Insert(currentWord,hints[randomIndex],HintCounter);
        HintCounter++;
	int startRow=0;
        int startCol=0;
        char randomChar=currentWord[(rand() % (currentWord.length()-1))+1];
        int iIndex;
        int jIndex;
	for (int i=0;i<currentWord.length();++i)
	{
                if(currentWord[i]==randomChar)
                {
                    iIndex=0;
                    jIndex=startCol+i;
                }
                grid[startRow][startCol+i]=currentWord[i];
        }

    for (int wordCount=1; wordCount<numWords;++wordCount) // Can apply Backtracking
	{
		Node *temp=h.getNewWord(randomChar);
		currentWord=temp->Word;

		hints2[HintCounter]=temp->Hint;
    	        h2.Insert(currentWord,temp->Hint,HintCounter);

		HintCounter++;

		startRow =iIndex;
                startCol=jIndex;
                randomChar=currentWord[(rand() % (currentWord.length()-1))+1];
                for (int i=0; i < currentWord.length(); ++i)
                {
                        if(isHorizontal)
                        {
                            if(i==0)
                            {
                                jIndex++;
                                continue;
                            }
                            if(currentWord[i]==randomChar)
                            {
                                jIndex=startCol + i;
                            }
                            grid[startRow][startCol+i]=currentWord[i];
                        }
                        else
                        {
                            if(i==0)
                            {
                                iIndex++;
                                continue;
                            }
                            if(currentWord[i]==randomChar)
                            {
                                iIndex=startRow+i;
                            }
                            grid[startRow+i][startCol]=currentWord[i];
                        }

                }
        if(isHorizontal)
        {
            isHorizontal=false;
        }
        else
        {
            isHorizontal=true;
        }
    }
    printGrid(grid);
}


int main()
{

    string words[] = {
        "apple", "banana", "cat", "dog", "elephant",
        "fish", "grape", "hat", "ice", "jelly",
        "kite", "lion", "monkey", "nest", "orange",
        "pen", "queen", "rain", "sun", "tree",
        "umbrella", "van", "whale", "xray", "yarn",
        "zebra", "book", "car", "duck", "egg",
        "frog", "goat", "hand", "ink", "jam",
        "key", "lamp", "mouse", "nose", "owl",
        "pig", "quilt", "ring", "sock", "train",
        "unicorn", "vase", "window", "box", "yard",
        "zip", "ant", "ball", "cake", "drum",
        "ear", "fan", "gift", "hill", "iron",
        "jar", "king", "leaf", "milk", "net",
        "octopus", "pan", "quiz", "rose", "star",
        "toy", "under", "village", "wolf", "xylophone",
        "yoyo", "zoo", "alligator", "bread", "cloud",
        "dance", "engine", "flag", "glove", "house",
        "idea", "jacket", "kitten", "ladder", "moon",
        "needle", "ocean", "pencil", "quiet", "river",
        "ship", "tiger", "use", "violin", "water"
    };
    
    string hints[] = {
        "A red or green fruit that keeps the doctor away.",
        "A long yellow fruit.",
        "A small pet that says meow.",
        "A loyal pet that barks.",
        "A huge animal with a trunk.",
        "A small animal that swims in water.",
        "A small purple fruit in bunches.",
        "You wear it on your head.",
        "Frozen water.",
        "A sweet food made from fruit and sugar.",
        "It flies in the sky on a string.",
        "The king of the jungle.",
        "A playful animal that loves bananas.",
        "A bird’s home.",
        "A juicy orange fruit.",
        "You write with it.",
        "A royal woman.",
        "Water that falls from the sky.",
        "The star that shines in the sky during the day.",
        "A tall plant with leaves.",
        "Used to stay dry in the rain.",
        "A vehicle with four wheels.",
        "A large sea animal.",
        "A picture taken of bones.",
        "Thread for knitting.",
        "An animal with black and white stripes.",
        "You read stories from this.",
        "A vehicle that moves on roads.",
        "A bird that swims and quacks.",
        "Laid by a chicken.",
        "A green amphibian that hops.",
        "An animal that gives milk.",
        "You use this to touch things.",
        "Liquid used for writing.",
        "Made from fruit, spread on bread.",
        "Opens locked doors.",
        "Used to light a room.",
        "A small rodent.",
        "You smell with this.",
        "A bird that hoots.",
        "An animal that oinks.",
        "A warm blanket made of pieces.",
        "Worn on a finger.",
        "Worn on your feet.",
        "Moves on tracks and carries people.",
        "A magical horse with a horn.",
        "Holds flowers.",
        "You can look through it.",
        "Used to pack things.",
        "Grass-covered outdoor area.",
        "Used to close a zipper.",
        "A small insect that bites.",
        "Used to play sports.",
        "A sweet baked dessert.",
        "A musical instrument you beat.",
        "You hear with this.",
        "Used to stay cool in hot weather.",
        "Something given on a birthday.",
        "A small hill or rise in land.",
        "Used to press clothes.",
        "Used to store food or pickles.",
        "A man who rules a kingdom.",
        "Falls from trees in autumn.",
        "A white drink from cows.",
        "Used to catch fish.",
        "A sea animal with 8 arms.",
        "Used to cook food on a stove.",
        "A short test.",
        "A flower with thorns.",
        "Shines at night in the sky.",
        "Something children play with.",
        "Opposite of over.",
        "A small group of houses.",
        "A wild dog that howls.",
        "An instrument you strike to play notes.",
        "A round toy that goes up and down.",
        "A place with lots of animals.",
        "A long green reptile with sharp teeth.",
        "Food made from flour and baked.",
        "White fluffy object in the sky.",
        "Moving to music.",
        "Drives machines or vehicles.",
        "Symbol of a country.",
        "Worn on your hand in winter.",
        "You live in it.",
        "A thought or plan.",
        "Worn to keep warm in winter.",
        "A baby cat.",
        "Used to climb up or down.",
        "Shines at night, orbits Earth.",
        "Used to sew things.",
        "A large body of salt water.",
        "Used to write or draw.",
        "The opposite of loud.",
        "Flows with water.",
        "A boat that travels on water.",
        "A big striped cat.",
        "To put something to work.",
        "A musical instrument with strings and a bow.",
        "Covers most of Earth and quenches thirst."
    };
    


    srand(time(0));
    string ptr;
    int choice;
    User u;

    while(1)
    {
        HintCounter = 0;
        HashMap h(26);
        HashMap h2(26);
        string hints2[numWords] = {""};
        int index[numWords];
        memset(index, -1, sizeof(index));
        char grid[GRID_SIZE][GRID_SIZE];
        char grid2[GRID_SIZE][GRID_SIZE];

        // Initialize grids
        for(int i=0; i<GRID_SIZE; ++i) {
            for(int j=0; j<GRID_SIZE; ++j) {
                grid[i][j] = ' ';
                grid2[i][j] = ' ';
            }
        }

        int n = sizeof(words)/sizeof(words[0]);
        vector<pair<string, string>> wordHintPairs;

        for(int i=0; i<n; ++i) {
            wordHintPairs.push_back({words[i], hints[i]});
        }

        for(int i=0; i<n; ++i) {
            h.Insert(wordHintPairs[i].first, wordHintPairs[i].second);
        }

        // h.printHashMapWithHints(h);

        cout << "\n\nEnter User Name: ";
        fflush(stdin);
        getline(cin, ptr);
        cout << "\n\n";

        generateCrossword(grid, words, hints, n, h, h2, hints2);
        CopyGrid(grid, grid2);
        // h2.printHashMapWithHints(h2);

        int attempts = 0;  // Initialize attempts counter
        auto start_time = high_resolution_clock::now();

        while(1) {
            printGrid(grid2);
            HintsDisplayer(hints2, index);

            if(CompareGrids(grid, grid2)) {
                break;
            }

            string str;
            cout << "\nEnter Word To Fill Crossword: ";
            cin >> str;
            attempts++;  // Increment attempt counter

            if(h2.Search(str)) {
                int ind = h2.SearchWithIndex(str);
                if(ind != -1) {
                    index[ind] = ind;
                }
                updateDisplayGrid(grid, grid2, str);
            }
            else {
                cout << "\nIncorrect! Try again.\n\n";
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration_user = duration_cast<seconds>(end_time - start_time).count();

        // Store user data with attempts
        u.insert_min_user(duration_user, ptr, attempts);
        int temp = u.size_u;

        // Heapify and sort
        for(int i=temp/2; i>0; i--) {
            u.heapify_min(i, temp);
        }
        u.user_min_sort(u.size_u);

        cout << "\nEnter 1 to play another game \nEnter 2 to exit\n";
        cin >> choice;
        cout << endl;

        while(1) {
            if(choice == 1) {
                u.print_score();
                break;
            }
            else if(choice == 2) {
                u.print_score();
                exit(1);
            }
            else {
                cout << "\n\t** Invalid Input **\n\nEnter Again: ";
                cin >> choice;
                cout << endl;
            }
        }
    }

    u.print_score();
    return 0;
}