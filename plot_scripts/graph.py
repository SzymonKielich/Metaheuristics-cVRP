import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot_graph(file_path, title):
    data = pd.read_csv(file_path)

    if data.shape[1] < 2:
        print("Plik CSV musi zawierać przynajmniej dwie kolumny.")
        return

    x_column = data.columns[0]
    x_data = data[x_column]

    plt.figure()
    for column in data.columns[1:]:
        if column == "std":
            continue
        y_data = data[column]
        plt.plot(x_data, y_data, label=column)

    plt.xlabel(x_column)
    plt.ylabel("fitness")
    plt.title(title.replace("_", ", "))
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Użycie: python graph.py <ścieżka_do_pliku_csv>")
        sys.exit(1)

    file_path = sys.argv[1]

    title = sys.argv[2]
    plot_graph(file_path, title)
