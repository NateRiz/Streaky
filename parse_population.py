import os
import parse_results

class Program:
    def __init__(self, program, data, name):
        self.name = name
        with open(program, "r") as p:
            self.program = p.readlines()
        self.instructions = {}
        self.fun_count = 0
        for line in self.program:
            line = line.strip().split()
            if line:
                inst = line[0].lower()
                if "fn" in inst:
                    self.fun_count+=1
                    continue    
                if inst not in self.instructions:
                    self.instructions[inst]=0
                self.instructions[inst]+=1

        self.median_fitness = -1
        with open(data, "r") as d:
            for line in d:
                self.median_fitness = line.strip().split(",")[7]
                
        self.data = data
        self.__repr__ = self.__str__

    def __str__(self):
        return F"({self.fun_count:<2}, {len(self.program)-self.fun_count:<3}) | {self.median_fitness:<3} | {self.name}"
        


def get_data():
    data_path = os.path.join(os.getcwd(), "data")
    results = [[],[]]
    for r,d,f in os.walk(data_path):
        for path in d:
            if "streaky-evolve" not in path:
                continue
            treatment = 0
            if "roulette" in path: treatment = 1
            files = os.listdir(os.path.join(data_path, path))
            prg=None
            data=None
            for file in files:
                if "csv" in file:
                    data = os.path.join(data_path, path, file)
                elif "program" in file:
                    prg = os.path.join(data_path, path, file)
            results[treatment].append(Program(prg, data, path))
    return results

def filter_on(instruction, results):
    filtered = []
    for i in range(len(results)):
        for j, result in enumerate(results[i]):
            if instruction in result.instructions:
                filtered.append(i*20+j)
    print(" ".join(str(i) for i in filtered))
    return filtered

def print_results(results, history):
    i=0
    print(F"Size: (Fx, Inst)\tHistory:")
    for i, r in enumerate(results[0]+results[1]):
        if not i: print("==RANKED==")
        if i==len(results[0]): print("==ROULETTE==")
        print(F"{i:>2}: {str(r):<24}\t{history[i] if i < len(history) else '-'}")
        i+=1
    
    print("______________________________\n\n")




def main():
    results = get_data()
    history = []
    while 1:
        print_results(results, history)
        choice = ""
        while not choice.isdigit():
            choice = input("{id} || [F]ilter {Instruction}")
            if choice and choice.lower()[0]=="f":
                filter_on(choice.lower().split()[1], results)

        pop = int(choice)
        if pop >= len(results[0]):
            pop-=len(results[0])
            selected = results[1][pop]
        else:
            selected = results[0][pop]
        
        history.insert(0, pop)
        if len(history)>len(results[0]):
            history.pop()
        print("".join(selected.program))
        graph = parse_results.Graph(selected.data)
        graph.start()

        




if __name__ == "__main__":
    main()
