import pandas as pd
import matplotlib.pyplot as plt

def plot_fitness_evolution(csv_path):
    data = pd.read_csv(csv_path)
    
    plt.figure(figsize=(10, 6))
    plt.plot(data["generation"], data["best"], label="Best")
    plt.plot(data["generation"], data["current"], label="Current")
    plt.plot(data["generation"], data["avg"], label="Average")
    plt.plot(data["generation"], data["worst"], label="Worst")
    
    plt.xlabel("Generation")
    plt.ylabel("Fitness Value")
    plt.title("Fitness Evolution Across Generations")
    plt.legend()
    plt.grid(True)
    
    plt.show()

num='7'
plot_fitness_evolution('C:\\Users\\szymo\\CLionProjects\\Metaheurystyki_cVRP\\summaries\\tabu7.csv')