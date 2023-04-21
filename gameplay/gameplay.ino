#include <map>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
// using namespace std; might include this

const int NUM_PILES = 3;
int current_pile = 1;

class hand {
  public:

  Hand() {
    // one way in which we store the hand. each row of this vector is a pile number, 
    // where the 0th row is pile 1, and each col is an order within the pile, where 
    // the first 0th element is the top card on the pile
    vector<vector<string>> cards = {{},
                                    {},
                                    {}};
    this->cards = cards;
  }

  // return the pile number that a letter is in
  // if the letter is in the hand multiple times, returns an arbitrary pile
  int find_pile_num(string letter) {
    for (int row = 0; row < NUM_PILES; row++) {
      for (int col = 0; col < (this->cards)[row].size(); col++) {
        if ((this->cards)[row][col] == letter) {
          return row + 1;
        }
      }
    }
  }

  // return the number that a card is in a pile, 
  // for example returns 2 if a card is second from the top of a pile
  int find_order_num(string letter, int pile) {
    for (int col = 0; col < (this->cards)[pile].size(); col++) {
      if ((this->cards)[row][col] == letter) {
          return col + 1;
        } 
    }
  }

  // move a card to the discard pile
  void discard(string letter) {

    int row = find_pile_num(letter);
    int col = find_order_num(letter);

    current_pile = this->get_shortest_pile();
    for (int i = 1; i < col; i++) {
      move_card(row, current_pile);
      current_pile = this->get_shortest_pile(exclude = row);
    }

    move_card(row, 0);
  }

  void rebalance_piles() {
    
  }

  int get_shortest_pile(int exclude=0) {
    if (exclude == 1) {
      if ((this->cards)[2].size() <= (this->cards)[3].size()) {
        return 2;
      }
      else {
        return 3;
      }
    }
    if (exclude == 2) {
      if ((this->cards)[1].size() <= (this->cards)[3].size()) {
        return 1;
      }
      else {
        return 3;
      }
    }
    if (exclude == 3) {
      if ((this->cards)[1].size() <= (this->cards)[2].size()) {
        return 1
      }
      else {
        return 2
      }
    }
    else {
      if ((this->cards)[1].size() <= (this->cards)[2].size() && (this->cards)[1].size() <= (this->cards)[3].size()) {
        return 1;
      }
      else if ((this->cards)[2].size() <= (this->cards)[1].size() && (this->cards)[2].size() <= (this->cards)[3].size()) {
        return 2;
      }
      else {
        return 3;
      }
    }
  }

  void move_card(int start_pile, int end_pile) {

    string letter = (this->cards)[start_pile][end_pile];
    // TODO: add mechanical movement

    (this->cards)[start_pile].erase((this->cards).begin());
    if (end_pile != 0) {
      (this->cards)[end_pile].insert((this->cards).begin(), letter);
    }
  }

  string scan_card(int end_pile) {
    string scanned_letter = "A"; // repalce with actual CV
    // TODO: add mechanical movement

    (this->cards)[end_pile].insert((this->cards).begin(), letter);
    return scanned_letter;
}

}

void move_card(int start_pile, int end_pile) {
  return;
}

string scan_card(int end_pile) {
  return "A";
}

void take_photo() {
  return;
}

void setup() {
  Serial.begin(9600);
}


void loop() {
  /*
  This current script implements one turn of gameplay for QuiddlerBot, the hand of 6. 
  It is not implemented to be able to go out by itself, but it will go out when it must.
  */
  hand_size = 6;

  // Input the command DEALT once the cards have been placed in the input tray
  while(true) {
    while (Serial.available() == 0) {} //wait for user input
    string input = Serial.read();
    if (input == "DEALT") {
      break; // move to next state when cards have been dealt
    }
  }

  // scan in the initally dealt hand
  for (int i = 1; i <= hand_size; i++) {
    string card = scan_card(current_pile);
    // TODO: add logic to track the card
    current pile = (current_pile + 1) % NUM_PILES;
  }

  // When it's QuiddlerBot's turn, input the name of the card it is dealt, for example "A"
  // If QuiddlerBot must go out, put an asterisk in front of the card name, for example "*A"
  while (Serial.available() == 0) {}
  string input_card = Serial.read();
  // TODO: add logic to do something with the previously discarded card
  system("bash -c \"py decide_draw.py > notepad.txt\""); // decide_draw.py runs, right now without arguments. It dumps its output in notepad.txt
  ifstream f; 
  f.open("notepad.txt"); // open the notepad file
  string move;
  f >> move;
  f.close()

  // Quiddlerbot either asks for the DRAWPILE or DISCARDPILE
  if (move == "DRAWPILE") {
    Serial.println("Draw from DRAWPILE");
  }
  else if (move == "DISCARDPILE") {
    Serial.println("Draw from DISCARDPILE");
  }

  // scan new card
  string card = scan_card(current_pile);
  current pile = hand->get_shortest_pile();

  // decide what to discard
  system("bash -c \"py decide_discard.py > notepad.txt\""); // prints two lines to the notepad: the card to discard and then the best current arrangement
  ifstream f; 
  f.open("notepad.txt"); // open the notepad file
  string card_to_discard;
  string word[hand_size];
  f >> card_to_discard;
  for (int i=0; i<hand_size; i++) {
    f >> word[i];
  }
  f.close()

  // discard
  hand->discard(card_to_discard)

  // user must remove card from output tray, and then input DISCARDED
  while(true) {
    while (Serial.available() == 0) {} // wait for user input
    string input = Serial.read();
    if (input == "DISCARDED") {
      break; // move to next state when card has been removed from the output tray
    }
  }

  // go out if required
  if (input_card.substring(0,1) == "*") {
    for (int i=0; i<hand_size; i++) {
      hand->discard(word[i])
    }
  }


}