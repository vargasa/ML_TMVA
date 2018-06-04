import numpy as np
import math

def genData(n):

    fileData = open("sin_x.dat","w")
    
    for i in range(n):
        
        x = np.random.uniform(-10*math.pi,10*math.pi)
        
        fileData.write('{0:.5f}\t{1:.5f}\n'.format(x, math.sin(x)))
        
    fileData.close()
    
genData(int(1e6));
