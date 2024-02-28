import csv
import pickle 

class node:
    def __init__(self, parent, letter):
        self.parent = parent
        self.children = {}
        self.letter = letter

root_node = node(None, "NULL")

with open("dictionary.csv") as f:
    reader = csv.reader(f)
    for row in reader:
        word = row[0]
        current_node = root_node
        for letter in word:
            if letter in current_node.children:
                current_node = current_node.children[letter]
            else:
                current_node.children[letter] = node(current_node, letter)
                current_node = current_node.children[letter]
        current_node.children["NULL"] = node(current_node, "NULL")

with open('gameplay/dictionary.pickle', 'wb') as f:
    pickle.dump(root_node, f)