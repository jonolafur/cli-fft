#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

///////////////////////////////////////////////////////////////////////////////
template<typename T>
bool compare(T x, T y, T tol)
{
   if(std::abs(x-y)>tol)
   {
      std::cout << "Expected value: " << y << " value obtained: " << x << std::endl;
      return false;
   }
   return true;
}

#endif // TEST_HELPERS_H
