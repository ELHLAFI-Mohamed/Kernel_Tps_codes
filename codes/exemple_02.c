/************************************************************************\
  exemple_02 - Chapitre "Ecriture de driver - peripherique caractere"

  Fonctions d'ouverture et fermeture du fichier

\************************************************************************/

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
	static dev_t exemple_dev = MKDEV(0, 0);
	static int numero = 0;
	module_param(numero, int, 0644);
	static struct cdev exemple_cdev;

	static int exemple_open  (struct inode * ind, struct file * filp);
	static int exemple_release (struct inode * ind, struct file * filp);

	static struct file_operations fops_exemple = {
		.owner   =  THIS_MODULE,
		.open    =  exemple_open,
		.release =  exemple_release,
	};

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

	cdev_init(& exemple_cdev, & fops_exemple);

	erreur = cdev_add(& exemple_cdev, exemple_dev, 1);
	if (erreur != 0) {
		unregister_chrdev_region(exemple_dev, 1);
		return erreur;
	}
	return 0; 
}

static void __exit exemple_exit (void)
{
	cdev_del(& exemple_cdev);
	unregister_chrdev_region(exemple_dev, 1);
}

static int exemple_open(struct inode * ind, struct file * filp)
{
	printk(KERN_INFO "%s - exemple_open()\n", THIS_MODULE->name);
	return 0;
}

static int exemple_release(struct inode * ind, struct file * filp)
{
	printk(KERN_INFO "%s - exemple_release()\n", THIS_MODULE->name);
	return 0;
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

