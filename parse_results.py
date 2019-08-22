import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys
from keyname import keyname as kn 

sns.set()
data = pd.read_csv(sys.argv[1])
args = kn.unpack(sys.argv[1])
def main():
    labels = ["Fitness","Sense Count", "Guess0 Count", "Guess1 Count"]
    means = ["Function Calls", "Function Forks", "Function Calls & Forks"]
    fig = plt.figure()
    fig.subplots_adjust(hspace=0.4, wspace=0.4)
    results = []
    i=0
    for label in labels:
        i+=1
        ax=fig.add_subplot(3, 3, i)
        results.append(data[[f"Minimum {label}", f"Maximum {label}", f"Mean {label}", f"Median {label}"]])
        sns.lineplot(data=results[i-1], ax=ax)
        set_labels(label)

    for label in means:
        i+=1
        ax=fig.add_subplot(3, 3, i)
        results.append(data[[f"Mean {label}"]])
        sns.lineplot(data=results[i-1], ax=ax)
        set_labels(label)
        
    plt.show()
    
def set_labels(y):
    plt.title(F"treatment:{args['treatment']} Seqs:{args['SEQ_A']}/{args['SEQ_B']} Seed:{args['seed']}")
    plt.xlabel("Generations")
    plt.ylabel(y)

if __name__ == "__main__":
    main()
