import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import sys

sns.set()
data = pd.read_csv(sys.argv[1])

sense = data[["Minimum Sense Count", "Maximum Sense Count", "Mean Sense Count"]]
ax = sns.lineplot(data=sense)
plt.xlabel("Generations")
plt.ylabel("Senses")
plt.show()

fun_calls = data[["Minimum Function Calls", "Maximum Function Calls", "Mean Function Calls"]]
ax = sns.lineplot(data = fun_calls)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

fun_forks = data[["Minimum Function Forks", "Maximum Function Forks", "Mean Function Forks"]]
ax = sns.lineplot(data = fun_forks)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

fun_calls_total = data[["Minimum Function Calls & Forks", "Maximum Function Calls & Forks", "Mean Funciton Calls & Forks"]]
ax = sns.lineplot(data = fun_calls_total)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

fitness = data[["Minimum Fitness", "Maximum Fitness", "Mean Fitness"]]
ax = sns.lineplot(data=fitness)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

for i in range((len(data.columns) - 15) // 3):
    guess = data[[F"Minimum Guess{i} Count", F"Maximum Guess{i} Count", F"Mean Guess{i} Count"]]
    ax = sns.lineplot(data=guess)
    plt.xlabel("Generations")
    plt.ylabel(F"Guess {i} Count")
    plt.show()
