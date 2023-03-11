from matplotlib.pyplot import *
from numpy import *

# Solicita al usuario la cantidad de ciclos de la señal triangular.
cantidadCiclos = int(input("Introduzca la cantidad de ciclos: "))
# Solicita al usuario el número de iteraciones a utilizar en la serie de Fourier.
iteraciones = int(input("Introduzca el numero de iteraciones: "))

# Crea un arreglo de 100 puntos equidistantes entre 0 y la cantidad de ciclos 
# introducidos por el usuario, multiplicado por pi, la función "linspace" genera 
# un arreglo que cubre el rango completo de la señal, desde 0 hasta "cantidadCiclos" veces pi.
x = linspace(0, cantidadCiclos * pi, 100)

# Se define una función llamada "serieFuorierTriangular" que toma como argumentos un arreglo "x" 
# y un entero "iteraciones", la función calcula y devuelve la serie de Fourier de la señal
# triangular.
def serieFuorierTriangular(x, iteraciones):
    suma = 0
    for i in range(iteraciones):
        suma += ((sin((2*i-1)*x)) * (((-1)**i) / ((2*i-1)**2)))
    return suma

# Se grafica la serie de Fourier utilizando la función plot de Matplotlib.
# Utilizamos el arreglo x y la función serieFuorierTriangular para obtener los valores de la señal.
# 'g-' indica que la línea debe ser de color verde.
plot(x, serieFuorierTriangular(x, iteraciones), 'g-')
# Se configura la etiqueta del eje x
xlabel("Tiempo")
# Se configura la etiqueta del eje y
ylabel("Amplitud")
# Se configura el titulo de la grafica
title("Serie de Fuorier para una señal triangular")
# Se configura la leyenda de la grafica
legend(['Señal Triangular'])
# Se muestra la grafica
show()