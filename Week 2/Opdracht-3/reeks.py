"""
def reeks():
    counter = 1
    result = 1

    while result <= 255:
        print(result)

        if (counter % 2) == 0:
            result = result - 1
        else:
            result = result + (result * 2)
        
        counter +=1

reeks()
"""
def number_pattern():

        x = 3
        y = 1
        z = 1
        sequence = [1]

        while z * x <= 255:
                multiply = z * x
                sequence.append(multiply)
                substract = multiply - y
                sequence.append(substract)
                z = substract

        print(sequence)

number_pattern()