"""
This class represents a competitive market, where buyers and sellers are both price takers.

Consider : 

    1. A linear demand curve Q_d = a_d - (b_d) * p 
    2. A linear supply curve Q_s = a_z + (b_z) * (p-t)

Whereby:
    p is price paid by buyer
    Q is quantity
    t is per-tax unit
    Other symbols are demand and supply params

    The class gives methods to comute values of interest,
    including competitive equilibrim price and quantity,
    tax revenue raised, consumer surplus and producer
    surplus.
"""

from scipy.integrate import quad
import numpy as np
import matplotlib.pyplot as plt

class Market:

    def __init__(self, ad, bd, az, bz, tax):
        "Setup market params. All are scalars"
        self.ad, self.bd, self.az, self.bz, self.tax = ad, bd, az, bz, tax
        if ad < az:
            raise ValueError('Insufficient demand')


    def price(self):
        "Compute the equilibrium price where Q_d == Q_s"
        return (self.ad - self.az + self.bz * self.tax) / (self.bd + self.bz)

    def quantity(self):
        "Compute equilibrium quantity"
        return self.ad - self.bd * self.price()

    def consumer_surplus(self):
        "Compute consumer surplus"
        #Area under inverse demand function
        integrand = lambda x: (self.ad / self.bd) - (1/self.bd) * x
        area, error = quad(integrand, 0, self.quantity())
        return area - self.price() * self.quantity()
    
    def producer_surplus(self):
        "Compute producer surplus"
        # Compute area above inverse supply curve, excluding tax
        integrand = lambda x : -(self.az / self.bz) + (1/self.bz) * x
        area, error = quad(integrand, 0, self.quantity())
        return (self.price() - self.tax) * self.quantity() - area
    
    def tax_revenue(self):
        "Compute tax revenue"
        return self.tax * self.quantity()

    def inverse_demand(self, x):
        "Compute inverse demand"
        return self.ad / self.bd - (1 / self.bd) * x

    def inverse_supply(self, x):
        "Compute inverse supply"
        return -(self.az / self.bz) + (1 / self.bz) * x + self.tax

    def inverse_supply_no_tax(self, x):
        "Compute inverse supply curve without tax"
        return -(self.az / self.bz) + (1 / self.bz) * x


"""Examples of usage"""

"""
def deadweightloss(m):
    "Computes the deadweight loss for the given market"
    #Create a simlar market with no tax
    no_tax_market = Market(m.ad, m.bd, m.az, m.bz, 0)

    #Get the surplus of the two markets and return the difference
    notax_surplus = no_tax_market.consumer_surplus() + no_tax_market.producer_surplus()
    original_surplus = m.consumer_surplus() + m.producer_surplus() + m.tax_revenue()
    return notax_surplus - original_surplus

base_params = 15, .5, -2, .5, 3
m = Market(*base_params)
print("Equilibrium price = ", m.price())
print("Consumer surplus = ", m.consumer_surplus())

#Inverse demand curve, inverse supply curve (with and without tax)
q_max = m.quantity()*2
q_grid = np.linspace(0.0, q_max, 100)
p_dem = m.inverse_demand(q_grid)
p_sup = m.inverse_supply(q_grid)
p_sup_no_tax = m.inverse_supply_no_tax(q_grid)

fig, ax = plt.subplots()
ax.plot(q_grid, p_dem, lw=2, alpha=0.6, label='demand')
ax.plot(q_grid, p_sup, lw=2, alpha=0.6, label='supply')
ax.plot(q_grid, p_sup_no_tax, '--k', lw=2, alpha=0.6, label='supply without tax')

ax.set_xlabel('quantity', fontsize = 14)
ax.set_ylabel('price', fontsize = 14)
ax.set_xlim(0, q_max)

ax.legend(loc='lower right', frameon=False, fontsize=14)

plt.show()

print("Deadweight loss: " + str(deadweightloss(m)))
"""
