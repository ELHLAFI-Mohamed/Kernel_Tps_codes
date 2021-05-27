/************************************************************************\
  exemple_01 - Chapitre "Programmer pour le noyau Linux"

  Messages dans les traces du noyau au chargement et dechargement
  du module.

\************************************************************************/

#include <linux/module.h>

static int __init exemple_01_init (void)
{
	printk(KERN_INFO "%s : Bonjour\n", THIS_MODULE->name);
	return 0; 
}
 
static void __exit exemple_01_exit (void)
{
	printk(KERN_INFO "%s : Au revoir\n", THIS_MODULE->name);
}

module_init(exemple_01_init);
module_exit(exemple_01_exit);

MODULE_LICENSE("GPL");

