
def central(x, fx, f=None, h=1e-6):
    if fx is None:
        fx = f(x)
    return (f(x+h) - 2*f(x) + f(x-h)) / (h*h)

