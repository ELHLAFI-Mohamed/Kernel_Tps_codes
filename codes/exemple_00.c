/************************************************************************\

  Squelette minimal de module du noyau

\************************************************************************/

#include <linux/module.h>

MODULE_AUTHOR("Romadi Rahal");
MODULE_DESCRIPTION("Ceci est un squelette de module");
MODULE_LICENSE("GPL");

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

