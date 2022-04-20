#include <vector>
#include <iostream>
#include <unordered_set>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Board_Node {

public:
    vector<Board_Node> frontier;
    Board_Node* parent;
    int blank_index = 0;
    vector<char> board;


    //constructor which takes the initial state as input and assigning it to the board node
    Board_Node(vector<char> start_state) {
        for (int i = 0; i < start_state.size(); i++) {
            board.push_back(start_state[i]);
        }

    }
    //method to move right, creates a child of this node and assigning this node as the parent of the child.
    void move_right(vector<char> b, int j) {
        if ((j != 2) && (j != 5) && (j != 8)) {
            vector<char> new_board;
            make_copy(new_board, b);
            char temp = new_board[j + 1];
            new_board[j + 1] = new_board[j];
            new_board[j] = temp;

            Board_Node child(new_board);
            frontier.push_back(child);
            child.parent = this;
        }
    }
    //method to move left, the same as right but with different indexing operations
    void move_left(vector<char> b, int j) {
        if ((j != 0) && (j != 3) && (j != 6)) {
            vector<char> new_board;
            make_copy(new_board, b);
            char temp = new_board[j - 1];
            new_board[j - 1] = new_board[j];
            new_board[j] = temp;

            Board_Node child(new_board);
            frontier.push_back(child);
            child.parent = this;
        }
    }

    void move_up(vector<char> b, int j) {
        if (j - 3 >= 0) {
            vector<char> new_board;
            make_copy(new_board, b);
            char temp = new_board[j - 3];
            new_board[j - 3] = new_board[j];
            new_board[j] = temp;

            Board_Node child(new_board);
            frontier.push_back(child);
            child.parent = this;
        }
    }

    void move_down(vector<char> b, int j) {
        if (j + 3 < b.size()) {
            vector<char> new_board;
            make_copy(new_board, b);
            char temp = new_board[j + 3];
            new_board[j + 3] = new_board[j];
            new_board[j] = temp;

            Board_Node child(new_board);
            frontier.push_back(child);
            child.parent = this;
        }
    }
    //method to show the state of this node
    void show_state() {
        int x = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[x] == 0) {
                    cout << " " << " ";
                }
                else {
                    cout << board[x] << " ";
                }

                x++;
            }
            cout << endl;
        }

    }
    //method to expand this node
    void expand() {
        for (int i = 0; i < board.size(); i++) {
            if (board[i] == 0) {
                blank_index = i;
            }
        }
        move_right(board, blank_index);
        move_left(board, blank_index);
        move_up(board, blank_index);
        move_down(board, blank_index);
    }
    //method to copy a board state to another board
    void make_copy(vector<char>& copied_board, vector<char> original_board) {
        copied_board = original_board;
    }
};

vector<Board_Node> R(Board_Node starting_node, int num) {
    vector<Board_Node> node_stack;
    vector<Board_Node> marked;
    map<vector<char>, int> my_map;

    node_stack.push_back(starting_node);
    int counter = 1;
    while (node_stack.size() > 0) {
        Board_Node current_node = node_stack.back();
        node_stack.pop_back();
        if (my_map.size() >= num) {
            break;
        }
        if (marked.size() >= 2000000) {
            break;
        }
        if (my_map.find(current_node.board) == my_map.end()) {
            current_node.expand();
            my_map.insert({ current_node.board, counter });
            marked.push_back(current_node);
            for (int i = 0; i < current_node.frontier.size(); i++) {
                Board_Node current_child = current_node.frontier[i];
                if (my_map.find(current_child.board) == my_map.end()) {
                    node_stack.push_back(current_child);
                }
            }
        }
    }

    return marked;

}

int main() {
    istringstream iss;
    vector<char> first_state;
    vector<char> second_state;
    string first_string;
    string second_string;


    cout << "PLEASE INPUT THE FIRST STATE OF THE FIRST BOARD: ";
    getline(cin, first_string);
    iss.str(first_string);
    for (int i = 0; i < 9; i++) {
        char val;
        iss >> val;
        if (val == '0') {
            first_state.push_back(0);
        }
        else {
            first_state.push_back(val);

        }
    }
    Board_Node S1(first_state);
    S1.show_state();

    cout << "PLEASE INPUT THE FIRST STATE OF THE SECOND BOARD: ";
    getline(cin, second_string);
    iss.str(second_string);
    for (int i = 0; i < 9; i++) {
        char val;
        iss >> val;
        if (val == '0') {
            second_state.push_back(0);
        }
        else {
            second_state.push_back(val);
        }
    }

    Board_Node S2(second_state);
    S2.show_state();

    map<vector<char>, int> mapped;


    // Board_Node S1({first_state});
    // Board_Node S1({'a','b','c','d','e','f','g','h',0});
    // Board_Node S2({0,'e','b','a','h','c','d','g','f'});

    vector<Board_Node> state_space = R(S1, 200000);
    vector<Board_Node> state_space2 = R(S2, 200000);

    for (Board_Node i : state_space) {
        mapped.insert({ i.board, 1 });
    }

    vector<Board_Node> S1_intersect_S2;
    for (Board_Node j : state_space2) {
        if (!(mapped.find(j.board) == mapped.end())) {
            S1_intersect_S2.push_back(j);
        }
    }

    ofstream myfile;
    myfile.open("S1_states.txt", ofstream::out | ofstream::trunc);
    myfile << state_space.size() << " states generated." << endl;
    for (Board_Node i : state_space) {
        myfile << i.board[0] << " " << i.board[1] << " " << i.board[2] << endl;
        myfile << i.board[3] << " " << i.board[4] << " " << i.board[5] << endl;
        myfile << i.board[6] << " " << i.board[7] << " " << i.board[8] << endl;
        myfile << endl;
    }

    myfile.close();

    myfile.open("S2_states.txt", ofstream::out | ofstream::trunc);
    myfile << state_space2.size() << " states generated." << endl;
    for (Board_Node i : state_space2) {
        myfile << i.board[0] << " " << i.board[1] << " " << i.board[2] << endl;
        myfile << i.board[3] << " " << i.board[4] << " " << i.board[5] << endl;
        myfile << i.board[6] << " " << i.board[7] << " " << i.board[8] << endl;
        myfile << endl;
    }

    myfile.close();

    myfile.open("S1_intersection_S2.txt", ofstream::out | ofstream::trunc);
    myfile << S1_intersect_S2.size() << " states generated." << endl;
    for (Board_Node i : S1_intersect_S2) {
        myfile << i.board[0] << " " << i.board[1] << " " << i.board[2] << endl;
        myfile << i.board[3] << " " << i.board[4] << " " << i.board[5] << endl;
        myfile << i.board[6] << " " << i.board[7] << " " << i.board[8] << endl;
        myfile << endl;
    }

    myfile.close();

    cout << "******* 4a **************************************" << "\n" << endl;
    cout << "state space for S1 is stored in 'S1_states.txt'" << "\n" << endl;


    cout << "******* 4b **************************************" << "\n" << endl;
    cout << "S1 size: " << state_space.size() << "\n" << endl;


    cout << "******* 4c **************************************" << "\n" << endl;
    cout << "state space for S2 is stored in 'S1_states.txt'" << "\n" << endl;


    cout << "******* 4d **************************************" << "\n" << endl;
    cout << "S2 size: " << state_space2.size() << endl << endl;


    cout << "******* 4e **************************************" << "\n" << endl;
    cout << "state space for S1 intersection S2 is stored in 'S1_intersection_S2.txt'" << "\n" << endl;


    cout << "******* 4f **************************************" << "\n" << endl;
    cout << "S1 intersection S2 size: " << S1_intersect_S2.size() << "\n" << endl;

    cout << "PRESS ENTER TO EXIT....";
    cin.ignore();


}