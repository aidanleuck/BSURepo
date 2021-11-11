#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BSU CS 452 HW4");
MODULE_AUTHOR("<aidanleuck@u.boisestate.edu>");

typedef struct
{
    dev_t devno;
    struct cdev cdev;
    char *sep;
} Device;
typedef struct
{
    char *s;
    char *sep;
    ssize_t inputSize;
    int ioctl;
    ssize_t sepLength;
    ssize_t inputScanned;
} Scanner;

static Device device;

static struct file_operations ops = {
    .open = open,
    .release = release,
    .read = read,
    .unlocked_ioctl = ioctl,
    .owner = THIS_MODULE};

// Initializes the scanner with default values
static int __init my_init(void)
{
    const char *defaultSep = ";,+-=!@./#$%&*: ";
    int err;
    device.sep = (char *)kmalloc(strlen(defaultSep) + 1, GFP_KERNEL);
    if (!device.sep)
    {
        printk(KERN_ERR, "%s: kmalloc failed\n", DEVNAME);
        return -ENOMEM;
    }
    strcpy(device.sep, defaultSep);

    err = alloc_chrdev_region(&device.devno, 0, 1, DEVNAME);
    if (err < 0)
    {
        printk(KERN_ERR, "%s: alloc_chrdev_region() failed\n", DEVNAME);
        return err;
    }
    cdev_init(&device.cdev, &ops);
    device.cdev.owner = THIS_MODULE;
    err = cdev_add(&device.cdev, device.devno, 1);
    if (err)
    {
        printk(KERN_ERR "%s: cdev_add() failed\n", DEVNAME);
        return err;
    }
    printk(KERN_INFO "%s: init\n", DEVNAME);
}

static int open(struct inode *inode, struct file *filp)
{
    Scanner* scanner = (Scanner*)kmalloc(sizeof(*scanner), GFP_KERNEL);
    if(!scanner){
        printk(KERN_ERR, "%s: kmalloc() failed\n", DEVNAME);
        return -ENOMEM;
    } 
    scanner->sep = kmalloc(strlen(device.sep) + 1, GFP_KERNEL);
    if(!scanner->sep){
        printk(KERN_ERR, "%s: kmalloc() failed\n", DEVNAME);
        return -ENOMEM;
    }
    strcpy(scanner->sep, device.sep);
    filp->private_data = scanner;
    return 0;
}
static int release(struct inode *inode, struct file *filp){
    Scanner* scanner = filp->private_data;
    kfree(scanner->sep);
    kfree(scanner->s);
    kfree(scanner);
    return 0;
}

// Determine if the current character is in the list of separators
static int inSep(struct file *filp, char cmp)
{
    Scanner* scan=filp->private_data;
    for (int i = 0; i < scan->sepLength; i++)
    {
        if (scan->sep[i] == cmp)
        {
            return 1;
        }
    }
    return 0;
}

// Reads the current string and separate into tokens
extern ssize_t read(struct file *filp, char *buf, ssize_t charRequested)
{
    Scanner* scan = filp->private_data;
    char *currentString = kmalloc(sizeof(char) * (charRequested + 1), GFP_KERNEL);
    if(!currentString){
        printk(KERN_ERR, "%s: kmalloc failed", DEVNAME);
        return -ENOMEM;
    }

    ssize_t numCharRead = 0;
    int tokenFound = 0;

    while (numCharRead < charRequested && !tokenFound && scan->inputScanned < scan->inputSize)
    {
        // Gets the current character in input
        char currChar = *scan->s;

        // Build the string by concatenating the current character
        strncat(currentString, &currChar, 1);

        // Checks if the current character was a token
        tokenFound = inSep(scan, currChar);

        // Increments to next character in input
        scan->s = scan->s + 1;

        // Increments the number of characters read
        numCharRead++;

        // Increments how much of the input has been scanned
        scan->inputScanned++;
    }
    strncpy(buf, currentString, numCharRead);
    kfree(currentString);

    if (tokenFound)
    {
        numCharRead = 0;
    }
    else if (scan->inputScanned >= scan->inputSize)
    {
        numCharRead = -1;
    }
    return numCharRead;
}

static long ioctl(struct file *filp,
                  unsigned int cmd,
                  unsigned long arg)
{
    Scanner* scan = filp->private_data;
    if (cmd == 0 && arg == 0)
    {
        scan->ioctl = 0;
    }
}

extern ssize_t write(struct file *filp, char *line, ssize_t len)
{
    Scanner* scan = filp->private_data;
    if (!scan->ioctl)
    {
        scan->sep = line;
        scan->sepLength = len;
        scan->ioctl = 1;
    }
    else
    {
        scan->s = line;
        scan->inputSize = len;
    }

    return len;
}
static void __exit my_exit(void) {
  cdev_del(&device.cdev);
  unregister_chrdev_region(device.devno,1);
  kfree(device.sep);
  printk(KERN_INFO "%s: exit\n",DEVNAME);
}

module_init(my_init);
module_exit(my_exit);
