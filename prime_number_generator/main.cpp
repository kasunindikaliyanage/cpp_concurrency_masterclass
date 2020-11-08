#include <iostream>
#include <cstring>
#include <thread>

#include <experimental/coroutine>

using namespace std;

void SieveOfEratosthenes(const unsigned int num) 
{
    const unsigned int array_size = num + 1;
    bool* pno = new bool[array_size];
   
    memset((void*)pno, true, sizeof(pno));
    
    for (int i = 2; i * i <= num; i++) 
    {
        if ( pno[i] == true )
        {
            for (int j = i * 2; j <= num; j += i)
            {
                pno[j] = false;
            }
        }
    }
    
    for (int i = 2; i <= num; i++)
    {
        if (pno[i])
            cout << i << " ";
    }
}


int main()
{
    int num = 15;
    cout << "The prime numbers smaller or equal to " << num << " are: ";
    SieveOfEratosthenes(num);
    return 0;
}