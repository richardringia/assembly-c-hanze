def fibonacci(n):
    f1 = 0
    f2 = 1

    if n ==0:
        return f1
    if n == 1:
        return f2

    for _ in range(1, n):
        f1, f2 = f2, f1 + f2
    return f2
