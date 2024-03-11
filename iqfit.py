import re, argparse, sys
from collections import defaultdict

colours = {'B': 34, 'C': 96, 'G': 32, 'O': 33, 'P': 35, 'R': 31, 'Y': 93, 'b': 94, 'g': 92, 'p': 95}

class Iqfit:

    def read_solutions(self, filename):
        self.solutions = []
        with open(filename, "rt") as f:
            for line in f:
                line = line.strip()
                self.solutions.append(line)
        self.pieces = sorted(list(set(self.solutions[0])))
        print(len(self.solutions), self.pieces)


    def get_one_piece_solutions(self):
        solutions = []
        for piece in self.pieces:
            pattern = "[^" + piece + "]"
            regex = re.compile(pattern)
            for solution in self.solutions:
                solution = regex.sub("_", solution)
                solutions.append(solution)

        return solutions

    def get_two_piece_solutions(self):
        solutions = []
        for i, piece1 in enumerate(self.pieces):
            for j, piece2 in enumerate(self.pieces):
                if j < i:
                    pattern = "[^" + piece1 + piece2 + "]"
                    regex = re.compile(pattern)
                    for solution in self.solutions:
                        solution = regex.sub("_", solution)
                        solutions.append(solution)

        return solutions

    def get_one_piece_solutions(self):
        solutions = []
        for piece in self.pieces:
            pattern = "[^" + piece + "]"
            regex = re.compile(pattern)
            for solution in self.solutions:
                solution = regex.sub("_", solution)
                solutions.append(solution)

        return solutions

    def count_solutions(self, solutions):
        solution_count = defaultdict(int)

        for piece_solution in solutions:
            solution_count[piece_solution] += 1
        
        solution_count = sorted(solution_count.items(), key=lambda x: x[1], reverse=True)

        return solution_count


    def summarise_solutions(self, solutions):
        print("most solutions for a single piece")
        for solution in solutions[:6]:
            self.print_solution(solution[0], solution[1])

        print("least solutions for a single piece")
        for solution in solutions[-6:]:
            self.print_solution(solution[0], solution[1])


    def print_solutions(self, solutions, columns=4, max=-1):
        start = 0
        n = 0
        if max != -1: solutions = solutions[:max]

        print("")

        for s in range(0, len(solutions), columns):
            chunk = solutions[s:s + columns]
            for solution in chunk:
                n += 1
                print(" %-20d" % n, end="")
            print("")

            for i in range(0, 5):
                for solution in chunk:
                    for j in range(0, 10):
                        c = solution[i * 10 + j]
                        print(" " + self.format_cell(c), end="")
                    print(" ", end="")
                print("")
            print("")
            start += columns

        print("")

    def format_cell(self, c):
        if c in colours: return "\033[%sm%s\033[0m" % (colours[c], c)
        else: return "-"


    def print_solution(self, solution, title):
        pos = 0
        print(title)
        for c in solution:
            pos += 1
            print(" " + self.format_cell(c), end="")
            if pos % 10 == 0: print("")


    def write_solutions(self, solutions, filename):
        with open(filename, "w") as f:
            for solution in solutions:
                f.write("%s %s\n" % (solution[0], str(solution[1])))


    def write_png(self, solutions, filename, width=1920):
        
        pass


    def generate_solutions(self):
        # self.print_solutions(self.solutions[:40])
        # return
        # one_piece_solutions = self.get_one_piece_solutions()

        # one_piece_solutions = self.count_solutions(one_piece_solutions)

        # self.summarise_solutions(one_piece_solutions)

        two_piece_solutions = self.get_two_piece_solutions()
        print(len(two_piece_solutions))
        two_piece_solutions = self.count_solutions(two_piece_solutions)
        
        two_piece_single_solutions = [s for s in two_piece_solutions if s[1] == 1]

        print(len(two_piece_single_solutions))
        self.summarise_solutions(two_piece_single_solutions)
        # self.print_solutions(two_piece_single_solutions)

        self.write_solutions(two_piece_single_solutions, "two_piece_solutions.txt")


    def process_arguments(self, argv):
        parser = argparse.ArgumentParser()

        parser.add_argument("--max", action='store', help="Maximum number of solutions to process", type=int, default=-1)
        parser.add_argument("--columns", action='store', help="Number of columns in formatted output", type=int, default=5)
        parser.add_argument("--format", action='store', help="File to format.")

        self.args = parser.parse_args(argv)

        if self.args.format:
            self.read_solutions(self.args.format)

            self.print_solutions(self.solutions, self.args.columns, self.args.max)

def main():
    iqfit = Iqfit()

    iqfit.process_arguments(sys.argv[1:])
    # iqfit.read_solutions("solutions.txt")

    # iqfit.generate_solutions()
    
if __name__ == "__main__":
    main()
