import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys

sns.set()
data = pd.read_csv(sys.argv[1])
args = {k:v for k,v in [arg.split("=") for arg in sys.argv[1].split("+")]}


def main():
    fitness = data[["Minimum Fitness", "Maximum Fitness", "Mean Fitness"]]
    ax = sns.lineplot(data=fitness)
    set_labels("Fitness")
    plt.show()

    sense = data[["Minimum Sense Count", "Maximum Sense Count", "Mean Sense Count"]]
    ax = sns.lineplot(data=sense)
    set_labels("Senses")
    plt.show()

    fun_calls = data[["Minimum Function Calls", "Maximum Function Calls", "Mean Function Calls"]]
    ax = sns.lineplot(data = fun_calls)
    set_labels("Function Calls")
    plt.show()

    fun_forks = data[["Minimum Function Forks", "Maximum Function Forks", "Mean Function Forks"]]
    ax = sns.lineplot(data = fun_forks)
    set_labels("Function Forks")
    plt.show()

    fun_calls_total = data[["Minimum Function Calls & Forks", "Maximum Function Calls & Forks", "Mean Function Calls & Forks"]]
    ax = sns.lineplot(data = fun_calls_total)
    set_labels("Calls & Forks")
    plt.show()

    for i in range((len(data.columns) - 15) // 3):
        guess = data[[F"Minimum Guess{i} Count", F"Maximum Guess{i} Count", F"Mean Guess{i} Count"]]
        ax = sns.lineplot(data=guess)
        set_labels(F"Guess {i} Count")
        plt.show()

def set_labels(y):
    plt.title(F"treatment:{args['treatment']} Seqs:{args['SEQ_A']}/{args['SEQ_B']} Seed:{args['seed']}")
    plt.xlabel("Generations")
    plt.ylabel(y)

if __name__ == "__main__":
    main()
