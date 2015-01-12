from ctypes import * #https://docs.python.org/2/library/ctypes.html

class RegData(Structure):
    _fields_ = [('i', c_int), #odwrotnie niz w strukturze C!
                ('f', c_float)] #https://docs.python.org/2/library/ctypes.html

with open('test.bin', 'rb') as file:
    result = []
    reg = RegData()
    while file.readinto(reg) == sizeof(reg): #readinto zwraca liczbe zwracanych bajtow. uwaga: moze dzialac roznie na roznych systemach
        result.append((reg.f, reg.i))

for r in result:
    print r
