// smps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "serializable.h"

int main()
{
	smps::serializable<10> ob;
	std::vector<std::pair<std::string, std::string>> vec;
	smps::ser_field_ind_class<1> a1;
	ob.ser_member(a1, vec);
    return 0;
}

