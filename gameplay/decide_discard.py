hand = ('c', 'a', 't', 's')

import pickle
class node:
    def __init__(self, parent, letter):
        self.parent = parent
        self.children = {}
        self.letter = letter
with open('gameplay/dictionary.pickle', 'rb') as f:
    root_node = pickle.load(f)

values = {"a":2, "b":8, "c":8, "d":5, "e":2, "f":6, "g":6, "h":7, "i":3, "j":13, "k":8, "l":3, "m":5, "n":5, "o":2, "p":6, "q":15, "r":5, "s":3, "t":3, "u":4, "v":11, "w":10, "x":12, "y":4, "z":14, " ":0}
hands = []
current_node = root_node

def compute_hand(current_words, remaining_hand, current_node):
    if len(remaining_hand) > 1:
        for letter in remaining_hand:
            if letter in current_node.children:
                new_current_words = current_words + (letter,)
                new_remaining_hand = tuple(remaining_hand[i] for i in range(len(remaining_hand)) if i != remaining_hand.index(letter))
                if "NULL" in current_node.children[letter].children:
                    hands.append((new_current_words, new_remaining_hand))
                    compute_hand(new_current_words + (" ",), new_remaining_hand, root_node)
                compute_hand(new_current_words, new_remaining_hand, current_node.children[letter])

def evaluate_hand(hand):
    return max([0, sum([values[l] for l in hand[0]]) - sum([values[l] for l in hand[1]]) + max([values[l] for l in hand[1]])])


compute_hand((), hand, root_node)
hand_values = [evaluate_hand(hand) for hand in hands]
leftover = hands[hand_values.index(max(hand_values))][1]
leftover_values = [values[letter] for letter in leftover]
discard = leftover[leftover_values.index(max(leftover_values))]
print(discard)
leftover_without_discard = list(hands[hand_values.index(max(hand_values))][1])
leftover_without_discard.remove(discard)
final_hand = list(hands[hand_values.index(max(hand_values))][0]) + leftover_without_discard
print(''.join(final_hand))