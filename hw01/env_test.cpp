#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main()
{
    string env_var = "HOME";
    char* env_p = getenv(env_var.c_str());
    if (env_p == NULL)
    {
        cout << "Environment variable " << env_var << " is not set." << endl;
    }
    else
    {
        cout << "Environment variable " << env_var << " is set to " << env_p << endl;
    }
    return 0;
}