
def central(x, _, f=None, h=1e-6):
    return (f(x+h) - f(x-h)) / (2*h)

def forward(x, fx, f=None, h=1e-6):
    if fx is None:
        fx = f(x)
    return (f(x+h) - fx) / h

def backward(x, fx, f=None, h=1e-6):
    if fx is None:
        fx = f(x)
    return (fx - f(x-h)) / h


