# from matplotlib.pyplot import *
# from numpy import *

# # Solicita al usuario la cantidad de ciclos de la señal triangular.
# cantidadCiclos = int(input("Introduzca la cantidad de ciclos: "))
# # Solicita al usuario el número de iteraciones a utilizar en la serie de Fourier.
# iteraciones = int(input("Introduzca el numero de iteraciones: "))

# # Crea un arreglo de 100 puntos equidistantes entre 0 y la cantidad de ciclos 
# # introducidos por el usuario, multiplicado por pi, la función "linspace" genera 
# # un arreglo que cubre el rango completo de la señal, desde 0 hasta "cantidadCiclos" veces pi.
# x = linspace(0, cantidadCiclos * pi, 100)

# # Se define una función llamada "serieFuorierTriangular" que toma como argumentos un arreglo "x" 
# # y un entero "iteraciones", la función calcula y devuelve la serie de Fourier de la señal
# # triangular.
# def serieFuorierTriangular(x, iteraciones):
#     suma = 0
#     for i in range(iteraciones):
#         suma += ((sin((2*i-1)*x)) * (((-1)**i) / ((2*i-1)**2)))
#     return suma

# # Se grafica la serie de Fourier utilizando la función plot de Matplotlib.
# # Utilizamos el arreglo x y la función serieFuorierTriangular para obtener los valores de la señal.
# # 'g-' indica que la línea debe ser de color verde.
# plot(x, serieFuorierTriangular(x, iteraciones), 'g-')
# # Se configura la etiqueta del eje x
# xlabel("Tiempo")
# # Se configura la etiqueta del eje y
# ylabel("Amplitud")
# # Se configura el titulo de la grafica
# title("Serie de Fuorier para una señal triangular")
# # Se configura la leyenda de la grafica
# legend(['Señal Triangular'])
# # Se muestra la grafica
# show()

import matplotlib.pyplot as plt
import numpy as np

# Funciones para generar las señales
def square_wave(x, n):
    f = 0
    for i in range(1, n + 1):
        f += np.sin((2*i - 1)*x) / (2*i - 1)
    return 4/np.pi * f

def triangle_wave(x, n):
    f = 0
    for i in range(1, n + 1):
        f += (-1)**(i+1) * np.sin(2*np.pi*(2*i-1)*x) / (2*i-1)**2
    return 8/np.pi**2 * f

def sawtooth_wave(x, n):
    f = 0
    for i in range(1, n + 1):
        f += (-1)**(i+1) * np.sin(2*np.pi*i*x) / i
    return -2/np.pi * f

# Selección de señal
signal = input("Seleccione una señal (cuadrada/triangular/diente de sierra): ")

# Selección de términos
terms = int(input("Ingrese el número de términos: "))

# Rango de ángulos
angles = np.linspace(0, 2*np.pi, 1000)

# Graficación de la señal
if signal == "cuadrada":
    y = square_wave(angles, terms)
elif signal == "triangular":
    y = triangle_wave(angles, terms)
elif signal == "diente de sierra":
    y = sawtooth_wave(angles, terms)
else:
    print("Señal no válida")
    exit()

plt.plot(angles, y)
plt.title(f"Serie de Fourier de la señal {signal} con {terms} términos")
plt.xlabel("Ángulo")
plt.ylabel("Amplitud")
plt.show()
