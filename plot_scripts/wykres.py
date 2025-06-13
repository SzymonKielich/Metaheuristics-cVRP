import pandas as pd
import matplotlib.pyplot as plt
import os
import subprocess

cross = "CX"
mut = "inv 0.4"
problem_file = 3
files = [
    "toy.vrp",
    "A-n32-k5.vrp",
    "A-n37-k6.vrp",
    "A-n39-k5.vrp",
    "A-n45-k6.vrp",
    "A-n48-k7.vrp",
    "A-n54-k7.vrp",
    "A-n60-k9.vrp"
]
population = 500
generations = 100
cprob = 0.8
tourSize = 15

tmp = subprocess.call(f"./wykres.exe {problem_file} {population} {generations} {cross} {cprob} {mut} {tourSize}")

file_path = f'outputs\\{cross}{cprob}{mut.replace(" ", "")}output.csv'

df = pd.read_csv(file_path)

plt.figure(figsize=(10, 6))
plt.plot(df['generation'], df['best'], label='Best')
plt.plot(df['generation'], df['avg'], label='Average')
plt.plot(df['generation'], df['worst'], label='Worst')

plt.title(f'{cross} {cprob} {mut} fitness: {tmp}, popS: {population}, gen: {generations}')
plt.xlabel('Generation')
plt.ylabel('Fitness')
plt.legend()
plt.grid(True)

output_image_path = f'graphs\\{cross}_{mut.replace(" ", "_")}_plot{population}x{generations}.png'
plt.savefig(output_image_path)
plt.close()
