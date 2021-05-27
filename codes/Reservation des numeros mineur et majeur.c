/************************************************************************\
  exemple_01 - Chapitre "Ecriture de driver - peripherique caractere"

  Reservation des numeros mineur et majeur

  

\************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>

	static dev_t exemple_dev = MKDEV(0, 0);

	static int numero = 0;
	module_param(numero, int, 0644);

static int __init exemple_init (void)
{
	int erreur;
	if (numero == 0) {
		erreur = alloc_chrdev_region(& exemple_dev, 0, 1, THIS_MODULE->name);
	} else {
		exemple_dev = MKDEV(numero, 0);
		erreur = register_chrdev_region(exemple_dev, 1, THIS_MODULE->name);
	}
	if (erreur < 0)
		return erreur;
	return 0; 
}

static void __exit exemple_exit (void)
{
	unregister_chrdev_region(exemple_dev, 1);
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

