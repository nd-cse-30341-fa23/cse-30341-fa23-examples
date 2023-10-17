/* quote.c
 * Based on:
 *  http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/
 *  http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 */

#include <linux/init.h>
#include <linux/random.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Bui");
MODULE_DESCRIPTION("A simple quotes LKM.");
MODULE_VERSION("0.1");

#define DEVICE_NAME "quote"
#define CLASS_NAME  "quote"

static char *QUOTES[] = {
    "I'm wonder if you'd go wandering with me",
    "Bluegrass for breakfast",
};

static int            majorNumber;
static struct class  *quoteClass  = NULL;
static struct device *quoteDevice = NULL;
static DEFINE_MUTEX(quoteMutex);

static int     dev_open(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static int     dev_release(struct inode *, struct file *);

static struct file_operations fops = {
    .open    = dev_open,
    .read    = dev_read,
    .write   = NULL,
    .release = dev_release,
};

static int __init quote_init(void) {
    printk(KERN_INFO "Quote: Initializing the Quote LKM\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber<0) {
	printk(KERN_ALERT "Quote: failed to register a major number\n");
	return majorNumber;
    }
    printk(KERN_INFO "Quote: registered correctly with major number %d\n", majorNumber);

    quoteClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(quoteClass)) {
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_ALERT "Quote: failed to register device class\n");
	return PTR_ERR(quoteClass);
    }
    printk(KERN_INFO "Quote: device class registered correctly\n");

    quoteDevice = device_create(quoteClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(quoteDevice)){
	class_destroy(quoteClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_ALERT "Quote: Failed to create the device\n");
	return PTR_ERR(quoteDevice);
    }

    mutex_init(&quoteMutex);
    printk(KERN_INFO "Quote: device class created correctly\n");
    return 0;
}

static void __exit quote_exit(void) {
    mutex_destroy(&quoteMutex);
    device_destroy(quoteClass, MKDEV(majorNumber, 0));
    class_unregister(quoteClass);
    class_destroy(quoteClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "Quote: Goodbye from the LKM!\n");
}

static int     dev_open(struct inode *inodep, struct file *filep) {
    if (!mutex_trylock(&quoteMutex)) {
	printk(KERN_ALERT "Quote: Device in use by another process");
	return -EBUSY;
    }
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    char *quote = NULL;
    size_t index;

    get_random_bytes(&index, 1);
    quote = QUOTES[index % 2];
    error_count = raw_copy_to_user(buffer, quote, strlen(quote));

    if (error_count == 0) {
	printk(KERN_INFO "Quote: Sent %s to the user\n", quote);
	return strlen(quote);
    } else {
	printk(KERN_INFO "Quote: Failed to send %d characters to the user\n", error_count);
	return -EFAULT;
    }
}

static int dev_release(struct inode *inodep, struct file *filep){
    mutex_unlock(&quoteMutex);
    printk(KERN_INFO "Quote: Device successfully closed\n");
    return 0;
}

module_init(quote_init);
module_exit(quote_exit);
