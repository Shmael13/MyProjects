"""
This is a class to model the chaos dynamics in a non-linear system
The transition rule is:
    x_{t+1} = r x_t (1 - x_t)
Where:
    - x_0 in [0, 1] 
    - r in [0, 4]
"""
import matplotlib.pyplot as plt
class Chaos:
    "Models the dynamical system"

    def __init__(self, x0, r):
        "Initialize with state x0 and parameter rr"
        self.x, self.r = x0, r

    def update(self):
        "Update the state by one time interval"
        self.x = self.r * self.x * (1 - self.x)

    def generate_sequence(self, n):
        "Generate and return sequence of length n"
        path = []
        for i in range(n):
            path.append(self.x)
            self.update()
        return path


ch = Chaos(0.1, 4.0)

#Print the sequence of 5 transitions
print(ch.generate_sequence(5))

#longer trajectory is plotted here
num_transitions = 400
fig, subplot = plt.subplots()
subplot.set_xlabel('$t$', fontsize = 14)
subplot.set_ylabel('$x_t$', fontsize = 14)
x = ch.generate_sequence(num_transitions)
subplot.plot(range(num_transitions), x, 'bo-', alpha=0.5, lw=2, label='$x_t$')
plt.show()

#Bifurcation diagram of the same chaos dynamic
r = 1 
fig, ax = plt.subplots()
while r < 4:
    ch.r = r
    t = ch.generate_sequence(1000)[950:] #plot just the tail
    ax.plot([r] * len(t), t, 'b.', ms=0.6)
    r = r + 0.005

ax.set_xlabel('$r$', fontsize = 14)
ax.set_ylabel('state space $x_t$', fontsize = 14)
plt.show()
