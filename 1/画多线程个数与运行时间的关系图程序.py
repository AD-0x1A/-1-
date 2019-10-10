import matplotlib.pyplot as plt
import numpy as np

x = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,25,30,50,75,100,125,150,175]
y = [61,46,42,38,40,41,38,38,43,36,35,35,37,34,36,38,36,38,38,37,38,38,40,49,49,47]
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set(ylim=[30,65],
       ylabel='Run Time/ms', xlabel='Thread_Numbers')
ax.plot(x,y)
plt.show()
print(len(y),len(x))
