hand = ['c', 'a', 't', 's']

import pickle
class node:
    def __init__(self, parent, letter):
        self.parent = parent
        self.children = {}
        self.letter = letter
with open('gameplay/dictionary.pickle', 'rb') as f:
    dictionary = pickle.load(f)

words = []
remaining_hand = hand
while(remaining_hand):
    for letter in remaining_hand:
        