import re
from collections import defaultdict

class Iqfit:

    def read_solutions(self, filename):
        self.solutions = []
        with open(filename, "rt") as f:
            for line in f:
                line = line.strip()
                self.solutions.append(line)
        self.pieces = sorted(list(set(self.solutions[0])))
        print(len(self.solutions), self.pieces)


    def count_solutions(self):
        self.piece_solutions = []
        for piece in self.pieces:
            pattern = "[^" + piece + "]"
            regex = re.compile(pattern)
            for solution in self.solutions:
                solution = regex.sub("_", solution)
                self.piece_solutions.append(solution)

        self.solution_count = defaultdict(int)

        for piece_solution in self.piece_solutions:
            self.solution_count[piece_solution] += 1
        
        self.solution_count = sorted(self.solution_count.items(), key=lambda x: x[1], reverse=True)

        print("most solutions for a single piece")
        for solution in self.solution_count[:6]:
            self.print_solution(solution[0], solution[1])
        print("least solutions for a single piece")
        for solution in self.solution_count[-6:]:
            self.print_solution(solution[0], solution[1])

    def print_solution(self, solution, title):
        pos = 0
        print(title)
        for c in solution:
            pos += 1
            print(" " + c, end="")
            if pos % 10 == 0: print("")

def main():
    iqfit = Iqfit()
    iqfit.read_solutions("solutions.txt")

    iqfit.count_solutions()
    
if __name__ == "__main__":
    main()
