import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.pyplot as plt


fig, axs = plt.subplots(figsize=(12,4))
df = pd.read_csv(r"", sep='\t', header='infer') # Add the path to the file
df.plot(kind='line', x=df.columns[0], y=df.columns[1],ax=axs)

plt.show(block=True)
