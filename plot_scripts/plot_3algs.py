import pandas as pd
import matplotlib.pyplot as plt

def plot_algorithm_progress():
    ga_log = pd.read_csv('summaries/ga_progress.csv')
    sa_log = pd.read_csv('summaries/sa_progress.csv')
    ts_log = pd.read_csv('summaries/tabu_progress.csv')

    plt.figure(figsize=(10, 6))

    plt.plot(ga_log['generation'], ga_log['best'], label='Genetic Algorithm (Best Fitness)')

    plt.plot(sa_log['generation'], sa_log['best'], label='Simulated Annealing (Current Fitness)')

    plt.plot(ts_log['generation'], ts_log['best'], label='Tabu Search (Current Fitness)')

    plt.xlabel('Iteration/Generation')
    plt.ylabel('Fitness')
    plt.title('Algorithm Progress Comparison')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('outputs/algorithm_progress_comparison.png')
    plt.show()

if __name__ == "__main__":
    plot_algorithm_progress()
