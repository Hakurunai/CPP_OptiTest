#ifndef CHRONO__H
#define CHRONO__H

#include <chrono>


class Chrono
{
public :    
    Chrono() 
    {
        mStart = std::chrono::high_resolution_clock::now();        
    }

	~Chrono() = default;

    Chrono(Chrono& other) = delete;
    Chrono(Chrono&& other) = delete;
    Chrono& operator=(const Chrono& other) = delete;

    __forceinline long long GetDurationMilliSeconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mStart).count();
    }

    __forceinline long long GetDurationMicroSeconds()
    {
	    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - mStart).count();
    }

    __forceinline long long GetDurationNanoSeconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count();
    }

    __forceinline void Restart() 
    {
        mStart = std::chrono::high_resolution_clock::now();
    }

private :
    std::chrono::time_point<std::chrono::steady_clock> mStart;
};

#endif // CHRONO__H