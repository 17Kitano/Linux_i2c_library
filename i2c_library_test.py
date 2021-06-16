#from i2c_library_class_py import i2c_fancs
import i2c_fancs
import sys

i2c = i2c_fancs.i2c_fanc(7, "/dev/i2c-1")
datas = [0, 3, 2]
try:
    stat = i2c.i2c_write(9, 3, datas)
    data2 = i2c.i2c_read(9, 3)
except:
    print("error")
    del i2c
    sys.exit()
print(stat)
print(data2)
del i2c
       
