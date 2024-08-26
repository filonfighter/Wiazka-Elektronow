import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.pyplot as plt


fig, axs = plt.subplots(figsize=(12,4))
df = pd.read_csv('braggs_curve.txt', sep='\t', header='infer')
df.plot(kind='line', x=df.columns[0], y=df.columns[1],ax=axs)

plt.show(block=True)
