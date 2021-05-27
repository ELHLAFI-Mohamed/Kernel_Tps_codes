#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#define PROCFS_MAX_SIZE		1024


	static dev_t exemple_dev = MKDEV(0, 0);
  int compteur=0 ;
  static char procfs_buffer[PROCFS_MAX_SIZE];

static unsigned long procfs_buffer_size = 0;
	static int numero = 0;
  int r=0 ;
	module_param(numero, int, 0644);

	static struct cdev exemple_cdev;

	static ssize_t exemple_read  (struct file * filp, char * buffer,
	                          size_t length, loff_t * offset);
  static ssize_t exemple_write(struct file *filp, const char *buffer, size_t count, loff_t *off);

	static struct file_operations fops_exemple = {
		.owner   =  THIS_MODULE,
		.read    =  exemple_read,
    .write   =  exemple_write,
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

static ssize_t exemple_read(struct file * filp, char * buffer,
                        size_t length, loff_t * offset)
{
  char chaine[128];
	int lg;



	snprintf(chaine, 128, "Compteur= %u\n",compteur);

	lg = strlen(chaine) + 1;

	lg -= *offset;
	if (lg <= 0) {
		printk("-> 0\n");
		return 0;
	}
	if (length < lg)
		lg = length;

	if (copy_to_user(buffer, & chaine[* offset], lg) != 0) {
		printk("-> Erreur\n");
		return -EFAULT;
	}
	*offset += lg;
 compteur++;
	printk("-> %d\n", lg);
	return lg;
}
static ssize_t exemple_write(struct file *filp, const char *buffer, size_t count, loff_t *off)
{
	/* get buffer size */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		return -EFAULT;
	}
compteur=simple_strtol(procfs_buffer,NULL,10);

	return procfs_buffer_size;
}


module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");
