#include <iostream>//for input and output
#include <string>//for string handling
#include <ctime>//for rand()
#include <fstream>//for file handling
#include <chrono> // Add this include for timing
#include <cstdlib> // for exit()
// i have to record this thing
// Au didnt teach exit()
//so i didn't know
// therefore i made a global var xxx
// and a if xxx = 0 return 0 in main(
//everything i want to exit in other function 
// i set xxx = 0
// how stupid is that :(

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

//global variables

string generateSecretCode(int length, int range ) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit = '0' + rand() % range + 1; 
        code += digit;
    }
    return code;
}// using rand()

bool Input_validation(string& input,int length) {
    while (input.length() != length ) {
        if (input == "exit" || input == "save" || input == "load" || input == "cheat" || input == "customize_difficulty" || input == "start" || input == "soc") {
            return true; // allow commands to pass through
        }
        cout << "Invalid input. Please enter a " << length << "-digit number: ";
        cin >> input;
    }
    return true;
}// validate length

string redpegs(string secretCode, string playerGuess) {
    int correctPosition = 0;
    for (int i = 0; i < secretCode.length(); ++i) {
        if (secretCode[i] == playerGuess[i]) {
            correctPosition++;
        }
    }
    string output = "Number of red pegs: " + to_string(correctPosition);
    return output;
}// correct position && number

string whitepegs(string secretCode, string playerGuess) {
    int correctNumber = 0;
    for ( int i = 0 ; i < secretCode.length(); ++i) {
        for ( int j = 0 ; j < playerGuess.length() ; ++j ) {
            if (secretCode[i] == playerGuess[i]) {
                i++;
            }
            if (secretCode[i] == playerGuess[j] && i != j) {
                correctNumber++;
                break;
            }
        }
    }
    string output = "Number of white pegs: " + to_string(correctNumber);
    return output;  
}// correct number but wrong position but not both
//if code 1234 enter 1111 -> 1 red peg only
//if code 1234 enter 4321 -> 0 red peg 4 white pegs
//if code 1234 enter 1222 -> 2 red pegs 0 white pegs
//confusing but easy rule :(
//whatever , it works

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
// get length range maxattempts all at one function
// i am a genius :)

void customize_difficulty(){
    getdifficulty("code length", codeLength);
    getdifficulty("range", range);
    getdifficulty("max attempts", maxAttempts);
    cout << endl ;
    cout << "Code length: " << codeLength << ", Range: 1-" << range  << ", Max attempts: " << maxAttempts << endl;
    cout << endl ;
}// recall the getdifficaulty
// type less in the main() :)

void cheatmode(string secretCode) {
    cout << "The secret code is: " << secretCode << endl;
}//idk why i even make a function for this XD
//thought it make the main() easier to read
//the comment is already longer than the function lmao XD

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
}// such a waste of space but it looks cool :)
//whatever ,follow the requirement :)
//sba need to write this in the report :)

//this is a nightmare to write and debug
//i hate file handling in c++
//:(
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
    //save the existing file
    //getline usage: getline; getline; getline; --> line1 line2 lin3 
    //Au didn't teach me this in class :(
    //i dont think ofstream can overwrite specific line
    //so i have to read all line first then overwrite the specific line
    //then write all line back to the file
    //this is so stupid :(
    //but it works :)

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
    //1-7line for game0 8-14line for game1 and so on
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
bool handle_command(string &xx, string &secretCode, string &latestguess);
//prototype
//real code is below

//game function, i put it out instaed of main()
//because it is too long
//and main() is already too long
void game(string secretCode, int codeLength, int range, int maxAttempts, string latestguess) {
    start_timer(); // Start the timer at the beginning of the game
    while (true) {
            string guess;
            cout << "Enter your guess (or type 'exit', 'save', 'load', 'cheat', 'customize_difficulty', 'start', 'soc'): ";
            cin >> guess ;
            Input_validation(guess,codeLength);
            handle_command(guess, secretCode, latestguess);
            //see how clear because of the functions :)
            //i hate long main() function
            bool isAllDigits = true;
            for (int i = 0; i < guess.length(); ++i) {
                if (!isdigit(guess[i])) {
                    isAllDigits = false;
                    break;
                }
            }
            //isdigit check
            if (isAllDigits) {
                attempts++;
                cout << redpegs(secretCode, guess) << endl;
                cout << whitepegs(secretCode, guess) << endl;//real game part, only 3 lines lmao
                attemptleft(attempts, maxAttempts) ;
            }
            if (attempts >= maxAttempts) {
                cout << "You've used all attempts! The secret code was: " << secretCode << endl;
                stop_timer(); // Show time used when game ends.i really don't know how to make a realtime timer in cli , there is really no need for that anyway
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

//finally the main function
//wanna cry
//how simple and short it is
//i am such a genius
int main(){
    gamepage();
    string xx;
    srand((unsigned)time(NULL));
    secretCode = generateSecretCode(codeLength, range);
    cin >> xx;
    attempts = 0;
    latestguess = "";
    //xx is the starter command
    while (true) {
        if (!handle_command(xx, secretCode, latestguess)) break;
        cin >> xx;
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
        cout << "Goodbye! press any key to exit" << endl;
        exit(0); // Exit the program
    }
}

//command handler function
bool handle_command(string &xx, string &secretCode, string &latestguess) {
    if (xx == "customize_difficulty") {
        customize_difficulty();
    } else if (xx == "exit") {
        cout << "Goodbye!" << endl;
        tryagain();
        return false; // break loop
    } else if (xx == "cheat") {
        cheatmode(secretCode);
        return true; // continue loop
    } else if (xx == "save") {
        savegame(secretCode, latestguess);
    } else if (xx == "load") {
        loadgame(secretCode, latestguess);
    } else if (xx == "start") {
        cout << "Starting a new game..." << endl;
        game(secretCode, codeLength, range, maxAttempts, latestguess);
        tryagain();
        return false; // break loop
    } else if (xx == "soc") {
        cout << "Enter your own secret code: ";
        cin >> secretCode;
        codeLength = secretCode.length();
    } else if (Input_validation(xx, codeLength)) {
        return true; // continue loop
    } else {
        cout << "Invalid command. Please try again." << endl;
        main();
        return false; // break loop
    }
    return true; // continue loop
}