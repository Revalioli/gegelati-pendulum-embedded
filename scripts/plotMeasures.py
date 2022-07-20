#!/usr/bin/python

import math
from os import stat
import numpy as np
import matplotlib as mp
import matplotlib.pyplot as plt

import sys
import re
import getopt
from datetime import datetime

# Check and parse script options

# --show : display graph of the measures in a matlotlib window
# -S <dir_path> : export graph as a png file and some stats about the measures at directory <dir_path>
# -p <prefix> : specify a prefix for all exported files 

if len(sys.argv) < 2:
    sys.exit("Not enough arguments, you must provide a path to the data")

try:
    options, remainder = getopt.getopt(sys.argv[2:], "S:p:", ["show"])
except getopt.GetoptError as err:
    sys.exit(err)

fileName = sys.argv[1]

exportResults = False
exportDir = ""
filePrefix = ""
matplotlibShow = False

for o, a in options:
    if o == "-S":
        exportResults = True
        exportDir = a
    elif o == "-p":
        filePrefix = a
    elif o == "--show":
        matplotlibShow = True
    else:
        sys.exit("Unhandled option #{o}")

pngFile = None
statsFile = None

if exportResults:
    try:
        pngFile = open(f"{exportDir}/{filePrefix}graph.png", "w+b")
        statsFile = open(f"{exportDir}/{filePrefix}energy_stats.md", "w+")
    except FileNotFoundError as err:
        sys.exit(err)


#================
# === Parsing ===
#================

# === Reading variables ===
logStart = "##### Log Start #####"
logEnd = "##### Log End #####"

floatRegex = "-?\d+\.?\d*"
integerRegex = "-?\d+"
textRegex = "[a-zA-Z]+"
lineRegex = f"({integerRegex})\t({floatRegex})\t({floatRegex})"
paramRegex = f"dataTimeUnit : ({textRegex})\tdataTimerMultiplier : ({floatRegex})\tStartAngle : {floatRegex}\tStartVelocity : {floatRegex}"
headerLine = "Step\tCurrent\tPower"
inferenceTimingLine = f"=== T_{textRegex} : \d* {textRegex}"

skipLine = True
timeUnit = ""
timeMultiplier = 1


currents = []
powers = []
pendulumSteps = []

file = open(fileName)    # Open read only

for idx, line in enumerate(file):
    line = line.rstrip('\n\r')

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
            powers.append( float(match.group(3)) )   # in W
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

        # Inference timing results line
        if re.match(inferenceTimingLine, line) is not None:
            continue

        sys.exit(f"Invalid line format at line {idx+1}")


time = [ (i+1) * timeMultiplier for i in range(len(currents)) ]

#=========================
# === Analyse measures ===
#=========================

if exportResults:
    currentAvg = np.average(currents)
    powerAvg = np.average(powers)
    stepAvgTime = time[-1]/pendulumSteps[-1]

    statsFile.write("# Measures statistics\n")
    statsFile.write(f"Average current : {currentAvg}\n")
    statsFile.write(f"Average power consumption : {powerAvg}\n")
    statsFile.write(f"Average step time : {stepAvgTime} {timeUnit}\n")

    statsFile.close()


#=====================
# === Plot results ===
#=====================

class AxeCursor(object):
    def __init__(self, ax, resultField):
        self.ax = ax
        self.ly = ax.axvline(color='k', alpha=0.2)  # The vertical line
        self.marker, = ax.plot([0],[0], marker="o", color="crimson", zorder=3)  # Create a single point line2D as marker

        self.xdata, self.ydata = ax.get_lines()[0].get_data()   # Get reference to the data of the first line of the Axes object

        self.txt = ax.text(0.7, 0.9, None, fontsize = 'medium', fontweight = 'bold')
        self.resultField = resultField

    def mouse_move(self, event):
        if not event.inaxes: return     # Do nothing if mouse not on axes
        mouse_x = event.xdata # Get mouse x cooridnate in the axes

        # Find point corresponding to mouse position
        indx = np.searchsorted(self.xdata, [mouse_x])[0]
        indx = min(indx, len(self.xdata)-1)

        # Finding point closest to the mouse
        if indx > 0 and indx < len(self.xdata)-1:
            indx = indx if math.dist( [mouse_x], [self.xdata[indx]] ) < math.dist( [mouse_x], [self.xdata[indx-1]] ) else indx

        marker_x = self.xdata[indx]
        marker_y = self.ydata[indx]

        self.ly.set_xdata(marker_x)
        self.marker.set_data([marker_x],[marker_y])
        self.txt.set_text('x=%1.2f, y=%1.2f' % (marker_x, marker_y))
        self.txt.set_position((marker_x,marker_y))

        if isinstance(marker_y, np.float):
            self.resultField.set_text(f"{marker_y:1.3f}")
        else:
            self.resultField.set_text(f"{marker_y}")
        
        self.ax.figure.canvas.draw_idle()
        
# Set figure inital size to be big enough for plt.savefig
fig = plt.gcf()
fig.set_size_inches(12.8, 9.6)

if exportDir:
    fig.suptitle(exportDir)

# Current
ax1 = plt.subplot()
ax1.set_ylabel("mA", color='red')
ax1.set(ylim=(10.0, 30.0))
ax1.set_xlabel(f"Elapsed time ({timeUnit})")
l1, = ax1.plot(time, currents, color='red')

# Pendulum steps
ax2 = ax1.twinx()
ax2.set_ylabel("Pendulum step", color='blue')
l2, = ax2.plot(time, pendulumSteps, color='blue')

# Power
ax3 = ax1.twinx()
ax3.spines["right"].set_position(("outward", 60))
ax3.set(ylim=(0.0, 0.1))
ax3.set_ylabel("W", color='green')
l3, = ax3.plot(time, powers, color='green')

plt.legend([l1, l2, l3], ["Current", "Pendulum step", "Power"])
plt.subplots_adjust(right=0.85)

if exportResults:
    plt.savefig(pngFile)
    pngFile.close()


if matplotlibShow:
    # Table for cursors
    t1 = ax1.table( [ ["Current", "0.0"], ["Power", "0.0"], ["Step", "0"] ], cellLoc='center', bbox=[0.0, -0.3, 0.2, 0.2])
    t1.auto_set_font_size(False)
    t1.set_fontsize(16)
    t1[0, 0].get_text().set_color('red')
    t1[1, 0].get_text().set_color('green')
    t1[2, 0].get_text().set_color('blue')

    # Cursors
    cursor = AxeCursor(ax1, t1[0, 1].get_text())
    cursor2 = AxeCursor(ax2, t1[2, 1].get_text())
    cursor3 = AxeCursor(ax3, t1[1, 1].get_text())
    plt.connect('motion_notify_event', cursor.mouse_move)
    plt.connect('motion_notify_event', cursor2.mouse_move)
    plt.connect('motion_notify_event', cursor3.mouse_move)

    plt.subplots_adjust(bottom=0.25)

    plt.show()
