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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Damien Zammit");

static int oldarc = 0;

module_param(oldarc, int, 0000);
MODULE_PARM_DESC(oldarc, "If set to 1, assume 1.5MB Damagement Engine firmware");

void medebug_remove (struct pci_dev *dev)
{

}
//pci_read_config_dword(struct pci_dev *dev, int where, u32 *val);
//result = pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &myirq);
//if (result) {
///* deal with error */
//}

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

int medebug_probe (struct pci_dev *dev, const struct pci_device_id *id)
{
	return(pci_enable_device(dev));
}

static struct pci_driver medebug_driver = {
	.name = "medebug",
	.id_table = medebug_ids,
	.probe = medebug_probe,
	.remove = medebug_remove,
};

static int __init medebug_init(void)
{
	printk(KERN_INFO "medebug: Damagement Engine debug driver\n");
	return pci_register_driver(&medebug_driver);
}

static void __exit medebug_exit(void)
{
	pci_unregister_driver(&medebug_driver);
	printk(KERN_INFO "medebug: byebye\n");
}

module_init(medebug_init);
module_exit(medebug_exit);
