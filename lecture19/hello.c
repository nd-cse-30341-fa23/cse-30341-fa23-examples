/* hello_simple.c
 * Based on:
 *  http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/
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
MODULE_DESCRIPTION("A simple hello, world LKM.");
MODULE_VERSION("0.1");


static int __init hello_init(void) {
    printk(KERN_INFO "hello: Initializing the hello LKM\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "hello: Goodbye from the LKM!\n");
}

module_init(hello_init);
module_exit(hello_exit);
