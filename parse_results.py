import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys

sns.set()
data = pd.read_csv(sys.argv[1])
args = {k:v for k,v in [arg.split("=") for arg in sys.argv[1].split("+")]}


def main():
    fig = plt.figure()
    fig.subplots_adjust(hspace=0.4, wspace=0.4)

    ax=fig.add_subplot(3,3,1)
    fitness = data[["Minimum Fitness", "Maximum Fitness", "Mean Fitness"]]
    sns.lineplot(data=fitness, ax=ax)
    set_labels("Fitness")
    
    ax=fig.add_subplot(3,3,2)
    sense = data[["Minimum Sense Count", "Maximum Sense Count", "Mean Sense Count"]]
    sns.lineplot(data=sense, ax=ax)
    set_labels("Senses")

    ax=fig.add_subplot(3,3,3)
    fun_calls = data[["Minimum Function Calls", "Maximum Function Calls", "Mean Function Calls"]]
    sns.lineplot(data = fun_calls, ax=ax)
    set_labels("Function Calls")

    ax=fig.add_subplot(3,3,4)
    fun_forks = data[["Minimum Function Forks", "Maximum Function Forks", "Mean Function Forks"]]
    sns.lineplot(data = fun_forks, ax=ax)
    set_labels("Function Forks")

    ax=fig.add_subplot(3,3,5)
    fun_calls_total = data[["Minimum Function Calls & Forks", "Maximum Function Calls & Forks", "Mean Function Calls & Forks"]]
    sns.lineplot(data = fun_calls_total, ax=ax)
    set_labels("Calls & Forks")

    ax=fig.add_subplot(3,3,6)
    guess0 = data[["Minimum Guess0 Count", "Maximum Guess0 Count", "Mean Guess0 Count"]]
    sns.lineplot(data = guess0, ax=ax)
    set_labels("Guess 0 Count")

    ax=fig.add_subplot(3,3,7)
    guess1 = data[["Minimum Guess1 Count", "Maximum Guess1 Count", "Mean Guess1 Count"]]
    sns.lineplot(data = guess1, ax=ax)
    set_labels("Guess 1 Count")
    
    plt.show()
    
def set_labels(y):
    plt.title(F"treatment:{args['treatment']} Seqs:{args['SEQ_A']}/{args['SEQ_B']} Seed:{args['seed']}")
    plt.xlabel("Generations")
    plt.ylabel(y)

if __name__ == "__main__":
    main()
