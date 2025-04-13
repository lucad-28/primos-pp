#include <iostream>
#include <thread>
#include <vector>
#include <string>

void suma_rango(int inicio, int fin, long long& resultado) {
    long long suma_local = 0;
    for (int i = inicio; i <= fin; ++i) {
        suma_local += i;
    }
    resultado = suma_local;
}

void primos_rango(int inicio, int fin, std::vector<long long>& primos) {
    for (int i = inicio; i <= fin; ++i) {
        bool es_primo = true;
        if (i < 2) es_primo = false;
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                es_primo = false;
                break;
            }
        }
        if (es_primo) {
            primos.push_back(i);
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <numero tope>" << std::endl;
        return 1;
    }

    int n = 0;
    try {
        n = std::stoi(argv[1]);
        std::cout << "Numero ingresado: " << n << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: argumento no es un numero valido." << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: numero fuera de rango." << std::endl;
        return 1;
    }

    int num_hilos = std::thread::hardware_concurrency();
    int chunk = n / num_hilos;

    std::vector<std::thread> hilos;
    std::vector<std::vector<long long>> resultados(num_hilos);

    for (int i = 0; i < num_hilos; ++i) {
        int inicio = i * chunk + 1;
        int fin = (i == num_hilos - 1) ? n : (i + 1) * chunk;
        hilos.emplace_back(primos_rango, inicio, fin, std::ref(resultados[i]));
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    std::cout << "Numeros primos entre 1 y " << n << ":\n";

    for (const auto& parcial : resultados) {
        for (const auto& num : parcial) {
            std::cout << num << " ";
        }
    }

    return 0;
}