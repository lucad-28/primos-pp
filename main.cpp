#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>

void primos_rango(int inicio, int fin, std::vector<long long> &primos)
{
    for (int i = inicio; i <= fin; ++i)
    {
        bool es_primo = true;
        if (i < 2)
            es_primo = false;
        for (int j = 2; j * j <= i; ++j)
        {
            if (i % j == 0)
            {
                es_primo = false;
                break;
            }
        }
        if (es_primo)
        {
            primos.push_back(i);
        }
    }
}

void primos_secuencial(int n, bool mostrar = true)
{
    long count = 0;
    if (mostrar)
    {
        std::cout << "Numeros primos entre 1 y " << n << ": [";
    }

    for (int i = 1; i <= n; ++i)
    {
        bool es_primo = true;
        if (i < 2)
            es_primo = false;
        for (int j = 2; j * j <= i; ++j)
        {
            if (i % j == 0)
            {
                es_primo = false;
                break;
            }
        }
        if (es_primo)
        {
            if (mostrar)
            {
                if (count > 0)
                    std::cout << ", ";
                std::cout << i;
            }
            count++;
        }
    }
    if (mostrar)
    {
        std::cout << "]\n";
    }
    std::cout << " - Total de primos: " << count << std::endl;
}

void primos_hilos(int n, bool mostrar = true)
{
    int num_hilos = 0;
    std::cout << "Ingrese el numero de hilos: ";
    std::cin >> num_hilos;

    if (std::cin.fail() || num_hilos < 1)
    {
        std::cout << "Error: debe ingresar un numero entero positivo válido." << std::endl;
        return;
    }

    if (num_hilos > std::thread::hardware_concurrency())
    {
        std::cout << "Advertencia: el numero de hilos excede la capacidad del hardware." << std::endl;
    }

    if (num_hilos > n)
    {
        std::cout << "Error: el numero de hilos no puede ser mayor que el número tope." << std::endl;
        return;
    }

    std::cout << "Cantidad de hilos a usar: " << num_hilos << std::endl;
    int chunk = n / num_hilos;

    std::vector<std::thread> hilos;
    std::vector<std::vector<long long>> resultados(num_hilos);

    for (int i = 0; i < num_hilos; ++i)
    {
        int inicio = i * chunk + 1;
        int fin = (i == num_hilos - 1) ? n : (i + 1) * chunk;
        hilos.emplace_back(primos_rango, inicio, fin, std::ref(resultados[i]));
    }

    for (auto &hilo : hilos)
    {
        hilo.join();
    }

    if (mostrar)
    {
        std::cout << "Numeros primos entre 1 y " << n << ": [";
    }

    int count = 0;
    for (const auto &parcial : resultados)
    {
        for (const auto &num : parcial)
        {
            if (mostrar)
            {
                if (count > 0)
                    std::cout << ", ";
                std::cout << num;
            }
            count++;
        }
    }

    if (mostrar)
    {
        std::cout << "]\n";
    }
    
    std::cout << " - Total de primos: " << count << std::endl;
}

int main(int argc, char *argv[])
{

    int n = 0;
    std::cout << "Ingrese el numero tope: ";
    std::cin >> n;

    if (std::cin.fail() || n < 1)
    {
        std::cout << "Error: debe ingresar un número entero positivo válido." << std::endl;
        return 1;
    }

    std::cout << "Numero ingresado: " << n << std::endl;
    std::cout << "Desea mostrar los numeros primos? (s/n): ";
    std::string respuesta;
    std::cin >> respuesta;
    bool mostrar = (respuesta == "s" || respuesta == "S");

    std::cout << "\n======== Secuencial ========" << std::endl;
    auto inicio_secuencial = std::chrono::high_resolution_clock::now();
    primos_secuencial(n, mostrar);
    auto fin_secuencial = std::chrono::high_resolution_clock::now();
    auto duracion_secuencial = std::chrono::duration_cast<std::chrono::milliseconds>(fin_secuencial - inicio_secuencial).count();
    std::cout << " - Duracion secuencial: " << duracion_secuencial << " ms" << std::endl;

    std::cout << "\n\n======== Hilos ========" << std::endl;
    auto inicio_hilos = std::chrono::high_resolution_clock::now();
    primos_hilos(n, mostrar);
    auto fin_hilos = std::chrono::high_resolution_clock::now();
    auto duracion_hilos = std::chrono::duration_cast<std::chrono::milliseconds>(fin_hilos - inicio_hilos).count();
    std::cout << " - Duracion hilos: " << duracion_hilos << " ms" << std::endl;

    return 0;
}