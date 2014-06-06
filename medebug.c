/* 
 * medebug  Damagement Engine debug driver
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/pci.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include "me.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Damien Zammit");
MODULE_DESCRIPTION("Damagement Engine debug driver");

static int oldarc = 0;
dev_t me_dev;
struct cdev me_dev_file;

module_param(oldarc, int, 0000);
MODULE_PARM_DESC(oldarc, "If set to 1, assume 1.5MB Damagement Engine firmware");

//pci_read_config_dword(struct pci_dev *dev, int where, u32 *val);
//result = pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &myirq);
//if (result) {
///* deal with error */
//}
/*
static ssize_t memory_clone (struct class *class, struct class_attribute *attr, const char *buf, size_t len)
{
	return -EINVAL;
}
*/
static struct pci_device_id medebug_ids[ ] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x1c3a) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x1d3a) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x1e3a) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x2364) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x29b4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x29c4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x29d4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x29e4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x29f4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x2a04) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x2a14) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x2a44) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x2a50) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x8c3a) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x8d3a) },
	{ 0, },
};

MODULE_DEVICE_TABLE(pci, medebug_ids);

int medebug_open (struct inode* inode, struct file *filp) {
	printk("medebug: CLONE\n");
	
	printk("medebug: OPEN\n");

	return 0;
}

ssize_t medebug_read (struct file* filp, char __user * u, size_t len, loff_t * off) {
	printk("medebug: READ\n");
	return 0;
}

ssize_t medebug_write (struct file* filp, const char __user * u, size_t len, loff_t * off) {
	printk("medebug: FIXME: Write not implemented\n");
	return 0;
}

struct file_operations medebug_fops = {
	.owner = THIS_MODULE,
	.read = medebug_read,
	.write = medebug_write,
	.open =	medebug_open,
	.release = NULL,
};

int medebug_probe (struct pci_dev *dev, const struct pci_device_id *id)
{
	int err;
	struct class *me_cl;

	err = pci_enable_device(dev);
	if (!err) {
		// create dev node
		printk(KERN_INFO "medebug: probe: Found Damagement Engine\n");
		if (alloc_chrdev_region(&me_dev, 0, 1, "me_dev") < 0)
			goto out;
		if ((me_cl = class_create(THIS_MODULE, "me")) == NULL)
			goto err_cl;
		if (device_create(me_cl, NULL, me_dev, NULL, "medebug") == NULL)
			goto err_dev;
		cdev_init(&me_dev_file, &medebug_fops);
		if (cdev_add(&me_dev_file, me_dev, 1) == -1)
			goto err_cdev;

		// Initialise HECI
		printk(KERN_INFO "medebug: probe: Initialising HECI\n");

		intel_mei_setup(dev);
        	udelay(1000);

		// Reset
        	mei_reset();
        	udelay(10000);

		// Get version to kernel log
		printk(KERN_INFO "medebug: probe: Get version\n");
        	mkhi_get_fw_version();
        	udelay(10000);

		// Reset
        	mei_reset();
        	udelay(10000);

		// Get firmware capability to kernel log
		printk(KERN_INFO "medebug: probe: Get fw capabilities\n");
       		mkhi_get_fwcaps();
        	udelay(10000);
	} else {
		printk(KERN_INFO "medebug: probe: failed (%d)\n", err);
	}
	return err;
err_cdev:
	device_destroy(me_cl, me_dev);
err_dev:
	class_destroy(me_cl);
err_cl:
	unregister_chrdev_region(me_dev, 1);
out:
	return -1;
}

void medebug_remove (struct pci_dev *dev)
{
	cdev_del(&me_dev_file);
	unregister_chrdev_region(me_dev, 1);
}

static struct pci_driver medebug_driver = {
	.name = "medebug",
	.id_table = medebug_ids,
	.probe = medebug_probe,
	.remove = medebug_remove,
};

static int __init medebug_init(void)
{
	printk(KERN_INFO "medebug: === Damagement Engine debug driver ===\n");
	return pci_register_driver(&medebug_driver);
}

static void __exit medebug_exit(void)
{
	pci_unregister_driver(&medebug_driver);
	printk(KERN_INFO "medebug: === Bye! ===\n");
}
/*
static struct class_attribute medebug_attrs[] = {
	__ATTR(memory, 0000, NULL, memory_clone),
	__ATTR_NULL,
};

static struct class medebug_class = {
	.name = "me",
	.owner = THIS_MODULE,
	.class_attrs = medebug_attrs,
};
*/
module_init(medebug_init);
module_exit(medebug_exit);
