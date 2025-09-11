#include <iostream>//for input and output
#include <string>//for string handling
#include <ctime>//for rand()
#include <fstream>//for file handling
#include <chrono> // Add this include for timing
#include <cstdlib> // for exit()
using namespace std;
using namespace std::chrono; // in the timing functions

int codeLength = 4;
int range = 6; 
int maxAttempts = 10; 
int attempts = 0;
string secretCode;
string latestguess;
int currentgamefile = 0;

steady_clock::time_point startTime;
steady_clock::time_point endTime;


string generateSecretCode(int length, int range ) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit = '0' + rand() % range + 1; 
        code += digit;
    }
    return code;
}

bool Is_command(string& input,int length) {
    while (input.length() != length ) {
        if (input == "exit" || input == "save" || input == "load" || input == "cheat" || input == "customize_difficulty" || input == "start" || input == "soc") {
            return true; // allow commands to pass through
        }
        cout << "Invalid input. Please enter a " << length << "-digit number: ";
        cin >> input;
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
    int correctNumber = 0;
    // Create frequency maps for secret and guess
    int secretCount[10] = {0};  // Assuming digits 0-9
    int guessCount[10] = {0};
    
    for (char c : secretCode) {
        secretCount[c - '0']++;
    }
    for (char c : playerGuess) {
        guessCount[c - '0']++;
    }
    
    // Calculate total matches
    int totalMatches = 0;
    for (int i = 0; i < 10; ++i) {
        totalMatches += min(secretCount[i], guessCount[i]);
    }
    
    // Subtract red pegs (correct position matches)
    int redPegs = 0;
    for (size_t i = 0; i < secretCode.length(); ++i) {
        if (secretCode[i] == playerGuess[i]) {
            redPegs++;
        }
    }
    
    correctNumber = totalMatches - redPegs;
    
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

void customize_difficulty(){
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
}// only for typing less in main() lazy me  :)

void start_timer(){ 
    startTime = steady_clock::now();
}// chrono library 
// i dont think i need to explain this one :)

void stop_timer(){
    endTime = steady_clock::now();
    auto duration = duration_cast<seconds>(endTime - startTime).count();
    cout << "Elapsed time: " << duration << " seconds." << endl;
}// same as above :)

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
    cout << "customize_difficulty -> Change game settings e.g. code length, range and maxattempts " << endl;
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
    while (idx < totalGames * linesPerGame) {
        getline(inFile, lines[idx]);
        idx++;
    }
    inFile.close();

    cout << "save which game file? (0/1/2/3/4): ";
    cin >> currentgamefile;
    int base = currentgamefile * linesPerGame;

    lines[base + 0] = to_string(currentgamefile);
    lines[base + 1] = to_string(codeLength);
    lines[base + 2] = to_string(range);
    lines[base + 3] = to_string(maxAttempts);
    lines[base + 4] = to_string(attempts);
    lines[base + 5] = secretCode;
    lines[base + 6] = latestguess;
    //save the game in a string array
    //overwrite only the current game corresponding lines

    // Write all lines back including the original and updated 
    ofstream outFile("savegame.txt");
    for (int i = 0; i < totalGames * linesPerGame; ++i) {
        outFile << lines[i] << endl;
    }
    outFile.close();
    //using for loop to put all line[35] into txt

    cout << "Game saved successfully." << endl;
    cout <<" current game file: " << currentgamefile << endl;
    cout <<" code length: " << codeLength << endl;
    cout <<" range: " << range << endl; 
    cout <<" max attempts: " << maxAttempts << endl;
    cout <<" attempts: " << attempts << endl;
    cout <<" secret code: " << "****" << endl;
    cout <<" latest guess: " << latestguess << endl;
    //debugging
}

void loadgame(string &secretCode, string &latestguess){
    const int totalGames = 5;
    const int linesPerGame = 7;
    string lines[totalGames * linesPerGame] = {""};

    ifstream inFile("savegame.txt");
    int idx = 0;
    while (idx < totalGames * linesPerGame) {
        getline(inFile, lines[idx]);
        idx++;
    }
    inFile.close();
    //read all lines in txt
    //getline usage: getline; getline; getline; --> line1 line2 lin3 

    cout << "load which game file? (0/1/2/3/4): ";
    cin >> currentgamefile;
    int base = currentgamefile * linesPerGame;
    //1-7 line for game0 8-14line for game1 and so on
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
    //same as savegame
    //stoi is string to int

    cout << "Game loaded successfully." << endl;
    cout <<" current game file: " << currentgamefile << endl;
    cout <<" code length: " << codeLength << endl;
    cout <<" range: " << range << endl; 
    cout <<" max attempts: " << maxAttempts << endl;
    cout <<" attempts: " << attempts << endl;
    cout <<" secret code: " << "****" << endl;
    cout <<" latest guess: " << latestguess << endl;
    //debugging
}

void tryagain();
void game(string secretCode, int codeLength, int range, int maxAttempts, string latestguess);
bool handle_command(string &firstcommand, string &secretCode, string &latestguess);

bool isdigit(string c) {
    for (int i = 0; i < c.length(); ++i) {
        if (c[i] < '0' || c[i] > '9') {
            return false;
        }
    }
}

void game(string secretCode, int codeLength, int range, int maxAttempts, string latestguess) {
    start_timer(); // Start the timer at the beginning of the game
    while (true) {
            string guess;
            cout << "Enter your guess (or type 'exit', 'save', 'load', 'cheat', 'customize_difficulty', 'start', 'soc'): ";
            cin >> guess ;
            Is_command(guess,codeLength);
            handle_command(guess, secretCode, latestguess);
            if (isdigit(guess) && guess.length() == codeLength) {
                attempts++;
                cout << redpegs(secretCode, guess) << endl;
                cout << whitepegs(secretCode, guess) << endl;//real game part, only 3 lines lmao
                attemptleft(attempts, maxAttempts) ;
            }
            if (attempts >= maxAttempts) {
                cout << "You've used all attempts! The secret code was: " << secretCode << endl;
                stop_timer(); 
                break;
            }

            if (guess == secretCode) {
                cout << "Congratulations! You've guessed the secret code: " << secretCode << endl;
                stop_timer(); // Show time used when game ends
                break;
            }
            latestguess = guess;
    }
}

int main(){
    gamepage();
    string firstcommand;
    srand((unsigned)time(NULL));
    secretCode = generateSecretCode(codeLength, range);
    cin >> firstcommand;
    attempts = 0;
    latestguess = "";
    //firstcommand is the starter command
    while (true) {
        if (!handle_command(firstcommand, secretCode, latestguess)) break;
        cin >> firstcommand;
    }  
    return 0;
}

//try again?
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
        cout << "Goodbye! press any key to exit" << endl;
        exit(0); // Exit the program
    }
}

bool handle_command(string &firstcommand, string &secretCode, string &latestguess) {
    if (firstcommand == "customize_difficulty") {
        customize_difficulty();
    } else if (firstcommand == "exit") {
        cout << "Goodbye!" << endl;
        tryagain();
        return false; // break loop
    } else if (firstcommand == "cheat") {
        cheatmode(secretCode);
        return true; // continue loop
    } else if (firstcommand == "save") {
        savegame(secretCode, latestguess);
    } else if (firstcommand == "load") {
        loadgame(secretCode, latestguess);
    } else if (firstcommand == "start") {
        cout << "Starting a new game..." << endl;
        game(secretCode, codeLength, range, maxAttempts, latestguess);
        tryagain();
        return false; // break loop
    } else if (firstcommand == "soc") {
        cout << "Enter your own secret code: ";
        cin >> secretCode;
        codeLength = secretCode.length();
    } else if (Is_command(firstcommand, codeLength)) {
        return true; // continue loop
    } else {
        cout << "Invalid command. Please try again." << endl;
        main();
        return false; // break loop
    }
    return true; // continue loop
}