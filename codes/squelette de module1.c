/************************************************************************\

  Squelette minimal de module du noyau

\************************************************************************/

#include <linux/module.h>

int __init initialisation_module(void)
{
	/* initialisation */

	return 0;
}


void __exit suppression_module(void)
{
	/* liberation */

}


module_init(initialisation_module);
module_exit(suppression_module);

