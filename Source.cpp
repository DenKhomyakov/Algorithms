#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <random>

/* Функция создания файла со случайным массивом */
bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
    /* Создаём файл */
    std::ofstream labFile;
    labFile.open(fileName);

    /* Проверяем, открыт ли файл */
    if (!labFile.is_open())
    {
        std::cout << "Ошибка открытия файла" << std::endl;
    }
    else
    {
        std::cout << "Файл успешно открыт!" << std::endl;
    }

    /* Помещаем в файл случайный массив */
    std::random_device randDev;                                     // Недетерминированный генератор
    std::mt19937 gen(randDev());                                    // Генерация псевдослучайных чисел
    std::uniform_int_distribution<> dis(0, maxNumberValue);         // Задаём диапазон - от 0 до maxNumberValue

    for (int i = 0; i < numbersCount; ++i)
    {
        const int number = dis(gen);
        labFile << number << std::endl;
    }

    /* Закрываем файл */
    labFile.close();

    return true;
}

/* Функция проверки упорядоченности массива в файле */
bool isFileContainsSortedArray(const std::string& fileName)
{
    /* Открываем файл */
    std::ifstream labFile;
    labFile.open(fileName);

    /* Проверяем, открыт ли файл */
    if (!labFile.is_open())
    {
        std::cout << "Ошибка открытия файла" << std::endl;
    }
    else
    {
        std::cout << "Файл успешно открыт!" << std::endl;
    }

    int previousNumber = -1;
    int currentNumber = -1;

    /* Проверяем файл на упорядоченность */
    while (labFile >> currentNumber)
    {
        if (currentNumber < previousNumber)
        {
            return false;
        }

        previousNumber = currentNumber;
    }
       
    return true;
}

/* Функция разбиения главного файла */
void SplitFile(const std::string& inputFileName, const std::string& outputFileName1, const std::string& outputFileName2)
{
    /* Открываем главный файл */
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    /* Проверяем, открыт ли файл */
    if (!inputFile.is_open())
    {
        std::cout << "Ошибка открытия файла" << std::endl;
    }
    else
    {
        std::cout << "Файл успешно открыт!" << std::endl;
    }

    /* Создаём файлы для записи */
    std::ofstream outputFiles[2];
    outputFiles[0].open(outputFileName1);
    outputFiles[1].open(outputFileName2);

    int n = 0;
    int presentElement;
    int previousElement;
    inputFile >> previousElement;

    while (inputFile >> presentElement)
    {
        outputFiles[n] << previousElement << std::endl;

        if (previousElement > presentElement)
        {
            n = 1 - n;
        }

        previousElement = presentElement;
    }

    outputFiles[n] << previousElement;

    /* Закрываем файлы */
    inputFile.close();
    outputFiles[0].close();
    outputFiles[1].close();
}

/* Функция слияния файлов в главный файл */
bool MergeFiles(const std::string& inputFileName1, const std::string& inputFileName2, const std::string& outputFileName1, const std::string& outputFileName2)
{
    /* Создаём главный файл, куда будем записывать данные */
    std::ifstream inputFiles[2];
    inputFiles[0].open(inputFileName1);
    inputFiles[1].open(inputFileName2);

    std::ofstream outputFiles[2];
    outputFiles[0].open(outputFileName1);
    outputFiles[1].open(outputFileName2);

    int m = 0;
    int n = 0;
    int presentElement[2];
    int previousElement[2];
    inputFiles[0] >> previousElement[0];

    if (!(inputFiles[1] >> previousElement[1]))
    {
        return 1;
    }

    while (!inputFiles[0].eof() || !inputFiles[1].eof())
    {
        if (previousElement[m] > previousElement[1 - m] && !inputFiles[1 - m].eof())
        {
            m = 1 - m;
        }

        outputFiles[n] << previousElement[m] << std::endl;
        inputFiles[m] >> presentElement[m];

        if (inputFiles[m].eof() || previousElement[m] > presentElement[m])
        {
            if (!inputFiles[1 - m].eof())
            {
                m = 1 - m;
            }

            outputFiles[n] << previousElement[m] << std::endl;
            inputFiles[m] >> presentElement[m];

            while (!inputFiles[m].eof() && previousElement[m] <= presentElement[m])
            {
                previousElement[m] = presentElement[m];
                outputFiles[n] << previousElement[m] << std::endl;
                inputFiles[m] >> presentElement[m];
            }

            previousElement[1 - m] = presentElement[1 - m];

            n = 1 - n;
        }

        previousElement[m] = presentElement[m];
    }

    /* Закрываем файлы */
    for (int i = 0; i < 2; i++)
    {
        inputFiles[i].close();
        outputFiles[i].close();
    }

    return 0;
}

/* Функция сортировки главного файла */
void SortFile(const std::string& inputFileName)
{
    /* Помещаем в переменную имена файлов */
    std::string FileNames[4] = { "labF1.txt", "labF2.txt", "labF3.txt", "labF4.txt" };

    /* Вызываем функцию разбиения файла */
    SplitFile(inputFileName, FileNames[0], FileNames[1]);

    /* Вызываем функцию слияния файлов */
    while (!MergeFiles(FileNames[0], FileNames[1], FileNames[2], FileNames[3]))
    {
        std::string tmp;

        for (int i = 0; i < 2; i++)
        {
            tmp = FileNames[i + 2];
            FileNames[i + 2] = FileNames[i];
            FileNames[i] = tmp;
        }
    }

    std::cout << FileNames[2] << FileNames[3];
}

/* Функция создания и сортировки главного файла */
int CreateAndSortFile(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
    if (!CreateFileWithRandomNumbers(fileName, numbersCount, maxNumberValue))
    {
        return -1;
    }

    SortFile(fileName);

    if (!isFileContainsSortedArray(fileName))
    {
        return -2;
    }

    return 1;
}

int main()
{
    setlocale(LC_ALL, "Rus");

    /* Указываем параметры для использования функции создания и сортировки главного файла */
    std::string labFileName = "labFileSort.txt";
    const int numbersCount = 100000;
    const int maxNumberValue = 1000000;

    /* Вызываем функцию создания и сортировки главного файла */
    CreateAndSortFile(labFileName, numbersCount, maxNumberValue);

    std::cout << std::endl;
    system("pause");
}