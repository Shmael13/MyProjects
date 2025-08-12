class Consumer:
    
    def __init__(self, w):
        self.wealth = w

    def earn(self, y):
        self.wealth += y

    def spend(self, x):
        new_wealth = self.wealth - x
        if (new_wealth < 0):
            print("Insufficient Funds")
        else:
            self.wealth = new_wealth

c3 = Consumer(10)
print(c3.spend(5))
print(c3.wealth)
print(c3.__dict__)
