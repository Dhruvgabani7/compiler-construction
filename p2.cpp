#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

int main() {
    map<int, map<char, int>> transitions;

    int n;
    cout << "Number of input symbols: ";
    cin >> n;

    vector<char> symbols(n);
    cout << "Input symbols:\n";
    for (int i = 0; i < n; i++) {
        cin >> symbols[i];
    }

    int states;
    cout << "Enter number of states: ";
    cin >> states;

    int initialState;
    cout << "Initial state: ";
    cin >> initialState;

    int numAccepting;
    cout << "Number of accepting states: ";
    cin >> numAccepting;

    set<int> acceptingStates;
    cout << "Accepting states:\n";
    for (int i = 0; i < numAccepting; i++) {
        int x;
        cin >> x;
        acceptingStates.insert(x);
    }

    cout << "Transition table:\n";
    for (int i = 1; i <= states; i++) {
        map<char, int> innerMap;
        for (int j = 0; j < n; j++) {
            cout << i << " to " << symbols[j] << " -> ";
            int nextState;
            cin >> nextState;
            innerMap[symbols[j]] = nextState;
        }
        transitions[i] = innerMap;
    }

    string input;
    cin >> input;

    for (char c : input) {
        initialState = transitions[initialState][c];
    }

    if (acceptingStates.count(initialState)) {
        cout << "Valid String\n";
    } else {
        cout << "Invalid String\n";
    }

    return 0;
}