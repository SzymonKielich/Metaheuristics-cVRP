import pandas as pd 
import matplotlib.pyplot as plt
import os
import subprocess

crossovers = ["OX", "PMX", "CX"]
mutators = ["inv 0.01", "inv 0.1","inv 0.2", "swap 0.001", "inv 0.4", "inv 0.6"]
iterations = 4
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
pop_gen = ["100 500", "500 100", "250 200" ]
population = 100
generations = 200
cprobs = [0.3,0.5,0.8,0.9]
mprob = 0.01
turSizes = [5,10,15, 20]


for mut in mutators:
    print(mut)
    for cross in crossovers:
        for cprob in cprobs:
            for turSize in turSizes:
                for popgen in pop_gen:
                    tmp = subprocess.call(f"./refaktorr.exe {problem_file} {popgen} {cross} {cprob} {mut} {turSize} {iterations}")

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
