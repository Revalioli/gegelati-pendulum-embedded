#!/usr/bin/python

from audioop import minmax
import math
import numpy as np
import matplotlib as mp
import matplotlib.pyplot as plt

from matplotlib.widgets import Cursor 

import sys
import re

if len(sys.argv) != 2 :
    sys.exit("This script must have only one argument for the file with the current measures")

# === Reading variables ===
logStart = "##### Log Start #####"
logEnd = "##### Log End #####"
# lineRegex = "Step : (-?\d+), Current : (\d+\.?\d*)"
lineRegex = "(-?\d+)\t(\d+\.?\d*)\t(\d+\.?\d*)"
paramRegex = "TimeUnit : ([a-z]+)\tTimerMultiplier : (\d+\.?\d*)"
headerLine = "Step\tCurrent\tPower"
skipLine = True
timeUnit = ""
timeMultiplier = 1

# === Parsing ===

currents = []
power = []
pendulumSteps = []

file = open(sys.argv[1])    # Open read only

for idx, line in enumerate(file):
    line = line.rstrip('\n')

    if skipLine:    # Not currently in a valid log part
        skipLine = not (line == logStart)
    elif line == logEnd:    # Currently in a valid log part, is it the end ?
        skipLine = True
    else:
        # Parsing time !

        # Data line
        match = re.search(lineRegex, line)
        if not (match is None) :
            # Remove any measurement done before or after the inference
            if match.group(1) == -1:
                continue

            pendulumSteps.append( int(match.group(1)) )
            currents.append( float(match.group(2)) * 1000 ) # convert A to mA
            power.append( float(match.group(3)) )   # in W
            continue

        # Parameters line
        match = re.search(paramRegex, line)
        if not (match is None):
            timeUnit = match.group(1)
            timeMultiplier = float(match.group(2))
            continue

        # Header line
        if line == headerLine:
            continue

        sys.exit(f"Invalid line format at line {idx+1}")

        

time = [ (i+1) * timeMultiplier for i in range(len(currents)) ]

# === Plot results ===

class SnaptoCursor(object):
    def __init__(self, ax, x, y):
        self.ax = ax
        self.ly = ax.axvline(color='k', alpha=0.2)  # the vert line
        self.marker, = ax.plot([0],[0], marker="o", color="crimson", zorder=3) 
        self.x = x
        self.y = y
        self.txt = ax.text(0.7, 0.9, None, fontsize = 'medium', fontweight = 'bold')

    def mouse_move(self, event):
        if not event.inaxes: return
        x, y = event.xdata, event.ydata
        indx = np.searchsorted(self.x, [x])[0]

        indx = max(indx, 0)
        indx = min(indx, len(self.x)-1)

        x = self.x[indx]
        y = self.y[indx]
        self.ly.set_xdata(x)
        self.marker.set_data([x],[y])
        self.txt.set_text('x=%1.2f, y=%1.2f' % (x, y))
        self.txt.set_position((x,y))
        self.ax.figure.canvas.draw_idle()


# Current
ax1 = plt.subplot()
ax1.set_ylabel("mA")
l1, = ax1.plot(time, currents, color='red')
ax1.set(ylim=(10.0, 30.0))
ax1.set_xlabel(f"Elapsed time ({timeUnit})")

# Pendulum steps
ax2 = ax1.twinx()
ax2.set_ylabel("Pendulum step")
l2, = ax2.plot(time, pendulumSteps, color='blue')

# Cursors
cursor = SnaptoCursor(ax1, [i for i in range(1, len(currents))], currents)
cursor2 = SnaptoCursor(ax2, [i for i in range(1, len(pendulumSteps))], pendulumSteps)
plt.connect('motion_notify_event', cursor.mouse_move)
plt.connect('motion_notify_event', cursor2.mouse_move)

plt.legend([l1, l2], ["Current", "Pendulum step"])
plt.show()
