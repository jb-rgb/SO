#Codigo para algoritmo de forma recursiva
import time


def fib1(n):
    if n == 0:
       return 0
    elif n == 1:
        return 1
    else:
        return fib1(n-1) + fib1(n-2)

#Codigo para algoritmo de forma iperativa
def fib2(n):
    if n == 0:
        return 0
    else:
        f = [0,1]
        for i in range(2,n+1):
            f.append(f[i-1] + f[i-2])
        return f[i]

x = int(input("Ingrese numero n que se desea calcular: "))

start_time = time.time()
print(fib1(x))
fin_time = time.time() - start_time
print("El tiempo de la forma recursiva es de: ", fin_time)

#Calcular el tiempo de Fib2
start_time1 = time.time()
print(fib2(x))
fin_time = time.time() - start_time1
print("El tiempo de la forma iperativa es de: ", fin_time)