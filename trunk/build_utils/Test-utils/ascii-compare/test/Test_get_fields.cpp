
#include "get_fields.h"
///////////////////////////////////////////////////////////////////////////////
// Declarations
void compareResults ( std::vector<double>& result, std::vector<double>& expected );
///////////////////////////////////////////////////////////////////////////////
template<typename T>
void print_vec ( std::vector<T>& vec, std::string comment )
{
    std::cout << comment << std::endl;
    for ( std::size_t i=0; i< vec.size(); ++i )
        std::cout << vec[i] << ' ';
    std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// Main test function:
int Test_get_fields ( int , char*[] )
{
    const int ret_error = 1;
    const int ret_ok = 0;

    try
    {
        // Results are put into a vector of doubles:
        std::vector<double> res_vec;
        // creating a test string:
        std::string test_str ( "23.45 44 4 678.3 7 12 67 0 -34.3456" );

        std::vector<double> expected;
        expected.resize ( 4 );
        expected[0] = 23.45; expected[1]=4; expected[2]= 678.3; expected[3]= 12;

        // Create vector of indexes:
        std::vector<int> idx_vec;
        idx_vec.resize ( 4 );
        idx_vec[0] = 0; idx_vec[1] = 5; idx_vec[2] = 2; idx_vec[3] = 3;

        print_vec ( idx_vec, "Indexes to get:" );

        res_vec = get_fields<double> ( test_str, idx_vec );

        print_vec ( idx_vec, "Indexes after sort:" );
        print_vec ( res_vec, "Numbers retrieved:" );

        if ( res_vec.size() != idx_vec.size() )
        {
            std::cout << "Mis-match in number of indexes and number of retrieved arguments"<< std::endl;
            throw ( 1 );
        }

        // Now I do the same thing wth a comma separated list:
        test_str = "23.45,44, 4, 678.3, 7, 12, 67, 0, -34.3456";
		  std::cout << "Processing test string: " << test_str << std::endl;
        res_vec = get_fields<double> ( test_str, idx_vec, ',' );

		  print_vec ( idx_vec, "Indexes after sort:" );
        print_vec ( res_vec, "Numbers retrieved:" );

        if ( res_vec.size() != idx_vec.size() )
        {
            std::cout << "Mis-match in number of indexes and number of retrieved arguments"<< std::endl;
            throw ( 1 );
        }
    }
    catch ( int& ret_val )
    {
        return ret_val;
    }
    catch ( ... )
    {
        return ret_error;
    }

    return ret_ok;
}
///////////////////////////////////////////////////////////////////////////////
void compareResults ( std::vector<double>& result, std::vector<double>& expected )
{
    if ( result.size() != expected.size() )
    {
        std::cout << "Mis-match in number of retrieved arguments:"<< std::endl
                  << "Expected: " << expected.size() << ", but got: "<< result.size() << std::endl;
        throw(1);
    }

    for ( std::size_t i=0; i<expected.size(); ++i )
    {
        if ( result[i] != expected[i] )
        {
            std::cout << "Unexpected result at index "<< i << std::endl;
            std::cout << "Found: "<< i << result[i] << ", expected: " << expected[i] << std::endl;
            throw(1);
        }
    }
}

























