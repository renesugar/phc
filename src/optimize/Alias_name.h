/*
 * phc -- the open source PHP compiler
 * 
 * Representation of a variable_name/object.
 */
 
/*
 * Every 'memory_location' modelled must have a unique name. We previously
 * used strings to represent these, but we dont want to be manipulating
 * strings just to find out what function a name is in.
 *
 * The alias_name is either an Index_node, in which case the first part is a
 * function_name, and the second is the variable name. Otherwise its a
 * storage node, in which case the first part is "ST", and the second is some
 * unique name based on the program point of its defintion.
 *
 * This abstracts over the name. The only important part is that they are
 * unique. However, is order to trim the results, some passes may wish to
 * rely on the first part of the name being the function_name. That's fine.
 */

#ifndef PHC_ALIAS_NAME
#define PHC_ALIAS_NAME

#define CFG_ST(CFG) (*(CFG)->method->signature->method_name->value)
#define ST(BB) (CFG_ST ((BB)->cfg))

// Storage node prefix
#define SNP "ST"
#define RETNAME "__RETNAME__"
#define MSN "__MAIN__"

#include <string>
#include "lib/List.h"

class Index_node;

class Alias_name
{
public:
	Alias_name ();
	Alias_name (std::string prefix, std::string name);

	std::string prefix;
	std::string name;
	int ssa_version;

	// Get an index onde with the same name. Checks that this is legal, and
	// that we arent in SSA form.
	Index_node* ind();

	bool operator< (const Alias_name& other) const;
	bool operator== (const Alias_name& other) const;

	// In some cases (at least lattice_map, maybe elsewhere), its hard to put
	// use an Alias_name instead of a string.
	std::string str ();

	void set_version (int version);
	void drop_ssa_version ();
};

class Basic_block;
Alias_name v2an (Basic_block* bb, MIR::VARIABLE_NAME* var);

typedef List<Alias_name> old_Alias_name_list;
typedef List<Alias_name*> Alias_name_list;

#endif // PHC_ALIAS_NAME
