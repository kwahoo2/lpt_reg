#!/usr/bin/env python
# -*- coding: utf-8 -*-

#pybingraph.py - odczyt plików binarnych (domyślnie "test.bin") wygenerowanych przez lpr_timed 

from ctypes import *
import pyqtgraph as pg
import numpy as np
import sys

class RegData(Structure):
    _fields_ = [('i', c_int), #odwrotnie niz w strukturze C!
                ('f', c_float)] #https://docs.python.org/2/library/ctypes.html

with open('test.bin', 'rb') as file:
    result = []
    reg = RegData()
    while file.readinto(reg) == sizeof(reg): #readinto zwraca liczbę zwracanych bajtow. uwaga: moze działac różnie na rożnych systemach
        result.append((reg.f, reg.i))

#print(result)

x = np.zeros(2 * len(result))
y = np.zeros((4, 2 * len(result)))

counter = 1;
for n in result:
    t = n[0]
    inp = n[1]
    bits = (0b00001000, 0b00010000, 0b00100000, 0b01000000)
    x[counter] = t
    if counter > 1:
        x[counter-1] = t
    
    for j in range (0,4): 
        y[j][counter] = int((inp | bits[j]) == inp)
        if counter > 1:
            y[j][counter-1] = int((oldinp | bits[j]) == oldinp)

    oldinp = inp
    counter = counter + 2

print x, y
plotWidget = pg.plot(title="LPT status reg")

for i in range(4):
    plotWidget.plot(x, y[i]+2*i, pen=(i,4))  #ustawienie pen=(i,4) automatycznie tworzy 4 różnolorowe pisaki, +2*i zapobiega nakłądnaniu się wykresów

data = np.column_stack((x, y[0], y[1], y[2], y[3])) #export do pliku csv
np.savetxt('test.csv', data, delimiter=" ") 

# Start pętli zdadzeń Qt (jeśli nie jest używany tryb interaktywny)
if __name__ == '__main__':
    import sys
    if sys.flags.interactive != 1 or not hasattr(QtCore, 'PYQT_VERSION'):
        pg.QtGui.QApplication.exec_()
