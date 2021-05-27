/************************************************************************\
  exemple_02 : Parametre configurable au chargement du module et via sysfs.
\************************************************************************/

#include <linux/module.h>

	static int param_entier = 0;
	module_param(param_entier, int, 0644);
	MODULE_PARM_DESC(param_entier, "une valeur entiere");

	static char * param_chaine = "-par defaut-";
	module_param(param_chaine, charp, 0644);
	MODULE_PARM_DESC(param_chaine, "une chaine de caracteres");

	static int param_table[8];
	static int nb_elements;
	module_param_array(param_table, int, & nb_elements, 0644);
        
static int __init exemple_02_init (void)
{
	int i;
	printk(KERN_INFO "%s (chargement) Entier = %d\n", THIS_MODULE->name, param_entier);
	printk(KERN_INFO "%s (chargement) Chaine = %s\n", THIS_MODULE->name, param_chaine);
	printk(KERN_INFO "%s (chargement) Nb elements = %d\n", THIS_MODULE->name, nb_elements);
	printk(KERN_INFO "%s (chargement) Table = ", THIS_MODULE->name);
	for (i = 0; i < nb_elements; i ++)
		printk("%d ", param_table[i]);
	printk("\n");
	return 0; 
}

static void __exit exemple_02_exit (void)
{
	int i;
	printk(KERN_INFO "%s (dechargement) Entier = %d\n", THIS_MODULE->name, param_entier);
	printk(KERN_INFO "%s (dechargement) Chaine = %s\n", THIS_MODULE->name, param_chaine);
	printk(KERN_INFO "%s (dechargement) Nb elements = %d\n", THIS_MODULE->name, nb_elements);
	printk(KERN_INFO "%s (dechargement) Table = ", THIS_MODULE->name);
	for (i = 0; i < nb_elements; i ++)
		printk("%d ", param_table[i]);
	printk("\n");
}

MODULE_LICENSE("GPL");

module_init(exemple_02_init);
module_exit(exemple_02_exit);

