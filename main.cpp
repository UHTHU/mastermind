#include <iostream> //for input and output
#include <string>   //for string handling
#include <ctime>    //for rand()
#include <fstream>  //for file handling
#include <chrono>   // Add this include for timing
#include <cstdlib>  // for exit()
using namespace std;
using namespace std::chrono; // for timing 
int codeLength = 4;
int range = 6; 
int maxAttempts = 10; 
int attempts = 0;
string secretCode;
string latestguess;
int currentgamefile = 0;
bool allowDuplicates = true ;
bool gamestarted = false;

steady_clock::time_point startTime;
steady_clock::time_point endTime;

string generateSecretCode(int length, int range, bool allowDuplicates) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit;
        bool isDuplicate;
        do {
            digit = '0' + rand() % range + 1;
            isDuplicate = false;
            if (!allowDuplicates) {
                for (int j = 0; j < i; ++j) {
                    if (code[j] == digit) {
                        isDuplicate = true;
                        break;
                    }
                }
            }
        } while (isDuplicate);
        code += digit;
    }
    return code;
}

bool isdigitstring(string input) {
    for (int i = 0; i < input.length(); ++i) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
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
    int length = secretCode.length();  
    bool used[20] = {false}; // assume max is 20
    int correctNumber = 0;
    
    string redOutput = redpegs(secretCode, playerGuess);
    int redPegs = stoi(redOutput.substr(19));  
    
    for (int i = 0; i < length; ++i) {
        if (secretCode[i] == playerGuess[i]) continue;  
        for (int j = 0; j < length; ++j) {
            if (!used[j] && secretCode[j] == playerGuess[i]) {
                correctNumber++;
                used[j] = true;  
                break;  
            }
        }
    }
    
    string output = "Number of white pegs: " + to_string(correctNumber);
    return output;
}

void getdifficulty(string category, int& defaultValue) {
    cout << "Enter the value for " << category << " (default is " << defaultValue << "): ";
    cin >> defaultValue;
    if (category == "range") {
        while (defaultValue < 1 || defaultValue > 10) { 
            cout << "Invalid range. Must be between 1 and 10." << endl;
            cout << "Enter the value for " << category << ": ";
            cin >> defaultValue;
        }
    } else if (category == "code length" || category == "max attempts") {
        while (defaultValue < 1 || defaultValue > 20) {
            cout << "Invalid value. Must be between 1 and 20." << endl;
            cout << "Enter the value for " << category << ": ";
            cin >> defaultValue;
        }
    } else if (category == "allow duplicates (1 for yes, 0 for no)") {
        while (defaultValue != 0 && defaultValue != 1) {
            cout << "Invalid value. Must be 0 or 1." << endl;
            cout << "Enter the value for " << category << ": ";
            cin >> defaultValue;
        }
    }
}

void customize_difficulty() {
    getdifficulty("code length", codeLength);
    getdifficulty("range", range);
    getdifficulty("max attempts", maxAttempts);
    int intallowDuplicates = allowDuplicates ? 1 : 0;
    getdifficulty("allow duplicates (1 for yes, 0 for no)", intallowDuplicates);
    allowDuplicates = (intallowDuplicates == 1);
    cout << endl;
    cout << "Settings updated: Code length " << codeLength 
         << ", Digits 1-" << range 
         << ", Max attempts " << maxAttempts 
         << ", Duplicates " << (allowDuplicates ? "allowed" : "not allowed") << "." << endl;
    cout << endl;
}

void cheatmode(string secretCode) {
    cout << "Cheat mode activated! The secret code is: " << secretCode << endl;
}

void attemptleft(int attempts, int maxAttempts) {
    cout << "Attempts used: " << attempts << " / " << maxAttempts << " ("
         << (maxAttempts - attempts) << " left)." << endl;
    cout << endl;
}

void start_timer() { 
    startTime = steady_clock::now();
}

void stop_timer() {
    endTime = steady_clock::now();
    auto duration = duration_cast<seconds>(endTime - startTime).count();
    cout << "Time elapsed: " << duration << " seconds." << endl;
}

void gamepage() {
    cout << "===================================" << endl;
    cout << "          MASTERMIND GAME          " << endl;
    cout << "===================================" << endl;
    cout << "      Welcome to Mastermind!       " << endl;
    cout << endl;
    cout << "Game Rules:" << endl;
    cout << "1. A secret code of " << codeLength << " digits (1-" << range << ") is generated." << endl;
    cout << "2. You have " << maxAttempts << " tries to guess the code." << endl;
    cout << "3. Feedback after each guess:" << endl;
    cout << "   - Red Peg: Correct digit in correct position" << endl;
    cout << "   - White Peg: Correct digit in wrong position" << endl;
    cout << endl;
    cout << "Special Commands:" << endl;
    cout << "/1 -> Start a new game" << endl;
    cout << "/2 -> Save your current game" << endl;
    cout << "/3 -> Load a saved game" << endl;
    cout << "/4 -> Exit the game" << endl;
    cout << "/5 -> Change game settings" << endl;
    cout << "/6 -> Show the secret code (cheat)" << endl;
    cout << "/7 -> Set your own secret code" << endl;
    cout << endl;
}

void savegame(string secretCode, string latestguess) {
    const int totalGames = 5;
    const int linesPerGame = 7;
    string lines[totalGames * linesPerGame];

    // set a array 
    for (int i = 0; i < totalGames * linesPerGame; ++i) {
        lines[i] = "";
    }

    // load exiting file 2 array
    ifstream inFile("savegame.txt");
    if (inFile) {
        int idx = 0;
        while (idx < totalGames * linesPerGame && getline(inFile, lines[idx])) {
            ++idx;
        }
        inFile.close();
    }

    cout << "Save to which slot? (0-4): ";
    cin >> currentgamefile;
    if (currentgamefile < 0 || currentgamefile > 4) {
        cout << "Invalid slot. Using 0." << endl;
        currentgamefile = 0;
    }
    int base = currentgamefile * linesPerGame;

    lines[base + 0] = to_string(currentgamefile);
    lines[base + 1] = to_string(codeLength);
    lines[base + 2] = to_string(range);
    lines[base + 3] = to_string(maxAttempts);
    lines[base + 4] = to_string(attempts);
    lines[base + 5] = secretCode;
    lines[base + 6] = latestguess;

    // load array 2 file
    ofstream outFile("savegame.txt");
    for (int i = 0; i < totalGames * linesPerGame; ++i) {
        outFile << lines[i] << endl;
    }
    outFile.close();

    cout << "Game saved to slot " << currentgamefile << "." << endl;
    cout << "Settings: Code length " << codeLength 
         << ", Digits 1-" << range 
         << ", Max attempts " << maxAttempts 
         << ", Duplicates " << (allowDuplicates ? "allowed" : "not allowed") << "." << endl;
    cout << "Progress: " << attempts << " / " << maxAttempts << " attempts used." << endl;
    cout << "Last guess: " << latestguess << endl;
    cout << endl;
}

void loadgame(string &secretCode, string &latestguess) {
    const int totalGames = 5;
    const int linesPerGame = 7;
    string lines[totalGames * linesPerGame];

    for (int i = 0; i < totalGames * linesPerGame; ++i) {
        lines[i] = "";
    }

    ifstream inFile("savegame.txt");
    if (inFile) {
        int idx = 0;
        while (idx < totalGames * linesPerGame && getline(inFile, lines[idx])) {
            ++idx;
        }
        inFile.close();
    }

    cout << "Load from which slot? (0-4): ";
    cin >> currentgamefile;
    if (currentgamefile < 0 || currentgamefile > 4) {
        cout << "Invalid slot. Using 0." << endl;
        currentgamefile = 0;
    }
    int base = currentgamefile * linesPerGame;

    if (lines[base].empty()) {
        cout << "No saved game in slot " << currentgamefile << "." << endl;
        return;
    }
//loadgame can refer to savegame ( they are same but in reverse )

    currentgamefile = stoi(lines[base + 0]);
    codeLength = stoi(lines[base + 1]);
    range = stoi(lines[base + 2]);
    maxAttempts = stoi(lines[base + 3]);
    attempts = stoi(lines[base + 4]);
    secretCode = lines[base + 5];
    latestguess = lines[base + 6];

    gamestarted = true;  
    start_timer();  

    cout << "Game loaded from slot " << currentgamefile << "." << endl;
    cout << "Settings: Code length " << codeLength 
         << ", Digits 1-" << range 
         << ", Max attempts " << maxAttempts 
         << ", Duplicates " << (allowDuplicates ? "allowed" : "not allowed") << "." << endl;
    cout << "Progress: " << attempts << " / " << maxAttempts << " attempts used." << endl;
    cout << "Last guess: " << latestguess << endl;
    cout << endl;
}

void set_own_code(string &secretCode) {
    string customCode;
    bool valid = false;
    cout << "Enter your custom secret code (" << codeLength << " digits, 1-" << range << "): ";
    cin >> customCode;
    secretCode = customCode;
    cout << "Custom secret code set!" << endl;
    cout << endl;
}

void handle_command(string input) {
    if (input == "/1") {
        gamestarted = true;
        attempts = 0;
        secretCode = generateSecretCode(codeLength, range, allowDuplicates);
        start_timer();
        cout << "New game started! Good luck." << endl;
        cout << endl;
    } else if (input == "/2") {
        if (gamestarted) {
            savegame(secretCode, latestguess);
        } else {
            cout << "No active game to save. Start a game first (/1)." << endl;
        }
    } else if (input == "/3") {
        loadgame(secretCode, latestguess);
    } else if (input == "/4") {
        cout << "Thanks for playing! Goodbye." << endl;
        exit(0);
    } else if (input == "/5") {
        customize_difficulty();
    } else if (input == "/6") {
        if (gamestarted) {
            cheatmode(secretCode);
        } else {
            cout << "No active game. Start a game first (/1)." << endl;
        }
    } else if (input == "/7") {
        if (gamestarted) {
            cout << "Warning: Changing secret mid-game." << endl;
        }
        set_own_code(secretCode);
    }
}

bool iscommand(string input) {
    return (input == "/1" || input == "/2" || input == "/3" || input == "/4" || 
            input == "/5" || input == "/6" || input == "/7");
}

bool validguesscheck(string guess) {
    if (guess.length() != static_cast<size_t>(codeLength)) {
        cout << "Invalid length: Must be " << codeLength << " digits." << endl;
        return false;
    }
    if (!isdigitstring(guess)) {
        cout << "Invalid: Only digits allowed." << endl;
        return false;
    }
    for (int i = 0; i < codeLength; ++i) {
        if (guess[i] < '1' || guess[i] > '0' + range) {
            cout << "Invalid digit: Use 1-" << range << "." << endl;
            return false;
        }
    }
    return true;
}

void game(string guess) {
    if (!validguesscheck(guess)) {
        return;  
    }

    cout << "Guess: " << guess << endl;
    latestguess = guess;
    attempts++;

    cout << redpegs(secretCode, guess) << endl;
    cout << whitepegs(secretCode, guess) << endl;
    attemptleft(attempts, maxAttempts);
    
    if (guess == secretCode) {
        cout << "Congratulations! You cracked the code!" << endl;
        stop_timer();
        gamestarted = false;
        attempts = 0;
    } else if (attempts >= maxAttempts) {
        cout << "Out of attempts! The code was: " << secretCode << endl;
        stop_timer();
        gamestarted = false;
        attempts = 0;
    }
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed 
    gamepage(); 

    string input;
    
    while (true) {
        if (!gamestarted) {
            cout << "Enter a command (/1 to start): ";
        } else {
            cout << "Enter guess or command: ";
        }
        cin >> input;
        
        if (iscommand(input)) {
            handle_command(input);
            if (input == "/4") {
                break;
            }
            continue;
        }
        
        if (gamestarted) {
            game(input);
        } else {
            cout << "Game not started. Use /1 to begin." << endl;
            cout << endl;
        }
    }
    return 0;
}