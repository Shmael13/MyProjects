""" 
    This is a class that plots a function f of two variables (x, y) over [-a, a] X [-a, a]
        - It's initialization takes as input f.
        - It's method sketch_3d takes as input the area a, and the resolution of the sketch
    
    In our example, let f(x,y) = cos(x^2 + y^2)/ (1 + x^2 + y^2), a = 3, and resolution = 100
"""
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.axes3d import Axes3D
from matplotlib import cm

class Plotter:
    def __init__(self, function):
        self.function = function 

    def sketch_3d(self, area, resolution):
        xgrid = np.linspace(-area, area, resolution)
        ygrid = xgrid
        
        x, y = np.meshgrid(xgrid, ygrid)
        z = self.function(x, y)

        fig = plt.figure(figsize=(10,8))
        ax = fig.add_subplot(111, projection='3d')


        ax.plot_surface(x, y, z, rstride=2, cstride=2, cmap=cm.jet, alpha=0.7, linewidth=0.25)
        ax.set_zlim(-0.5, 1)
        ax.set_xlabel('$x$', fontsize=14)
        ax.set_ylabel('$y$', fontsize=14)
        plt.show()

def f(x, y):
    return np.cos(x**2 + y **2) / (1 + x**2 + y**2)

plotter = Plotter(f)
plotter.sketch_3d(3, 100)
