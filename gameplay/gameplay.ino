#include <map>
#include <string>
#include <String>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
// using namespace std; might include this

const int NUM_PILES = 2;
int current_arm_loc = 0;
// Physical constants
const int dirPin = 26;
const int stepPin = 25;
const int stepsPerRevolution = 200;
const int lin1 = 17;
const int lin2 = 16;
const int noid = 19;

std::string decide_draw() {
  return "DRAWPILE";
}

std::string decide_discard() {
  return "A";
}

void move_card_physical(int start_pile, int end_pile) {
  move_arm_physical(current_arm_loc, start_pile);
  // pick up card
  // extend lin act
  delay(100);
  digitalWrite(lin1, HIGH);
  digitalWrite(lin2, LOW);
  delay(6300);
  // hold linact position
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, LOW);
  delay(1000);
  // retract linact
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, HIGH);
  delay(6300);
  // hold linact position
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, LOW);
  delay(100);

  move_arm_physical(start_pile, end_pile);
  // put down card
  digitalWrite(noid, HIGH);
  delay(500);
  digitalWrite(noid, LOW);
  // home the arm
}

void move_arm_physical(int start_pile, int end_pile) {
  if (start_pile == end_pile) {
    return;
  }
  if (start_pile == 0 && end_pile == 1) {
    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(500);
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  } else if (start_pile == 0 && end_pile == 2) {
    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(500);
    for (int x = 0; x < 800; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  } else if (start_pile == 1 && end_pile == 2) {
    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  } else if (start_pile == 1 && end_pile == 0) {
    digitalWrite(dirPin, LOW);
    for (int x = 0; x < 800; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  } else if (start_pile == 2 && end_pile == 1) {
    digitalWrite(dirPin, LOW);
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  } else if (start_pile == 2 && end_pile == 0) {
    digitalWrite(dirPin, LOW);
    for (int x = 0; x < 1200; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
  }
  current_arm_loc = end_pile;
}


std::string scan_card_physical(int end_pile) {
  Serial.println("What is the top card on my input?");
  while (Serial.available() == 0) {}  // use user input instead of vision
  std::string input = Serial.readStringUntil('\n').c_str();
  move_card_physical(0, end_pile);
  return "A";
}

void take_photo() {
  return;
}





class Hand {

public:

  std::vector<std::vector<std::string>> cards;

  Hand() {
    // one way in which we store the hand. each row of this vector is a pile number,
    // where the 0th row is pile 1, and each col is an order within the pile, where
    // the first 0th element is the top card on the pile
    cards = { {},
              {} };
  }

  // return the pile number that a letter is in
  // if the letter is in the hand multiple times, returns an arbitrary pile
  int find_pile_num(std::string letter) {
    for (int row = 0; row < NUM_PILES; row++) {
      for (int col = 0; col < (this->cards)[row].size(); col++) {
        if ((this->cards)[row][col] == letter) {
          return row + 1;
        }
      }
    }
    return 0;
  }

  // return the number that a card is in a pile,
  // for example returns 2 if a card is second from the top of a pile
  int find_order_num(std::string letter, int pile) {
    for (int col = 0; col < (this->cards)[pile].size(); col++) {
      if ((this->cards)[pile][col] == letter) {
        return col + 1;
      }
    }
    return 0;
  }

  // move a card to the discard pile
  void discard(std::string letter) {

    int row = find_pile_num(letter);
    int col = find_order_num(letter, row);

    for (int i = 1; i < col; i++) {
      move_card(row, get_shortest_pile(row));
    }

    move_card(row, 0);
  }

  int get_shortest_pile(int exclude = 0) {
    if (exclude == 1) {
      return 2;
    }
    if (exclude == 2) {
      return 1;
    } else {
      if ((this->cards)[0].size() <= (this->cards)[1].size()) {
        return 1;
      } else {
        return 2;
      }
    }
  }

  void move_card(int start_pile, int end_pile) {

    // When direction pin is HIGH, and the dropper is facing you ("forward"), then the motor moves left
    std::string letter = (this->cards)[start_pile - 1][(this->cards)[start_pile - 1].size() - 1];
    move_card_physical(start_pile, end_pile);

    (this->cards)[start_pile - 1].erase((this->cards)[start_pile - 1].begin());
    if (end_pile != 0) {
      (this->cards)[end_pile - 1].insert((this->cards)[end_pile - 1].begin(), letter);
    }
  }

  std::string scan_card(int end_pile) {
    std::string scanned_letter = scan_card_physical(end_pile);
    (this->cards)[end_pile - 1].insert((this->cards)[end_pile - 1].begin(), scanned_letter);
    return scanned_letter;
  }
};



void setup() {
  Serial.begin(9600);
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(lin1, OUTPUT);
  pinMode(lin2, OUTPUT);
  pinMode(noid, OUTPUT);
  digitalWrite(noid, LOW);
}

void loop() {
  /*
  This current script implements one turn of gameplay for QuiddlerBot, the hand of 6. 
  It is not implemented to be able to go out by itself, but it will go out when it must.
  */
  int hand_size = 5;  //ATTN

  // home the actuator
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, HIGH);
  delay(7000);
  // hold linact position
  //digitalWrite(lin1, LOW);
  //digitalWrite(lin2, LOW);
  //delay(100);
  // digitalWrite(lin1, HIGH);
  // digitalWrite(lin2, LOW);
  // delay(2000);
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, LOW);
  delay(100);


  // Input the command DEALT once the cards have been placed in the input tray
  while (true) {
    while (Serial.available() == 0) {}  //wait for user input
    std::string input = Serial.readStringUntil('\n').c_str();
    if (input == "DEALT") {
      break;  // move to next state when cards have been dealt
    }
  }

  Hand hand;

  // scan in the initally dealt hand
  for (int i = 1; i <= hand_size; i++) {
    std::string card = hand.scan_card((i % 2) + 1);
  }

  // When it's QuiddlerBot's turn, input the name of the card it is dealt, for example "A"
  // If QuiddlerBot must go out, put an asterisk in front of the card name, for example "*A"
  while (true) {
    Serial.println("What is the top card on the discard pile?");
    while (Serial.available() == 0) {}
    bool must_go_out;
    must_go_out = Serial.readStringUntil('\n').c_str()[0] == '*';
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double rand_num = dis(gen);
    std::string move;
    if (rand_num > 0.7) {  //ATTN
      move = "DRAWPILE";
    } else {
      move = "DISCARDPILE";
    }

    // Quiddlerbot either asks for the DRAWPILE or DISCARDPILE
    if (move == "DRAWPILE") {
      Serial.println("Draw from DRAWPILE");
    } else if (move == "DISCARDPILE") {
      Serial.println("Draw from DISCARDPILE");
    }

    while (true) {
      while (Serial.available() == 0) {}  //wait for user input
      std::string input = Serial.readStringUntil('\n').c_str();
      if (input == "DRAWN") {
        break;  // move to next state when cards have been dealt
      }
    }

    // scan new card

    std::string card = hand.scan_card(hand.get_shortest_pile());

    // make words
    std::string card_to_discard = "A";  //ATTN
    std::string word = "AA";            //ATTN

    // discard
    hand.discard(card_to_discard);

    // user must remove card from output tray, and then input DISCARDED
    while (true) {
      while (Serial.available() == 0) {}  // wait for user input
      std::string input = Serial.readStringUntil('\n').c_str();
      if (input == "DISCARDED") {
        break;  // move to next state when card has been removed from the output tray
      }
    }

    // go out if required
    if (must_go_out) {
      /*      
      for (int i = 0; i < hand_size; i++) {
        hand.discard(std::string(word[i], 1));
      }
      break;
      */
      Serial.println("here");
    }
  }
}

/*

void loop() {
  
  //This current script implements one turn of gameplay for QuiddlerBot, the hand of 6. 
  //It is not implemented to be able to go out by itself, but it will go out when it must.
  
  int hand_size = 6;

  // home the actuator
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, HIGH);
  delay(7000);
  // hold linact position
  //digitalWrite(lin1, LOW);
  //digitalWrite(lin2, LOW);
  //delay(100);
  // digitalWrite(lin1, HIGH);
  // digitalWrite(lin2, LOW);
  // delay(2000);
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, LOW);
  delay(100);
  

  // Input the command DEALT once the cards have been placed in the input tray
  while(true) {
    while (Serial.available() == 0) {} //wait for user input
    std::string input = Serial.readStringUntil('\n').c_str();
    if (input == "DEALT") {
      break; // move to next state when cards have been dealt
    }
  }

  Hand hand;

  // scan in the initally dealt hand
  for (int i = 1; i <= hand_size; i++) {
    std::string card = hand.scan_card((i % 2) + 1); 
  }

  // When it's QuiddlerBot's turn, input the name of the card it is dealt, for example "A"
  // If QuiddlerBot must go out, put an asterisk in front of the card name, for example "*A"
  while (Serial.available() == 0) {}
  std::string input_card = Serial.readStringUntil('\n').c_str();
  // TODO: add logic to do something with the previously discarded card
  system("bash -c \"py decide_draw.py > notepad.txt\""); // decide_draw.py runs, right now without arguments. It dumps its output in notepad.txt
  std::ifstream file1("notepad.txt");
  std::stringstream buffer1;  
  buffer1 << file1.rdbuf();
  std::string move = buffer1.str();

  // Quiddlerbot either asks for the DRAWPILE or DISCARDPILE
  if (move == "DRAWPILE") {
    Serial.println("Draw from DRAWPILE");
  }
  else if (move == "DISCARDPILE") {
    Serial.println("Draw from DISCARDPILE");
  }

  // scan new card
  std::string card = hand.scan_card(hand.get_shortest_pile());

  std::ifstream file2("notepad.txt");
  std::stringstream buffer2; 
  std::stringstream buffer3;  
  buffer2 << file2.rdbuf();
  std::string card_to_discard = buffer2.str();
  std::string word;
  if (!file2.eof()) {
    buffer3 << file2.rdbuf();
    word = buffer3.str(); 
  }


  // discard
  hand.discard(card_to_discard);

  // user must remove card from output tray, and then input DISCARDED
  while(true) {
    while (Serial.available() == 0) {} // wait for user input
    std::string input = Serial.readStringUntil('\n').c_str();
    if (input == "DISCARDED") {
      break; // move to next state when card has been removed from the output tray
    }
  }

  // go out if required
  if (input_card.substr(0,1) == "*") {
    for (int i=0; i<hand_size; i++) {
      hand.discard(std::string(word[i], 1));
    }
  }


}
*/
