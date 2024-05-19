#include <iostream>
#include "Chrono.h"

static void PrintSpace()
{
    std::cout << "\n//////////////////////////////////////////////\n" << std::endl;
}


#pragma region PaddingSpeedTest
#define PaddingSpeedTestNumber 10'000'000
//Struct Size exemple : Exemple1 is 12 bytes due to padding when Exemple2 is 8 bytes
struct Exemple1
{
    char mLetter;
    unsigned int mInteger;
    short mShort;
};

struct Exemple2
{
    unsigned int mInteger;
    short mShort;
    char mLetter;
} ;


static void PaddingSpeedTest()
{
    Exemple1* E1 = new Exemple1[PaddingSpeedTestNumber]{};
    Exemple2* E2 = new Exemple2[PaddingSpeedTestNumber]{};

    //Start with the longer struct size
    Chrono Timer;
    for (int i = 0; i < PaddingSpeedTestNumber; ++i)
    {
        E1[i].mInteger = 2;
    }
    auto duration1 = Timer.GetDurationNanoSeconds();
    std::cout << "E1 time : " << duration1 << "ns\n";
    std::cout.flush();

    //Then with the shorter one
    Timer.Restart();
    for (int i = 0; i < PaddingSpeedTestNumber; ++i)
    {
        E2[i].mInteger = 2;
    }
    auto duration2 = Timer.GetDurationNanoSeconds();
    std::cout << "E2 time : " << duration2 << "ns\n";
    std::cout.flush();

    std::cout << "E1 take " << static_cast<double>(duration1) / static_cast<double>(duration2) << " time longer than E2" << std::endl;

    delete[] E1;
    delete[] E2;

    PrintSpace();
}
#pragma endregion PaddingSpeedTest


#pragma region SameSizeArrayLoopIteration
//Size is 72 bytes (8 bytes alignment) -> more than 64 bytes (i'm currently compiling in x64)
struct HeavyExemple
{
    double D1;
    double D2;
    int* intPtr{ nullptr };
    Exemple1 E1;
    Exemple2 E2;
    char* Name;
    short S;
    bool B;
    double D3;
};

#define ThreeSameSizeArrayIterationNumber 1'000'000
static void ThreeSameSizeArrayIteration()
{
    Exemple1* E1 = new Exemple1[ThreeSameSizeArrayIterationNumber]{};
    Exemple2* E2 = new Exemple2[ThreeSameSizeArrayIterationNumber]{};
    HeavyExemple* E3 = new HeavyExemple[ThreeSameSizeArrayIterationNumber]{};

    //Iterate through two differents array in the same loop
    Chrono Timer;
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E1[i].mInteger = 2;
        E2[i].mInteger = 2;
        E3[i].D1 += 104;
    }
    auto duration1 = Timer.GetDurationNanoSeconds();
    std::cout << "Two arrays in the same loop : " << duration1 << "ns\n";
    std::cout.flush();

    //Now one loop for each array
    Timer.Restart();
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E1[i].mInteger = 4;
    }
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E2[i].mInteger = 4;
    }
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E3[i].D1 += 104;
    }
    auto duration2 = Timer.GetDurationNanoSeconds();
    std::cout << "One loop per array : " << duration2 << "ns\n";
    std::cout.flush();

    std::cout << "One loop take " << static_cast<double>(duration1) / static_cast<double>(duration2) << " time longer than two loops" << std::endl;

    delete[] E1;
    delete[] E2;
    delete[] E3;

    PrintSpace();
}
#pragma endregion SameSizeArrayLoopIteration


#pragma region HotColdSplitting
struct LightData
{
    double D1;
    double D2;
    int I1;
    int I2;
};

struct LightHotData
{
    struct LightColdData
    {
        double D2;
        int I1;
        int I2;
    };
    LightHotData() { Data = new LightColdData(); }
    ~LightHotData() { delete Data; }
    LightColdData* Data{ nullptr };
    double D1;
};


struct HeavyHotData
{
    struct HeavyColdData
    {
        int* intPtr{ nullptr };
        Exemple1 E1;
        Exemple2 E2;
        char* Name;
        short S;
        bool B;
        double D3;
    };

    HeavyHotData() { Data = new HeavyColdData(); }
    ~HeavyHotData() { delete Data; }
    double D1;
    double D2;
    HeavyColdData* Data{ nullptr };
};


#define HotColdSplittingNumber 1'000'000
static void HotColdSplitting()
{
    LightData* E1 = new LightData[HotColdSplittingNumber]{};
    LightHotData* E2 = new LightHotData[HotColdSplittingNumber]{};

    //All data inside the struct
    Chrono Timer;
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E1[i].D1 = 2;
    }
    auto duration1 = Timer.GetDurationNanoSeconds();
    std::cout << "LIGHT All datas together : " << duration1 << "ns\n";
    std::cout.flush();

    //Cold data are stored as a pointer inside and are not accessed here
    Timer.Restart();
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        E2[i].D1 = 2;
    }

    auto duration2 = Timer.GetDurationNanoSeconds();
    std::cout << "LIGHT Hot datas access only : " << duration2 << "ns\n";
    std::cout.flush();

    std::cout << "LIGHT All datas take " << static_cast<double>(duration1) / static_cast<double>(duration2)
        << " time longer than LIGHT Hot datas access" << std::endl;

    delete[] E1;
    delete[] E2;


    //Same thing with a larger struct
    HeavyExemple* HE = new HeavyExemple[HotColdSplittingNumber]{};
    HeavyHotData* HD = new HeavyHotData[HotColdSplittingNumber]{};

    //All data inside the struct
    Timer.Restart();
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        HE[i].D1 = 2;
    }
    auto duration3 = Timer.GetDurationNanoSeconds();
    std::cout << "HEAVY All datas together : " << duration1 << "ns\n";
    std::cout.flush();

    //Cold data are stored as a pointer inside and are not accessed here
    Timer.Restart();
    for (int i = 0; i < ThreeSameSizeArrayIterationNumber; ++i)
    {
        HD[i].D1 = 2;
    }

    auto duration4 = Timer.GetDurationNanoSeconds();
    std::cout << "HEAVY Hot datas access only : " << duration2 << "ns\n";
    std::cout.flush();

    std::cout << "HEAVY All datas take " << static_cast<double>(duration3) / static_cast<double>(duration4) 
                << " time longer than HEAVY Hot datas access" << std::endl;

    delete[] HE;
    delete[] HD;

    PrintSpace();
}
#pragma endregion HotColdSplitting


#pragma region StructureOfArray_VS_ArrayOfStructure
#define StructureOfArray_VS_ArrayOfStructureNumber 1'500'000
//Here we got some padding for each instance of this struct
struct SimpleStruct
{
    unsigned int mInt;
    char Letter;
};

//here all the datas are correctly aligned thanks to the arrays
//->in this case, one ArraStruct instance cost more size, but this cost will be negligible when we want a lot of datas
//the number of data need to exceed the size cache for the gaim to be substantial
struct ArrayStruct
{
    ArrayStruct(int size) 
    {
        mArrayInt = new unsigned int[size];
        mArrayLetter = new char[size];
    }
    ~ArrayStruct()
    {
        delete[] mArrayInt;
        delete[] mArrayLetter;
    }

    unsigned int* mArrayInt{ nullptr };
    char* mArrayLetter{ nullptr };
};

static void ArrayOfStructure_VS_StructureOfArrays()
{
    SimpleStruct* ArrayOfSimpleStruct = new SimpleStruct[StructureOfArray_VS_ArrayOfStructureNumber];
    ArrayStruct* StructOfArrays = new ArrayStruct(StructureOfArray_VS_ArrayOfStructureNumber);


    //Start with the array of struct
    Chrono Timer;
    for (int i = 0; i < StructureOfArray_VS_ArrayOfStructureNumber; ++i)
    {
        ArrayOfSimpleStruct[i].mInt = 2;
    }
    auto duration1 = Timer.GetDurationNanoSeconds();
    std::cout << "ArrayOfSimpleStruct time : " << duration1 << "ns\n";
    std::cout.flush();

    //Then with the struct of arrays
    Timer.Restart();
    for (int i = 0; i < StructureOfArray_VS_ArrayOfStructureNumber; ++i)
    {
        StructOfArrays->mArrayInt[i] = 2;
    }
    auto duration2 = Timer.GetDurationNanoSeconds();
    std::cout << "StructOfArrays time : " << duration2 << "ns\n";
    std::cout.flush();

    std::cout << "ArrayOfSimpleStruct take " << static_cast<double>(duration1) / static_cast<double>(duration2) << " time longer than StructOfArrays" << std::endl;

    delete[] ArrayOfSimpleStruct;
    delete StructOfArrays;
    PrintSpace();
}
#pragma endregion StructureOfArray_VS_ArrayOfStructure


int main()
{
    //Less memory size is more cpu efficient, we need to be careful of our padding to reduce memory usage and
    //obtain more cache hit / more data pulled at the same time by the bus ?
    PaddingSpeedTest(); 

    //The timing are generally close, but can vary by a large amount in one or other direction sometimes, why ? Is it my compilator ?
    ThreeSameSizeArrayIteration(); 

    //We can hide a part of our objects under a pointer -> less size in memory when we don't need to access data stored inside it
    //-> program run quicker
    //NOTE : if the ColdStruct is not stored through a pointer, we loose some speed
    HotColdSplitting();

    //Strangely, results are different between Debug and Release. In Debug, ArrayOfStructure is generally a little quicker,
    //but in Release, the struct of Arrays is WAY MORE EFFICIENT
    ArrayOfStructure_VS_StructureOfArrays();
    return 0;
}