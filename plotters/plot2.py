import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Set the font family to Times New Roman
plt.rcParams['font.family'] = 'Times New Roman'

# File paths for the Bragg's curves
file_paths = [
    r"C:\Projekty (Edycja Laptop)\KodynaNagrody\Wiązka Elektronów\Wiazka-Elektronow\data\proton-1\proton_118MeV.txt",
    r"C:\Projekty (Edycja Laptop)\KodynaNagrody\Wiązka Elektronów\Wiazka-Elektronow\data\proton-2\proton_128MeV.txt",
    r"C:\Projekty (Edycja Laptop)\KodynaNagrody\Wiązka Elektronów\Wiazka-Elektronow\data\proton-3\proton_138MeV.txt",
    r"C:\Projekty (Edycja Laptop)\KodynaNagrody\Wiązka Elektronów\Wiazka-Elektronow\data\proton-4\proton_147MeV.txt"
]

# Colors for the different curves
colors = ['blue', 'green', 'red', 'purple']

# Create a figure and axis
fig, ax = plt.subplots(figsize=(12, 6))

# Plot each Bragg's curve and calculate the area under the curve
areas = []
for file_path, color in zip(file_paths, colors):
    try:
        # Skip the first 8 lines of metadata and read the data
        df = pd.read_csv(file_path, sep='\t', header=0, skiprows=8)
        print(f"First few rows of {file_path}:")
        print(df.head())
        ax.plot(df.iloc[:, 0], df.iloc[:, 1], color=color)
        
        # Calculate the area under the curve
        area = np.trapz(df.iloc[:, 1], df.iloc[:, 0])
        areas.append(area)
    except Exception as e:
        print(f"Error reading {file_path}: {e}")

# Label the axes
ax.set_xlabel('Distance from particle source [mm]')
ax.set_ylabel('Energy deposition [MeV/mm]')

# Add background grid
ax.grid(True)

# Add light blue rectangle representing water body from 10 cm (100 mm) to 25 cm (250 mm)
ax.axvspan(100, 600, color='lightblue', alpha=0.5, label='Water Body')

# Add vertical line at 10 cm (100 mm)
ax.axvline(x=100, color='red', linestyle='--', linewidth=0.8, label='Beginning of water body')

# Find the peak of the Bragg curve for each file and add vertical lines at the peak positions
for file_path, color, area in zip(file_paths, colors, areas):
    try:
        df = pd.read_csv(file_path, sep='\t', header=0, skiprows=8)
        peak_x = df[df.columns[0]][df[df.columns[1]].idxmax()]
        peak_y = df[df.columns[1]].max()
        ax.axvline(x=peak_x, color='black', linestyle='--', linewidth=0.8, label='Bragg Peak')
        ax.axhline(y=peak_y, color='black', linestyle='--', linewidth=0.8)
        ax.annotate(f'Peak: {peak_y:.2f} MeV/mm\nArea: {area:.2f}', xy=(peak_x, peak_y), xytext=(peak_x + 2, peak_y + 2), fontsize=10, color='black', fontweight='bold')
    except Exception as e:
        print(f"Error reading {file_path}: {e}")

# Set x-axis limits to display only the given distance range
ax.set_xlim([0, 350])  # Adjust the range as needed

# Add legend
ax.legend(['Proton 118 MeV', 'Proton 128 MeV', 'Proton 138 MeV', 'Proton 147 MeV', 'Water Body', 'Beginning of water body', 'Bragg Peak'])

# Show the plot
plt.show(block=True)