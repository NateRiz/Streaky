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

fun_calls = data[["Min Fx Calls", "Max Fx Calls", "Mean Fx Calls"]]
ax = sns.lineplot(data = fun_calls)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

fitness = data[["Minimum Fitness", "Maximum Fitness", "Mean Fitness"]]
ax = sns.lineplot(data=fitness)
plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.show()

for i in range((len(data.columns) - 9) // 3):
    guess = data[[F"Minimum Guess{i} Count", F"Maximum Guess{i} Count", F"Mean Guess{i} Count"]]
    ax = sns.lineplot(data=guess)
    plt.xlabel("Generations")
    plt.ylabel(F"Guess {i} Count")
    plt.show()
