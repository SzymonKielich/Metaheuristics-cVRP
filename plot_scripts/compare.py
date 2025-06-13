import pandas as pd 
import matplotlib.pyplot as plt
import os
import subprocess

crossovers = ["OX", "PMX", "CX"]
mutators = ["inv 0.1"]
iterations =5
problem_file = 4
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
population = 100
generations = 300
cprobs = [0.1, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
mprob = 0.1
turSizes = [5]


for mut in mutators:
    for cross in crossovers:
        for cprob in cprobs:
            for turSize in turSizes:
                    tmp = subprocess.call(f"./refactor.exe {problem_file} {population} {generations} {cross} {cprob} {mut} {turSize} {iterations}")

                # file_path = f'{cross}{cprob}{mut.replace(" ", "")}output.csv'

                # print(tmp)
                # df = pd.read_csv(file_path)

                # plt.figure(figsize=(10, 6))
                # plt.plot(df['generation'], df['best'], label='Best')
                # plt.plot(df['generation'], df['avg'], label='Average')
                # plt.plot(df['generation'], df['worst'], label='Worst')

                # plt.title(f'{cross} {cprob} {mut} avgfit {tmp}')
                # plt.xlabel('Generation')
                # plt.ylabel('Fitness')
                # plt.legend()
                # plt.grid(True)

                # output_image_path = f'{cross}_{mut.replace(" ", "_")}_plot.png'
                # plt.savefig(output_image_path)
                # plt.close()
