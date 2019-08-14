import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys

sns.set()
data = pd.read_csv(sys.argv[1])
args = {k:v for k,v in [arg.split("=") for arg in sys.argv[1].split("+")]}

def main():
    labels = ["Fitness","Sense Count", "Function Calls", "Function Forks", "Function Calls & Forks", "Guess0 Count", "Guess1 Count"]
    fig = plt.figure()
    fig.subplots_adjust(hspace=0.4, wspace=0.4)
    results = []
    for i, label in enumerate(labels):
        ax=fig.add_subplot(3, 3, i+1)
        results.append(data[[f"Minimum {label}", f"Maximum {label}", f"Mean {label}", f"Median {label}"]])
        sns.lineplot(data=results[i], ax=ax)
        set_labels(label)

    plt.show()
    
def set_labels(y):
    plt.title(F"treatment:{args['treatment']} Seqs:{args['SEQ_A']}/{args['SEQ_B']} Seed:{args['seed']}")
    plt.xlabel("Generations")
    plt.ylabel(y)

if __name__ == "__main__":
    main()
