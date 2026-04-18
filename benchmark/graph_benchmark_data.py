import argparse
import csv
import matplotlib.pyplot as plt
import numpy as np

def main():
    parser = argparse.ArgumentParser();

    parser.add_argument("file", help="csv file of benchmark data to analyse")

    args = parser.parse_args()

    ilist_random_access = []
    eastl_random_access = []
    eastl_lin_access = []
    ilist_lin_access = []
    element_size = []

    file = args.file

    with open(file, "r") as f:
        reader = csv.DictReader(f)

        for row in reader: 
            print(dict(row))
            
            func: str = row["function"]
            cycles: float = float(row["cycles_per_element"])
            if ("random_access" in func):
                if ("ilist" in func):
                    ilist_random_access.append(cycles)
                    element_size.append(row["element_size"])
                else:
                    eastl_random_access.append(cycles)
            else:
                if ("ilist" in func):
                    ilist_lin_access.append(cycles)
                else:
                    eastl_lin_access.append(cycles)

    

    plt.plot(element_size, eastl_random_access, label="EASTL random")
    plt.plot(element_size, ilist_random_access, label="ilist random")
    plt.plot(element_size, ilist_lin_access, label="ilist linear")
    plt.plot(element_size, eastl_lin_access, label="EASTL linear")
    plt.title("Benchmark for 65535 elements")
    plt.gca().yaxis.set_major_locator(plt.MaxNLocator(5))

    plt.xlabel("element size (bytes)")
    plt.ylabel("cycles/element")

    plt.legend()
    plt.grid()
    plt.show()


if (__name__ == "__main__"):
    main()
