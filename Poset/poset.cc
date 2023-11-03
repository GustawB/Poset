#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
#include <cstring>
#include "poset.h"

#ifdef NDEBUG
bool constexpr debug = false;
#else
bool constexpr debug = true;
#endif

using std::unordered_map;
using std::unordered_set;
using std::string;
using std::cerr;

using poset_map = unordered_map<unsigned long, unordered_map<string*,
	unordered_set<string*>>>;
using data_container = unordered_map<unsigned long, unordered_set<string>>;

//Mapping of the poset's id to all its relations.
//It wil hold pointers to strings in poset_elements.
poset_map& poset_collection() {
	static poset_map* poset_collection = new poset_map();
	return *poset_collection;
}

//Mapping of the poset's id to all its elements.
//It will hold strings.
data_container& poset_elements() {
	static data_container* poset_elements = new data_container();
	return *poset_elements;
}

namespace {
	//Keeps the information about the id of the last created poset.
	//Used to create new posets with unique ids.
	unsigned long last_id = 0;

	//Searches for the poset with the given id in the poset_collection.
	bool findKeyInCollection(unsigned long id) {
		if (poset_collection().find(id) != poset_collection().end()) {
			return true;
		}
		else {
			return false;
		}
	}

	//Searches for the poset in the poset_elements.
	bool findKeyInElements(unsigned long id) {
		if (poset_elements().find(id) != poset_elements().end()) {
			return true;
		}
		else {
			return false;
		}
	}

	//Checks whether the given value is in poset marked with the id.
	bool findValueInPoset(unsigned long id, char const* value) {
		if (poset_elements()[id].find(value) != poset_elements()[id].end()) {
			return true;
		}
		else {
			return false;
		}
	}

	//Checks whether the given value is in relation with something.
	bool findValueInCollection(unsigned long id, char const* value) {
		auto valueIter = poset_elements()[id].find(value);
		if (poset_collection()[id].find((std::string*)(&*valueIter)) !=
			poset_collection()[id].end()) {
			return true;
		}
		else {
			return false;
		}
	}

	//Checks whether the value1 is the parent of the value2.
	//Said operation is realised as BFS.
	bool findParent(unsigned long id, char const* value1, char const* value2) {
		std::queue<std::string*> BSTqueue;
		auto value1Iter = poset_elements()[id].find(value1);
		auto value2Iter = poset_elements()[id].find(value2);
		for (std::string* s : poset_collection()[id][(std::string*)(&*value1Iter)]) {
			BSTqueue.push(s);
		}

		while (!BSTqueue.empty()) {
			std::string* s = BSTqueue.front();
			BSTqueue.pop();
			if (s == (std::string*)(&*value2Iter)) {
				//value1 is the parent of the value2.
				return true;
			}
			else {
				//We continue BFS algorithm.
				for (std::string* s2 : poset_collection()[id][s]) {
					BSTqueue.push(s2);
				}
			}
		}
		//We didn't find value1, so it's not a parent of the value2.
		return false;
	}

	//Checks whether the value1 is the parent of the value2.
	//Said operation is realised as BFS.
	bool findParent(unsigned long id, std::string* value1, char const* value2) {
		std::queue<std::string*> BSTqueue;
		auto value1Iter = poset_elements()[id].find(*value1);
		auto value2Iter = poset_elements()[id].find(value2);
		for (std::string* s : poset_collection()[id][(std::string*)(&*value1Iter)]) {
			BSTqueue.push(s);
		}

		while (!BSTqueue.empty()) {
			std::string* s = BSTqueue.front();
			BSTqueue.pop();
			if (s == (std::string*)(&*value2Iter)) {
				//value1 is the parent of the value2.
				return true;
			}
			else {
				//We continue BFS algorithm.
				for (std::string* s2 : poset_collection()[id][s]) {
					BSTqueue.push(s2);
				}
			}
		}
		//We didn't find value2, so value1 is not its parent.
		return false;
	}

	//Function checks, if the given string is NULL
	string ifNULL(const char* value) {
		if (value == nullptr) {
			return "NULL";
		}
		return "\"" + string(value) + "\"";
	}
}

unsigned long cxx::poset_new(void) {
	if constexpr (debug) {
		cerr << "poset_new()" << "\n";
	}

	unsigned long id = last_id;
	++last_id;
	poset_elements()[id] = std::unordered_set<std::string>();
	poset_collection()[id] = std::unordered_map<std::string*,
		std::unordered_set<std::string*>>();

	if constexpr (debug) {
		cerr << "poset_new: poset " << id << " created" << "\n";
	}

	return id;
}

size_t cxx::poset_size(unsigned long id) {
	if constexpr (debug) {
		cerr << "poset_size(" << id << ")" << "\n";
	}

	if (findKeyInElements(id)) {
		if constexpr (debug) {
			cerr << "poset_size: poset " << id
				<< " contains " << poset_elements()[id].size()
				<< " element(s)" << "\n";
		}
		return poset_elements()[id].size();
	}
	else {
		if constexpr (debug) {
			cerr << "poset_size: poset " << id
				<< " does not exist" << "\n";
		}

		return 0;
	}
}

bool cxx::poset_remove(unsigned long id, char const* value) {
	string s = ifNULL(value);

	if constexpr (debug) {
		cerr << "poset_remove(" << id << ", " << s << ")" << "\n";
	}

	if (value == NULL) {
		//We can't remove NULL;
		if constexpr (debug) {
			cerr << "poset_remove: invalid value (NULL)" << "\n";
		}

		return false;
	}

	//Poset with the given id doesn't exist.
	if (!findKeyInElements(id)) {
		if constexpr (debug) {
			cerr << "poset_remove: " << "poset " << id
				<< " does not exist" << "\n";
		}

		return false;
	}
	else if (findValueInPoset(id, value)) {
		//Poset exists and is not empty and the value is in poset.
		bool bIsParentInCollection = findValueInCollection(id, value);
		auto elementToRemove = poset_elements()[id].find(value);
		for (auto &pair : poset_collection()[id]) {
			if (pair.second.find((std::string*)(&*elementToRemove)) != pair.second.end()) {
				//Value is the parent of something, we need to re-map the relation.
				if (bIsParentInCollection) {
					for (std::string* s : poset_collection()[id]
						[(std::string*)(&*elementToRemove)]) {
						pair.second.insert(s);
					}
				}
				pair.second.erase(pair.second.find((std::string*)(&*elementToRemove)));
			}
		}
		if (bIsParentInCollection) {
			//If value was a parent of something, we need to remove it
			//from the poset_collection.
			poset_collection()[id].erase(
				poset_collection()[id].find((std::string*)(&*elementToRemove)));
		}
		//Remove element from th list of the poset's elements.
		poset_elements()[id].erase(value);
		if constexpr (debug) {
			cerr << "poset_remove: poset " << id << " , element " << s << " removed" << "\n";
		}
		return true;
	}
	else {
		if constexpr (debug) {
			cerr << "poset_remove: " << id << ", element " << s << " does not exist" << "\n";
		}

		return false;
	}
}

bool cxx::poset_del(unsigned long id, char const* value1,
	char const* value2) {
	string s1 = ifNULL(value1);
	string s2 = ifNULL(value2);

	if constexpr (debug) {
		cerr << "poset_del(" << id << ", " << s1 << ", " << s2 << ")" << "\n";
	}

	if (value1 == NULL || value2 == NULL) {
		//We can't delete relation between NULLs.
		if constexpr (debug) {
			if (value1 == NULL) {
				cerr << "poset_del: invalid value1 (NULL)" << "\n";
			}

			if (value2 == NULL) {
				cerr << "poset_del: invalid value2 (NULL)" << "\n";
			}
		}

		return false;
	}

	//Poset doesn't exist or at least one of the values doesn't exist. 
	if (!findKeyInElements(id) || !findValueInPoset(id, value1) ||
		!findValueInPoset(id, value2)) {
		if constexpr (debug) {
			if (!findKeyInElements(id)) {
				cerr << "poset_del: poset " << id
					<< " does not exist" << "\n";
			}

			if (!findValueInPoset(id, value1)) {
				cerr << "poset_del: poset" << id << ", element " << s1
					<< " does not exist" << "\n";
			}

			if (!findValueInPoset(id, value2)) {
				cerr << "poset_del: poset" << id << ", element " << s2
					<< " does not exist" << "\n";
			}
		}
		return false;
	}

	//Both values exist in the given poset.
	if (strcmp(value1, value2) == 0) {
		//We can't delete a->a relation.
		if constexpr (debug) {
			cerr << "poset_del: poset " << id << " relation ("
				<< s1 << "," << s2 << ") cannot be deleted" << "\n";
		}
		return false;
	}
	auto value1Iter = poset_elements()[id].find(value1);
	auto value2Iter = poset_elements()[id].find(value2);

	if (poset_collection()[id][(std::string*)(&*value1Iter)]
		.find((std::string*)(&*value2Iter)) !=
		poset_collection()[id][(std::string*)(&*value1Iter)].end()) {
		//Value1 and Value2 are in a relation.
		for (std::string* elem : poset_collection()[id][(std::string*)(&*value1Iter)]) {
			if (elem != (std::string*)(&*value2Iter)) {
				//Value2 isn't strictly after ther value1 in the relation chain.
				if (findParent(id, elem, value2)) {
					//If we have following relations: b->c, c->d, b->d,
					//then we can't delete the relation.
					if constexpr (debug) {
						cerr << "poset_del: poset " << id << " relation ("
							<< s1 << "," << s2 << ") cannot be deleted" << "\n";
					}
					return false;
				}
			}
		}
		for (auto& pair : poset_collection()[id]) {
			if (pair.second.find((std::string*)(&*value1Iter)) !=
				pair.second.end()) {
				//a->b->c, deleting b->c, we need to remap it so that a->c.
				pair.second.insert((std::string*)(&*value2Iter));
			}
		}
		if (poset_collection()[id].find((std::string*)(&*value2Iter)) !=
			poset_collection()[id].end()) {
			//a->b->c, deleting a->b, we need to remap it so that a->c
			for (std::string* elem : (poset_collection()[id][(std::string*)(&*value2Iter)])) {
				poset_collection()[id][(std::string*)(&*value1Iter)].insert(elem);
			}
		}
		poset_collection()[id][(std::string*)(&*value1Iter)].erase((std::string*)(&*value2Iter));

		if constexpr (debug) {
			cerr << "poset_del: poset " << id << " relation (" <<
				s1 << "," << s2 << ") deleted" << "\n";
		}

		return true;
	}
	else {
		//Elements aren't in a relation, we delete nothing.
		if constexpr (debug) {
			cerr << "poset_del: poset " << id << " relation ("
				<< s1 << "," << s2 << ") cannot be deleted" << "\n";
		}
		return false;
	}
}

void cxx::poset_delete(unsigned long id) {
	if constexpr (debug) {
		cerr << "poset_delete(" << id << ")" << "\n";
	}

	auto posetToDeleteIter = poset_collection().find(id);
	auto posetElementsToDeleteIter = poset_elements().find(id);

	if (posetToDeleteIter != poset_collection().end()) {
		//We found the given poset in the poset_collection.
		poset_collection().erase(posetToDeleteIter);
	}

	if (posetElementsToDeleteIter != poset_elements().end()) {
		//We found poset's elements in the poset_elements.
		poset_elements().erase(posetElementsToDeleteIter);
		if constexpr (debug) {
			cerr << "poset_delete: poset " << id << " deleted" << "\n";
		}
	}
	else if constexpr (debug) {
		//If pose isn't in poset_elements, it means that it doesn't exist.
		cerr << "poset_delete: poset " << id << " does not exist" << "\n";
	}
}

bool cxx::poset_insert(unsigned long id, char const* value) {
	string s = ifNULL(value);

	if constexpr (debug) {
		cerr << "poset_insert(" << id << ", " << s << ")" << "\n";
	}

	if (value == NULL) {
		//We can't add null value;
		if constexpr (debug) {
			cerr << "poset_insert: invalid value1 (NULL)" << "\n";
		}
		return false;
	}
	else if (findKeyInElements(id)) {
		//We found the given poset, we can try to insert a new
		//element into it.
		if (!findValueInPoset(id, value)) {
			//Current poset doesn't contain the value, so we can 
			//insert it into poset.
			poset_elements()[id].insert(std::string(value));
			if constexpr (debug) {
				cerr << "poset_insert: poset " << id << ", element "
					<< s << " inserted" << "\n";
			}
			return true;
		}
		else {//Poset already contains the value, we add nothing.
			if constexpr (debug) {
				cerr << "poset_insert: poset " << id << ", element "
					<< s << " already exists" << "\n";
			}
			return false;
		}
	}
	else {//We inserted nothing, because the given poset doesn't exist.
		if constexpr (debug) {
			cerr << "poset_insert: poset " << id <<
				" does not exist" << "\n";
		}
		return false;
	}
}

bool cxx::poset_add(unsigned long id, char const* value1,
	char const* value2) {
	string s1 = ifNULL(value1);
	string s2 = ifNULL(value2);

	if constexpr (debug) {
		cerr << "poset_add(" << id << ", " << s1 << ", "
			<< s2 << ")" << "\n";
	}

	if (value1 == NULL || value2 == NULL) {
		//We can't delete relation between NULLs.
		if constexpr (debug) {
			if (value1 == NULL) {
				cerr << "poset_add: poset invalid value1 (NULL)" << "\n";
			}

			if (value2 == NULL) {
				cerr << "poset_add: poset invalid value2 (NULL)" << "\n";
			}
		}

		return false;
	}
	else if (findKeyInCollection(id)) {
		//Poset with the given id exists.
		if (findValueInPoset(id, value1) && findValueInPoset(id, value2)) {
			//Both values are in the poset.
			if (strcmp(value1, value2) == 0) {
				//We are trying to add a relation betweene the same element, and that relation already exists.
				cerr << "poset_add: poset " << id << ", relation (" << s1
					<< ", " << s2 << ") cannot be added" << "\n";
				return false;
			}
			else if ((findValueInCollection(id, value1) &&
				findParent(id, value1, value2)) ||
				(findValueInCollection(id, value2) &&
					findParent(id, value2, value1))) {
				//if value2 is already a parent of the value 1, or value1
				//is a parent of the value2, we don't want to add a new relation.
				if constexpr (debug) {
					cerr << "poset_add: poset " << id << ", relation (" << s1
						<< ", " << s2 << ") cannot be added" << "\n";
				}
				return false;
			}
			else {//Adds value2 to the set containing children of the value1.
				auto value1Iter = poset_elements()[id].find(value1);
				auto value2Iter = poset_elements()[id].find(value2);
				poset_collection()[id][(std::string*)(&*value1Iter)]
					.insert((std::string*)(&*value2Iter));
				if constexpr (debug) {
					cerr << "poset_add: poset " << id << ", relation (" << s1
						<< ", " << s2 << ") added" << "\n";
				}
				return true;
			}
		}
		else {//At least one value isn't in the poset, we add nothing.
			if constexpr (debug) {
				if (!findValueInPoset(id, value1)) {
					cerr << "poset_add: poset " << id << ", element " << s1
						<< " does not exist" << "\n";
				}

				if (!findValueInPoset(id, value2)) {
					cerr << "poset_add: poset " << id << ", element " << s2
						<< " does not exist" << "\n";
				}
			}

			return false;
		}
	}
	else {
		//Poset with the given id doesn't exist.
		if constexpr (debug) {
			cerr << "poset_add: poset " << id << " does not exist" << "\n";
		}
		return false;
	}
}

bool cxx::poset_test(unsigned long id, char const* value1,
	char const* value2) {
	string s1 = ifNULL(value1);
	string s2 = ifNULL(value2);

	if constexpr (debug) {
		cerr << "poset_test(" << id << ", " << s1 << ", " << s2
			<< ")" << "\n";
	}

	if (value1 == NULL || value2 == NULL) {
		//We can't delete relation between NULLs.
		if constexpr (debug) {
			if (value1 == NULL) {
				cerr << "poset_test: invalid value1 (NULL)" << "\n";
			}

			if (value2 == NULL) {
				cerr << "poset_test: invalid value2 (NULL)" << "\n";
			}
		}

		return false;
	}
	else if (findKeyInCollection(id)) {
		//Poset with the given id exists.
		if (strcmp(value1, value2) == 0) {
			//value1 is equal to value2.
			if (findValueInPoset(id, value1)) {
				//value1 exists and is in relation with itself.
				if constexpr (debug) {
					cerr << "poset_test: poset " << id << ", relation ("
						<< s1 << " , " << s1 << ")" << " exists" << "\n";
				}
				return true;
			}
			else {
				//value1 is not in the given poset.
				if constexpr (debug) {
					cerr << "poset_add: poset " << id << ", element "
						<< s2 << " does not exist" << "\n";
				}
				return false;
			}
		}
		else if (findValueInPoset(id, value1) && findValueInPoset(id, value2)) {
			//Both values are in the poset.
			if (findValueInCollection(id, value1) &&
				findParent(id, value1, value2)) {
				//Value1 is a parent of the value2.
				if constexpr (debug) {
					cerr << "poset_test: poset " << id << ", relation ("
						<< s1 << " , " << s2 << ")" << " exists" << "\n";
				}
				return true;
			}
			else {//Value1 is not a parent of the value2.
				if constexpr (debug) {
					cerr << "poset_test: poset " << id << ", relation ("
						<< s1 << " , " << s2 << ")" << " does not exist" << "\n";
				}
				return false;
			}
		}
		else {//At least one value isn't in the poset, we add nothing.
			if constexpr (debug) {
				cerr << "poset_test: poset " << id
					<< " does not exist" << "\n";
			}
			return false;
		}
	}
	else {
		//Poset with the given id doesn't exist.
		if constexpr (debug) {
			cerr << "poset_test: poset " << id << " does not exist" << "\n";
		}
		return false;
	}
}

void cxx::poset_clear(unsigned long id) {
	if constexpr (debug) {
		cerr << "poset_clear: poset (" << id << ")" << "\n";
	}

	if (findKeyInElements(id)) {
		//Poset with the given id exists.
		poset_elements()[id].clear();
		poset_collection()[id].clear();
		if constexpr (debug) {
			cerr << "poset_clear: poset " << id << " cleared" << "\n";
		}
	}
	else if constexpr (debug) {//Poset with the given id doesn't exists.
		cerr << "poset_clear: poset " << id << " does not exist" << "\n";
	}
}