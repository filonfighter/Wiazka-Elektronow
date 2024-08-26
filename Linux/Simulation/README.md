# Running code

Create folder for building code:
```bash
mkdir build
cd build
```

Compile code:
```bash
cmake ..
```

Build code:
```bash
make
```

Move files run.mac, vis.mac and plot.py to build folder.

Run code in Geant4 UI:
```bash
./BC2
```
or run code with data from run.mac:
```bash
./BC2 run.mac
```

Results will be saved in braggs_curve.txt