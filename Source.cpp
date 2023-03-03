#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <random>

/* ������� �������� ����� �� ��������� �������� */
bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
    /* ������ ���� */
    std::ofstream labFile;
    labFile.open(fileName);

    /* ���������, ������ �� ���� */
    if (!labFile.is_open())
    {
        std::cout << "������ �������� �����" << std::endl;
    }
    else
    {
        std::cout << "���� ������� ������!" << std::endl;
    }

    /* �������� � ���� ��������� ������ */
    std::random_device randDev;                                     // ������������������� ���������
    std::mt19937 gen(randDev());                                    // ��������� ��������������� �����
    std::uniform_int_distribution<> dis(0, maxNumberValue);         // ����� �������� - �� 0 �� maxNumberValue

    for (int i = 0; i < numbersCount; ++i)
    {
        const int number = dis(gen);
        labFile << number << std::endl;
    }

    /* ��������� ���� */
    labFile.close();

    return true;
}

/* ������� �������� ��������������� ������� � ����� */
bool isFileContainsSortedArray(const std::string& fileName)
{
    /* ��������� ���� */
    std::ifstream labFile;
    labFile.open(fileName);

    /* ���������, ������ �� ���� */
    if (!labFile.is_open())
    {
        std::cout << "������ �������� �����" << std::endl;
    }
    else
    {
        std::cout << "���� ������� ������!" << std::endl;
    }

    int previousNumber = -1;
    int currentNumber = -1;

    /* ��������� ���� �� ��������������� */
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

/* ������� ��������� �������� ����� */
void SplitFile(const std::string& inputFileName, const std::string& outputFileName1, const std::string& outputFileName2)
{
    /* ��������� ������� ���� */
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    /* ���������, ������ �� ���� */
    if (!inputFile.is_open())
    {
        std::cout << "������ �������� �����" << std::endl;
    }
    else
    {
        std::cout << "���� ������� ������!" << std::endl;
    }

    /* ������ ����� ��� ������ */
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

    /* ��������� ����� */
    inputFile.close();
    outputFiles[0].close();
    outputFiles[1].close();
}

/* ������� ������� ������ � ������� ���� */
bool MergeFiles(const std::string& inputFileName1, const std::string& inputFileName2, const std::string& outputFileName1, const std::string& outputFileName2)
{
    /* ������ ������� ����, ���� ����� ���������� ������ */
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

    /* ��������� ����� */
    for (int i = 0; i < 2; i++)
    {
        inputFiles[i].close();
        outputFiles[i].close();
    }

    return 0;
}

/* ������� ���������� �������� ����� */
void SortFile(const std::string& inputFileName)
{
    /* �������� � ���������� ����� ������ */
    std::string FileNames[4] = { "labF1.txt", "labF2.txt", "labF3.txt", "labF4.txt" };

    /* �������� ������� ��������� ����� */
    SplitFile(inputFileName, FileNames[0], FileNames[1]);

    /* �������� ������� ������� ������ */
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

/* ������� �������� � ���������� �������� ����� */
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

    /* ��������� ��������� ��� ������������� ������� �������� � ���������� �������� ����� */
    std::string labFileName = "labFileSort.txt";
    const int numbersCount = 100000;
    const int maxNumberValue = 1000000;

    /* �������� ������� �������� � ���������� �������� ����� */
    CreateAndSortFile(labFileName, numbersCount, maxNumberValue);

    std::cout << std::endl;
    system("pause");
}