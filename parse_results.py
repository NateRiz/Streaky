import matplotlib.pyplot as plt

class Data:
    def __init__(self, min_, max_, mean):
        self.min_ = min_
        self.max_ = max_
        self.mean = mean

sense = []
fitness = []
guess = []


with open("results.dat", "r") as file:
    file.readline()
    for line in file.readlines():
        line = line.strip().split(",")
        line = [float(x) for x in line]
        sense.append(Data(line[0], line[1], line[2]))
        fitness.append(Data(line[3], line[4], line[5]))
        line = line[6::]
        for i in range(len(line)//3):
            if len(guess) <= i:
                guess.append([])
            guess[i].append(Data(line[3*i + 0], line[3*i + 1], line[3*i + 2]))

plt.title("Fitness Count")
plt.xlabel("Generation")
plt.ylabel("Fitness")
plt.fill_between(list(range(len(fitness))), [x.min_ for x in fitness], [min(x.max_,100) for x in fitness])
plt.show()

plt.title("Sense Count")
plt.xlabel("Generation")
plt.ylabel("Senses")
plt.fill_between(list(range(len(sense))), [x.min_ for x in sense], [x.max_ for x in sense])
plt.show()

for i, g in enumerate(guess):
    plt.title(F"Guess {i} Count")
    plt.xlabel("Generation")
    plt.ylabel("Guesses")
    plt.fill_between(list(range(len(g))), [x.min_ for x in g], [x.max_ for x in g])
    plt.show()
