#!/usr/bin/python

import math
import numpy as np
import matplotlib as mp
import matplotlib.pyplot as plt

import sys
import re


fileName = ""
if len(sys.argv) != 2 :
    fileName = "../picocom.log"
    # sys.exit("This script must have only one argument for the file with the current measures")
else:
    fileName = sys.argv[1]

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

file = open(fileName)    # Open read only

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
        self.ly = ax.axvline(color='k', alpha=0.2)  # The vertical line
        self.marker, = ax.plot([0],[0], marker="o", color="crimson", zorder=3)  # Create a single point line2D as marker 
        self.x = x  # x data of the line to be tracked
        self.y = y  # y data of the line to be tracked
        self.txt = ax.text(0.7, 0.9, None, fontsize = 'medium', fontweight = 'bold')

    def mouse_move(self, event):
        if not event.inaxes: return     # Do nothing if mouse not on axes
        mouse_x = event.xdata # Get mouse x cooridnate in the axes

        # Find point corresponding to mouse position
        indx = np.searchsorted(self.x, [mouse_x])[0]
        indx = min(indx, len(self.x)-1)

        # Finding point closest to the mouse
        if indx > 0 and indx < len(self.x)-1:
            indx = indx if math.dist( [mouse_x], [self.x[indx]] ) < math.dist( [mouse_x], [self.x[indx-1]] ) else indx

        marker_x = self.x[indx]
        marker_y = self.y[indx]

        self.ly.set_xdata(marker_x)
        self.marker.set_data([marker_x],[marker_y])
        self.txt.set_text('x=%1.2f, y=%1.2f' % (marker_x, marker_y))
        self.txt.set_position((marker_x,marker_y))
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
cursor = SnaptoCursor(ax1, time, currents)
cursor2 = SnaptoCursor(ax2, time, pendulumSteps)
plt.connect('motion_notify_event', cursor.mouse_move)
plt.connect('motion_notify_event', cursor2.mouse_move)

plt.legend([l1, l2], ["Current", "Pendulum step"])
plt.show()
