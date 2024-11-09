#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

struct Automaton {
    int alphabetSize;
    int numOfStates;
    int initialState;
    vector<int> finalStates;
    unordered_map<int, unordered_map<char, int>> transitions;
};

Automaton readAutomatonFromFile(const std::string& filename) {
    Automaton automaton;
    ifstream file(filename);

    if (!file) {
        cerr << "Failed to open file." << endl;
    }

    file >> automaton.alphabetSize;
    file >> automaton.numOfStates;
    file >> automaton.initialState;

    cout << "Number of symbols in the alphabet: " << automaton.alphabetSize << endl;
    cout << "Number of states: " << automaton.numOfStates << endl;
    cout << "Initial state: " << automaton.initialState << endl;

    int state;
    while (file >> state) {
        automaton.finalStates.push_back(state);
        if (file.peek() == '\n' || file.peek() == EOF) {
            break;
        }
    }

    cout << "Final states: ";
    for (int i = 0; i < automaton.finalStates.size(); ++i) {
        cout << automaton.finalStates[i] << " ";
    }
    cout << endl;

    int from, to;
    char transitionChar;
    while (file >> from >> transitionChar >> to) {
        automaton.transitions[from][transitionChar] = to;
    }

    cout << "Transitions: " << endl;
    for (const auto& statePair : automaton.transitions) {
        for (const auto& transition : statePair.second) {
            cout << "From state " << statePair.first << " on '" << transition.first
                 << "' to state " << transition.second << endl;
        }
    }

    file.close();
    return automaton;
}

// Checking whether it is possible to get to the final state from the given state
bool canReachFinalStateFrom(const Automaton& automaton, int currentState) {
    queue<int> statesQueue;
    statesQueue.push(currentState);
    unordered_set<int> visitedStates;
    visitedStates.insert(currentState);

    while (!statesQueue.empty()) {
        int state = statesQueue.front();
        statesQueue.pop();

        if (find(automaton.finalStates.begin(), automaton.finalStates.end(), state) != automaton.finalStates.end()) {
            return true;
        }

        for (const auto& transition : automaton.transitions.at(state)) {
            int nextState = transition.second;
            if (visitedStates.find(nextState) == visitedStates.end()) {
                visitedStates.insert(nextState);
                statesQueue.push(nextState);
            }
        }
    }
    return false;
}

// checking a word of the form w = w0w1
bool checkForWords(const Automaton& automaton, const string& w0) {
    int currentState = automaton.initialState;

    for (char ch : w0) {
        if (automaton.transitions.count(currentState) == 0 ||
            automaton.transitions.at(currentState).find(ch) == automaton.transitions.at(currentState).end()) {
            return false;
            }
        currentState = automaton.transitions.at(currentState).at(ch);
    }

    bool isCurrentFinal = (find(automaton.finalStates.begin(), automaton.finalStates.end(), currentState) != automaton.finalStates.end());
    bool hasNoFurtherTransitions = (automaton.transitions.count(currentState) == 0 || automaton.transitions.at(currentState).empty());

    if (isCurrentFinal && hasNoFurtherTransitions) {
        return false;
    }

    if (isCurrentFinal && canReachFinalStateFrom(automaton, currentState)) {
        return true;
    }

    return false;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Automaton automaton;
    int n = -1;

    while (n != 0) {
        string filePath;
        cout << "Enter the path to a text file: ";
        cin >> filePath;

        try {
            cout << "-------------------------------------------" << endl;
            automaton = readAutomatonFromFile(filePath);
            cout << "-------------------------------------------" << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            return 1;
        }

        string w0;
        cout << "\nEnter the word w0: ";
        cin >> w0;

        if (checkForWords(automaton, w0)) {
            cout << "\nTrue. The finite automaton accepts a word of the form w = w0w1." << endl;
        } else {
            cout << "\nFalse. The finite automaton does not accept a word of the form w = w0w1." << endl;
        }

        cout << "\nIf you want to continue, enter 1. Otherwise enter 0: ";
        cin >> n;
    }

    // C:/Users/USER/Desktop/input2.txt
    // C:/Users/USER/Desktop/input.txt
    // C:/Users/USER/Desktop/input3.txt

    return 0;
}