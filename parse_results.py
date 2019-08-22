import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys
from keyname import keyname as kn 
sns.set()

class Graph:
    def __init__(self, csv_path):
        self.data = pd.read_csv(csv_path)
        self.args = kn.unpack(csv_path)
    
    def start(self):
        labels = ["Fitness","Sense Count", "Guess0 Count", "Guess1 Count"]
        means = ["Function Calls", "Function Forks", "Function Calls & Forks"]
        fig = plt.figure()
        fig.subplots_adjust(hspace=0.4, wspace=0.4)
        results = []
        i=0
        for label in labels:
            i+=1
            ax=fig.add_subplot(3, 3, i)
            results.append(self.data[[f"Minimum {label}", f"Maximum {label}", f"Mean {label}", f"Median {label}"]])
            sns.lineplot(data=results[i-1], ax=ax)
            self.set_labels(label)

        for label in means:
            i+=1
            ax=fig.add_subplot(3, 3, i)
            results.append(self.data[[f"Mean {label}"]])
            sns.lineplot(data=results[i-1], ax=ax)
            self.set_labels(label)
            
        plt.show()
        
    def set_labels(self, y):
        plt.title(F"treatment:{self.args['treatment']} Seqs:{self.args['SEQ_A']}/{self.args['SEQ_B']} Seed:{self.args['seed']}")
        plt.xlabel("Generations")
        plt.ylabel(y)

if __name__ == "__main__":
    graph = Graph(sys.argv[1])
    graph.start()
