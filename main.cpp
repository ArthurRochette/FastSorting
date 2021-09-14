#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <unistd.h>


using namespace std;

const int WINDOWX = 800;
const int WINDOWY = 600;

int g_maxDataSetBValue = 0;
int g_ratioRect = 0;
int actual_pivot;
int actual_testing;

/*
 * randomSet
 * **dataSet: pointer pointing an array of int
 * dataSize : size of dataSet array
 */
void randomSet(int **dataSet, int dataSize) {
    int randomNumber;
    *dataSet = new int[dataSize];
    srandom(time(nullptr));
    for (int i = 0; i < dataSize; i++) {
        randomNumber = (int) random() % 1000;
        (*dataSet)[i] = randomNumber;
        if (randomNumber > g_maxDataSetBValue) g_maxDataSetBValue = randomNumber;
    }
}
/*
 * isSorted
 * *dataSet: array of int
 * dataSize: size of the array
 */
bool isSorted(const int *dataSet, int dataSize) {
    for (int i = 0; i < dataSize - 1; i++) {
        if (dataSet[i] > dataSet[i + 1]) return false;
    }
    return true;
}
/*
 * swap
 * *dataSet : array of int
 * indexa : index of the first value to swap
 * indexb : index of the second value to swap
 */
void swap(int *dataSet, int indexa, int indexb) {
    int buff = dataSet[indexa];
    dataSet[indexa] = dataSet[indexb];
    dataSet[indexb] = buff;
}

/*
 * FastSorting
 * dataSet : array of values
 * dataSize : size of the pasted array
 * pivot : index in dataSet of the pivot
 * off : shift start of array
 */

void drawing(sf::RenderWindow *window, const int *dataSet, int dataSize) {
    for (int i = 0; i < dataSize; i++) {
        auto *rect = new sf::RectangleShape(
                sf::Vector2f((int) (WINDOWX / dataSize),
                             (float) (dataSet[i] * g_ratioRect) / (float) g_maxDataSetBValue));
        rect->setOrigin(sf::Vector2f(0, (float) (dataSet[i] * g_ratioRect) / (float) g_maxDataSetBValue));
        rect->setOutlineColor(sf::Color::Black);
        rect->setOutlineThickness(1);
        rect->setPosition((float) (WINDOWX / dataSize) * i, WINDOWY);
        if (i == actual_pivot) rect->setFillColor(sf::Color::Red);
        else if(i == actual_testing) rect->setFillColor(sf::Color::Green);
        else rect->setFillColor(sf::Color::White);
        window->draw(*rect);
    }
    window->display();
}
/*
 * FastSorting
 * *dataSet : array of int
 * dataSize: size of the array
 * pivot : pivot's index in the array 
 * off : value where we start sorting
 */
void FastSorting(int *dataSet, int dataSize, int pivot, int off) {
    actual_pivot = pivot;
    swap(dataSet, pivot, dataSize - 1);
    pivot = dataSize - 1;
    actual_pivot = pivot;
    for (int i = off; i < pivot; i++) {
        actual_testing = i;
        usleep(5000);
        if (dataSet[i] > dataSet[pivot]) {
            for (int a = i; a < dataSize - 1; a++) {
                swap(dataSet, a, a + 1);
            }
            i--;
        }
    }
    if (isSorted(dataSet, dataSize)) return;

    FastSorting(dataSet, pivot, 0, 0);
    FastSorting(dataSet, dataSize, pivot, pivot);

}

int main() {
    int dataSetSize = 10;
    int **dataSet = new int *;
    g_ratioRect = (((WINDOWY * 90) / 100));

    std::cout << "Data set size:" << std::endl;
    std::cin >> dataSetSize;
    randomSet(dataSet, dataSetSize);
    std::thread thread(FastSorting, *dataSet, dataSetSize, dataSetSize / 2, 0);
    thread.detach();
    sf::RenderWindow window(sf::VideoMode(800, 600), "FastSorting");
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        drawing(&window, *dataSet, dataSetSize);
    }


    return 0;
}
