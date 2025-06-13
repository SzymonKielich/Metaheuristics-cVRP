import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_boxplots_with_optimal_line_and_highlight(data, output_dir='./'):
    os.makedirs(output_dir, exist_ok=True)

    for index, row in data.iterrows():
        file_name = row['Nazwa']
        optimal = row['Optymalny']
        random_best = row['bestRand']

        methods = ['Greedy', 'EA', 'Tabu', 'SA', 'EASex', 'Roulette']
        values = {
            method: {
                'best': row[f'best{method}'],
                'worst': row[f'worst{method}'],
                'avg': row[f'avg{method}'],
                'std': row[f'std{method}'],
            } for method in methods
        }

        boxplot_data = []
        labels = []

        best_overall_value = float('inf')
        best_method_index = -1

        for i, (method, stats) in enumerate(values.items()):
            boxplot_data.append([
                stats['best'],
                stats['avg'] - stats['std'],
                stats['avg'],
                stats['avg'] + stats['std'],
                stats['worst']
            ])
            labels.append(method)

            if stats['best'] < best_overall_value:
                best_overall_value = stats['best']
                best_method_index = i

        fig, ax = plt.subplots(figsize=(10, 6))
        bplot = ax.boxplot(boxplot_data, labels=labels, showmeans=True, meanline=True, patch_artist=True)

        for patch, i in zip(bplot['boxes'], range(len(methods))):
            if i == best_method_index:
                patch.set_facecolor('green')
            else:
                patch.set_facecolor('purple')

        ax.axhline(y=optimal, color='green', linestyle='--', label='Optimal')

        legend_entries = [f'{method}: Best = {values[method]["best"]}' for method in methods]
        legend_entries.insert(0, f'Random: Best = {random_best}')
        ax.legend(legend_entries, loc='upper right')

        ax.set_title(f'Boxplot for {file_name}')
        ax.set_ylabel('Values')
        ax.grid(axis='y')

        # Save the plot
        output_file = os.path.join(output_dir, f'{file_name}_boxplot.png')
        plt.savefig(output_file)
        plt.close()

file_path_xlsx = './boxplot.xlsx' 
data_xlsx = pd.read_excel(file_path_xlsx)

plot_boxplots_with_optimal_line_and_highlight(data_xlsx)

print("Boxplots generated successfully.")
