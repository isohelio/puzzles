import re, argparse, sys, math
from collections import defaultdict
import png

pieces = ['B', 'C', 'G', 'O', 'P', 'R', 'Y', 'b', 'g', 'p']
terminal_colours = {'B': 34, 'C': 96, 'G': 32, 'O': 33, 'P': 35, 'R': 31, 'Y': 93, 'b': 94, 'g': 92, 'p': 95}
rgb_colours = { 'B': [0, 0, 255], 'C': [0, 255, 255], 'G': [0, 200, 0], 'O': [255, 165, 0], 'P': [91, 64, 96],
                'R': [255, 0, 0], 'Y': [255, 255, 0], 'b': [130, 130, 255], 'g': [100, 255, 100], 'p': [252, 45, 222],
                '_': [245, 245, 245], '-': [250, 250, 250]}

class Iqfit:

    def read_solutions(self, filename):
        self.solutions = []
        with open(filename, "rt") as f:
            for line in f:
                line = line.strip()
                self.solutions.append(line)

                # self.rotate_180(line)

        # self.solutions = self.solutions[:10000]
        self.pieces = pieces
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

    def get_three_piece_solutions(self):
        solutions = []
        for i, piece1 in enumerate(self.pieces):
            for j, piece2 in enumerate(self.pieces):
                if j < i:
                    for k, piece3 in enumerate(self.pieces):
                        if k < j:
                            pattern = "[^" + piece1 + piece2 + piece3 + "]"
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
        if c in terminal_colours: return "\033[%sm%s\033[0m" % (terminal_colours[c], c)
        else: return "-"


    def print_solution(self, solution, title):
        pos = 0
        print(title)
        for c in solution[:50]:
            pos += 1
            print(" " + self.format_cell(c), end="")
            if pos % 10 == 0: print("")


    def write_solutions(self, solutions, filename):
        with open(filename, "w") as f:
            for solution in solutions:
                write = True
                if self.args.dedup:
                    rotated = self.rotate_180(solution[0])
                    if rotated < solution[0]: write = False
                # print(write)
                if write:
                    f.write("%s %s\n" % (solution[0], str(solution[1])))


    def rotate_180(self, solution):
        rotated = ""
        pos = 50
        for y in range(0, 5):
            for x in range(0, 10):
                pos -= 1
                c = solution[pos]
                rotated += c

        # self.print_solution(solution, "original")
        # self.print_solution(rotated, "rotated")
        
        return rotated

    def get_xy(self, s, c, r, x, y):
        column = s % self.columns
        row = s // self.columns

        x = column * (10 + 1) + 1 + x
        y = row * (5 + 1) + 1 + y
        return (x, y)


    def set_pixel(self, s, c, r, x, y, rgb):
        xy = self.get_xy(s, c, r, x, y)
        pos = xy[0] + self.image_width * xy[1]
        self.buffer[pos] = rgb


    def configure_image(self, solutions, columns, gap):
        rows = math.ceil(len(solutions) / columns)
        self.gap = gap
        self.columns = columns
        self.rows = rows
        self.image_width = (10 + 1) * columns + 1
        self.image_height = (5 + 1) * rows + 1
        self.buffer = [0] * self.image_width * self.image_height

        self.palette = [[255, 255, 255] for i in range(0, 256)]
        for c in rgb_colours:
            self.palette[ord(c[0])] = rgb_colours[c]


    def write_image(self, solutions, filename, columns=100, gap=2):
        self.configure_image(solutions, columns, gap)
        print(len(solutions), self.rows, self.image_width, self.image_height)
        f = open(filename, 'wb')

        for s, solution in enumerate(solutions):
            pos = 0
            for y in range(0, 5):
                for x in range(0, 10):
                    c = solution[pos]
                    if (x + y) % 2 == 0 and c == '_': c = '-' 
                    self.set_pixel(s, 0, 0, x, y, ord(c))
                    pos += 1

        png.Writer(self.image_width, self.image_height, palette=self.palette).write_array(f, self.buffer)


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
        parser.add_argument("--dedup", action='store_true', help="Deduplicate", default=False)

        parser.add_argument("--format", action='store_true', help="File to format.")
        parser.add_argument("--two", action='store_true', help="Produce two piece single solution puzzles.")
        parser.add_argument("--three", action='store_true', help="Produce three piece single solution puzzles.")
        parser.add_argument("--image", action='store_true', help="File to generate image for.")
        parser.add_argument("--input", action='store', help="Input file.", default="solutions.txt")
        parser.add_argument("--output", action='store', help="Output file for image generation.")

        self.args = parser.parse_args(argv)
        print(self.args)
        if self.args.format:
            self.read_solutions(self.args.input)

            self.print_solutions(self.solutions, self.args.columns, self.args.max)
        elif self.args.image:
            if not self.args.output: self.args.output = "solutions.png"
            self.read_solutions(self.args.input)

            self.write_image(self.solutions, self.args.output, columns=self.args.columns)
        elif self.args.three:
            if not self.args.output: self.args.output = "three_piece_solutions.txt"
            self.read_solutions(self.args.input)

            three_piece_solutions = self.get_three_piece_solutions()
            print(three_piece_solutions[:10])
            three_piece_solutions = self.count_solutions(three_piece_solutions)
            
            three_piece_single_solutions = [s for s in three_piece_solutions if s[1] == 1]

            print(len(three_piece_single_solutions))

            self.write_solutions(three_piece_single_solutions, self.args.output)
        elif self.args.two:
            if not self.args.output: self.args.output = "two_piece_solutions.txt"
            self.read_solutions(self.args.input)

            two_piece_solutions = self.get_two_piece_solutions()
            print(two_piece_solutions[:10])
            two_piece_solutions = self.count_solutions(two_piece_solutions)
            
            two_piece_single_solutions = [s for s in two_piece_solutions if s[1] == 1]

            print(len(two_piece_single_solutions))

            self.write_solutions(two_piece_single_solutions, self.args.output)





def main():
    iqfit = Iqfit()

    iqfit.process_arguments(sys.argv[1:])
    # iqfit.read_solutions("solutions.txt")

    # iqfit.generate_solutions()
    
if __name__ == "__main__":
    main()
