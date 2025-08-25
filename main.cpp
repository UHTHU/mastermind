#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

int codeLength = 4;
int range = 6; 
int maxAttempts = 10; 
int attempts = 0;
string secretCode;
string latestguess;
int xxx = 1;
int currentgamefile = 0;

string generateSecretCode(int length, int range ) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit = '0' + rand() % range + 1; 
        code += digit;
    }
    return code;
}

void Input_validation(string& input,int length) {
    while (input.length() != length ) {
        cout << "Invalid input. Please enter a " << length << "-digit number: ";
        cin >> input;
    }
}

string redpegs(string secretCode, string playerGuess) {
    int correctPosition = 0;
    for (int i = 0; i < secretCode.length(); ++i) {
        if (secretCode[i] == playerGuess[i]) {
            correctPosition++;
        }
    }
    string output = "Number of red pegs: " + to_string(correctPosition);
    return output;
}

string whitepegs(string secretCode, string playerGuess) {
    int correctNumber = 0;
    for ( int i = 0 ; i < playerGuess.length(); ++i) {
        for ( int j = 0 ; j < secretCode.length() ; ++j ) {
            if (playerGuess[i] == secretCode[j] && i != j) {
                correctNumber++;
                break;
            }
        }
    }
    string output = "Number of white pegs: " + to_string(correctNumber);
    return output;  
}

void getdifficulty( string category, int& defaultValue) {
    cout << "Enter the value of " << category << " (default is " << defaultValue << ") : " << endl;
    cin >> defaultValue;
    if (category == "range"){
        while (defaultValue < 1 || defaultValue > 10) { 
            cout << "Invalid range. Must be between 1 to 10." << endl;
            cin >> defaultValue;
        }
    }
}

void settings(){
    getdifficulty("code length", codeLength);
    getdifficulty("range", range);
    getdifficulty("max attempts", maxAttempts);
    cout << endl ;
    cout << "Code length: " << codeLength << ", Range: 1-" << range  << ", Max attempts: " << maxAttempts << endl;
    cout << endl ;
}

void cheatmode(string secretCode) {
    cout << "The secret code is: " << secretCode << endl;
}

void attemptleft(int attempts, int maxAttempts) {
    cout << "You have used " << attempts << " out of " << maxAttempts << " attempts." << endl;
    cout << "You have " << (maxAttempts - attempts) << " attempts left." << endl;
    cout << endl ;
}

void gamepage(){
    cout << "===================================" << endl;
    cout << "          MASTERMIND GAME          " << endl;
    cout << "===================================" << endl;
    cout << "      Welcome to Mastermind!       " << endl;
    cout << endl;
    cout << "Game Rules:"<< endl;
    cout << "1. A secret code of 4 digits (1-6) is generated." << endl;
    cout << "2. You have 10 tries to guess the code." << endl;
    cout << "3. Feedback after each guess:"<< endl;
    cout << "   - Red Peg: Correct digit in correct position" << endl;
    cout << "   - White Peg: Correct digit in wrong position" << endl;
    cout << endl;
    cout << "Special Commands:" << endl;
    cout << "cheat -> Show the secret code" << endl;
    cout << "save -> Save your current game"    << endl;
    cout << "load -> Load your saved game" << endl;
    cout << "exit -> Exit the game" << endl;
    cout << "settings -> Change game settings" << endl;
    cout << "start -> Start a new game" << endl;
    cout << "soc -> set own code " << endl;
    cout << endl ;
}



void savegame(string secretCode, string latestguess){
    const int totalGames = 5;
    const int linesPerGame = 7;
    string lines[totalGames * linesPerGame] = {""};

    // Read existing file lines
    ifstream inFile("savegame.txt");
    int idx = 0;
    while (inFile && idx < totalGames * linesPerGame) {
        getline(inFile, lines[idx]);
        idx++;
    }
    inFile.close();

    cout << "save which game file? (0/1/2/3/4): ";
    cin >> currentgamefile;
    int base = currentgamefile * linesPerGame;

    // Overwrite the slot
    lines[base + 0] = to_string(currentgamefile);
    lines[base + 1] = to_string(codeLength);
    lines[base + 2] = to_string(range);
    lines[base + 3] = to_string(maxAttempts);
    lines[base + 4] = to_string(attempts);
    lines[base + 5] = secretCode;
    lines[base + 6] = latestguess;

    // Write all lines back
    ofstream outFile("savegame.txt");
    for (int i = 0; i < totalGames * linesPerGame; ++i) {
        outFile << lines[i] << endl;
    }
    outFile.close();

    cout << "Game saved successfully." << endl;
    cout <<" current game file: " << currentgamefile << endl;
    cout <<" code length: " << codeLength << endl;
    cout <<" range: " << range << endl; 
    cout <<" max attempts: " << maxAttempts << endl;
    cout <<" attempts: " << attempts << endl;
    cout <<" secret code: " << "****" << endl;
    cout <<" latest guess: " << latestguess << endl;
}

void loadgame(string &secretCode, string &latestguess){
    const int totalGames = 5;
    const int linesPerGame = 7;
    string lines[totalGames * linesPerGame] = {""};

    ifstream inFile("savegame.txt");
    int idx = 0;
    while (inFile && idx < totalGames * linesPerGame) {
        getline(inFile, lines[idx]);
        idx++;
    }
    inFile.close();

    cout << "load which game file? (0/1/2/3/4): ";
    cin >> currentgamefile;
    int base = currentgamefile * linesPerGame;

    // Read from the slot
    if (lines[base].empty()) {
        cout << "No saved game in this slot." << endl;
        return;
    }
    currentgamefile = stoi(lines[base + 0]);
    codeLength = stoi(lines[base + 1]);
    range = stoi(lines[base + 2]);
    maxAttempts = stoi(lines[base + 3]);
    attempts = stoi(lines[base + 4]);
    secretCode = lines[base + 5];
    latestguess = lines[base + 6];

    cout << "Game loaded successfully." << endl;
    cout <<" current game file: " << currentgamefile << endl;
    cout <<" code length: " << codeLength << endl;
    cout <<" range: " << range << endl; 
    cout <<" max attempts: " << maxAttempts << endl;
    cout <<" attempts: " << attempts << endl;
    cout <<" secret code: " << "****" << endl;
    cout <<" latest guess: " << latestguess << endl;
}

void tryagain();

void game(string secretCode, int codeLength, int range, int maxAttempts, string latestguess) {
    secretCode = generateSecretCode(codeLength, range);
    while (true) {
            string guess;
            cout << "Enter your guess (or type 'exit', 'save', 'load', 'cheat', 'settings', 'start'): ";
            cin >> guess ;
            if (guess == "exit") {
                cout << "Goodbye!" << endl;
                xxx = 0;
                break;
            } else if (guess == "save") {
                savegame(secretCode, latestguess);
                continue;
            } else if (guess == "load") {
                loadgame(secretCode, latestguess);
                continue;
            } else if (guess == "cheat") {
                cheatmode(secretCode);
                continue;
            } else if (guess == "settings") {
                settings();
                continue;
            }else if (guess == "soc") {
                cout << "Enter your own secret code: ";
                cin >> secretCode;
                Input_validation(secretCode, codeLength);
            } else if (guess == "start") {
                cout << "Starting a new game..." << endl;
                attempts = 0; 
                latestguess = "";
                continue;
            }
            Input_validation(guess,codeLength);
            cout << redpegs(secretCode, guess) << endl;
            cout << whitepegs(secretCode, guess) << endl;
            attempts++ ;
            attemptleft(attempts, maxAttempts) ;
            if (attempts > maxAttempts) {
                cout << "You've used all attempts! The secret code was: " << secretCode << endl;
                break;
            }

            if (guess == secretCode) {
                cout << "Congratulations! You've guessed the secret code: " << secretCode << endl;
                break;
            }
            latestguess = guess;
    }
}

int main(){
    if (xxx == 0)
    {
       return 0;
    }
    
    gamepage();
    string xx;
    secretCode = generateSecretCode(codeLength, range);
    cin >> xx;
    attempts = 0;
    latestguess = "";

    srand((unsigned)time(NULL));  // Seed the random number generator
    srand((unsigned)time(NULL));
    while(true){
        if (xx == "settings") {
            settings();
        } else if (xx == "exit") {
            cout << "Goodbye!" << endl;
            tryagain();
        } else if (xx == "cheat") {
            cheatmode(secretCode);
        } else if (xx == "save") {
            savegame(secretCode, latestguess);
        } else if (xx == "load"){
            loadgame(secretCode, latestguess);
        } else if (xx == "start") {
            cout << "Starting a new game..." << endl;
            game( secretCode, codeLength, range, maxAttempts, latestguess);
            tryagain();
        }else {
            cout << "Invalid command. Please try again." << endl;
            main();
        }
        cin >> xx;
        if (xxx == 0)
        {
           return 0;
        }
    }  
    return 0;
}

void tryagain(){
    cout << "try again? (Y/N): ";
    char tryAgain;  
    cin >> tryAgain;
    while (tryAgain != 'Y' && tryAgain != 'y' && tryAgain != 'N' && tryAgain != 'n') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> tryAgain;
    }
    if (tryAgain == 'Y' || tryAgain == 'y') {
        main(); // Restart the game
    } else {
        cout << "Goodbye!" << endl;
        xxx = 0;
        main(); // Exit the game
    }
}