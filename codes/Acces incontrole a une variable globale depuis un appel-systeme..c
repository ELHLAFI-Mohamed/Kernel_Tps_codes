/************************************************************************\
  exemple_06 - Chapitre "Ecriture de driver - peripherique caractere"

  Acces incontrole a une variable globale depuis un appel-systeme.

\************************************************************************/

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include <asm/uaccess.h>


	static dev_t exemple_dev = MKDEV(0, 0);

	static int numero = 0;
	module_param(numero, int, 0644);

	static struct cdev exemple_cdev;

	static ssize_t exemple_read  (struct file * filp, char * buffer,
	                          size_t length, loff_t * offset);

	static struct file_operations fops_exemple = {
		.owner   =  THIS_MODULE,
		.read    =  exemple_read,
	};

	static int current_pid = 0;

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

static ssize_t exemple_read(struct file * filp, char * buffer,
                        size_t length, loff_t * offset)
{
	char chaine[2];
	unsigned long limite;

	current_pid = current->pid;

	limite = jiffies + 10;
	
	/* Boucle de 10 tick pour provoquer artificiellement
	  la collision entre des appels-systeme simultanes.*/
	  
	while (time_before(jiffies, limite))
		schedule();
	/* on peut remplacer schedule() par cpu_relax() sur
	   un systeme preemptible */

	if (current_pid == current->pid)
		strcpy(chaine, ".");
	else
		strcpy(chaine, "#");
	
	if (length < 2)
		return -ENOMEM;
	if (copy_to_user(buffer, chaine, 2) != 0)
		return -EFAULT;
		
	return 1;
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");
