#ifndef POSET_H
#define POSET_H

#ifndef __cplusplus
#include <stdlib.h>
#include <stdbool.h>
#else
#include <iostream>
#include <cstdlib>
#endif

#ifdef __cplusplus
namespace cxx {
	extern "C" {
#endif
		/*
		* poset_new() creates new poset, add it to the
		* poset_collection and poset_elements structures
		* and returns its id.
		*/
		unsigned long poset_new(void);

		/*
		* This function deletes the given poset if it exists,
		* and does nothing otherwise.
		*/
		void poset_delete(unsigned long id);

		/*
		* Return the number of elements inside the given poset. If it doesn't
		* exist, zero is returned.
		*/
		size_t poset_size(unsigned long id);

		/*
		* If the given value doesn't exist inside the provided poset,
		* and the said poset exists, this function inserts this value
		* inside it. Otherwise, it does nothing.
		*/
		bool poset_insert(unsigned long id, char const* value);

		/*
		* Removes the given value and all its relations from the given poset.
		* If the given poset or value doesn't exist, this function returns false,
		* and tru, if removal was successful.
		*/
		bool poset_remove(unsigned long id, char const* value);

		/*
		* If a poset with the given id exists, and both of the provided values
		* aren't contained inside it, this function adds them to the poset
		* in a way, that value1 will be before value2 in the relation.
		* If at least one ot these conditions isn't met, this function
		* does nothing.
		*/
		bool poset_add(unsigned long id, char const* value1, char const* value2);

		/*
		* If poset with the given id exists, and so do value1 and value2, AND 
		* value1 is th eparent of the value2, AND the removal of the relation
		* between them won't disturb the partial order, this function deletes
		* the relation and returns true. Otherwise, it returns false.
		*/
		bool poset_del(unsigned long id, char const* value1, char const* value2);

		/*
		* Checks whether the given poset exists, that it contains values one
		* and two, and if value1 < value2. If all if these conditions are met,
		* poset_test returns true, and false otherwise.
		*/
		bool poset_test(unsigned long id, char const* value1, char const* value2);

		/*
		* If the given poset exists, this function removes all its elements and 
		* relations between them, and otherwise, it does nothing.
		*/
		void poset_clear(unsigned long id);
#ifdef __cplusplus
	}
}
#endif

#endif