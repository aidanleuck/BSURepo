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

// Per device struct
typedef struct
{
    dev_t devno;
    struct cdev cdev;
    char *sep;      // Default list of separators
} Device;

// Per init struct
typedef struct
{
    char *s;            // Input string
    char *sep;          // List of separators
    size_t inputSize;   // Size of the input
    int ioctl;          // Keeps track if next write will be to separator
    size_t sepLength;   // Length of the separators
    size_t inputScanned; // How much of the input has been scanned so far
} Scanner;

static Device device;

// <summary>Creates an instance of the scanner</summary>
// <param name = "inode">Inode associated with the driver</param>
// <param name = "filp">Holds private data about an instance</param>
// <returns>File descriptor to the scanner</returns>
static int open(struct inode *inode, struct file *filp)
{
    // Allocates memory for the scanner
    Scanner* scanner = (Scanner*)kmalloc(sizeof(*scanner), GFP_KERNEL);
    if(!scanner){
        printk(KERN_ERR, "%s: kmalloc() failed\n", DEVNAME);
        return -ENOMEM;
    } 

    // Allocates memory for the default list of separators
    scanner->sep = kmalloc(strlen(device.sep) + 1, GFP_KERNEL);
    if(!scanner->sep){
        printk(KERN_ERR, "%s: kmalloc() failed\n", DEVNAME);
        return -ENOMEM;
    }

    // Copies the default list to the scanner instance
    strcpy(scanner->sep, device.sep);
    scanner->sepLength = strlen(device.sep);

    // Sets ioctl to 1 by default (next write will not be to separators)
    scanner->ioctl = 1;
    filp->private_data = scanner;
    return 0;
}

// <summary>Frees instance of a scanner</summary>
// <param name = "inode">Inode associated with the driver</param>
// <param name = "filp">Holds private data about an instance</param>
// <returns>Whether release was succesful</returns>
static int release(struct inode *inode, struct file *filp){
    Scanner* scanner = filp->private_data;
    kfree(scanner->sep);
    kfree(scanner->s);
    kfree(scanner);
    return 0;
}

// <summary>Determines if a character is in the list of separators</summary>
// <param name = "scan">Instance of scanner to check</param>
// <param name = "cmp">The character to check if in list</param>
// <returns>1 if in list 0 otherwise</returns>
static int inSep(Scanner* scan, char cmp)
{
    int i;
    for (i = 0; i < scan->sepLength; i++)
    {
        if (scan->sep[i] == cmp)
        {
            return 1;
        }
    }
    return 0;
}

// <summary>Reads input from device and separates into tokens</summary>
// <param name = "buf">The buffer to fill and return to user</param>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "charRequested">Number of characters requested by user</param>
// <param name = "f_pos">File offset to start at</param>
// <returns>Number of characters read, EOF, or 0 if reading a token</returns>
extern ssize_t read(struct file *filp, char *buf, size_t charRequested, loff_t *f_pos)
{
    Scanner* scan = filp->private_data;

    // Allocate enough memory to hold the number of characters requested
    char *currentString = kmalloc(sizeof(char) * (charRequested + 1), GFP_KERNEL);
    if(!currentString){
        printk(KERN_ERR, "%s: kmalloc failed", DEVNAME);
        return -ENOMEM;
    }

    size_t numCharRead = 0; // Keep track of number of characters read
    int tokenFound = 0;     // Keep track if a token was found

    while (numCharRead < charRequested && !tokenFound && scan->inputScanned < scan->inputSize)
    {
        // Gets the current character in input
        char currChar = (scan->s[scan->inputScanned]);

        // Checks if the current character was a token
        tokenFound = inSep(scan, currChar);

        // If not a separator add it to our string
        if (!tokenFound)
        {
            strncat(currentString, &currChar, 1);

            // Increments the number of characters in buffer
            numCharRead++;

            // Increments how much of the input has been scanned
            scan->inputScanned++;
        }
    }

    // Copies buffer back to user
    if (copy_to_user(buf,currentString,numCharRead)) {    
        printk(KERN_ERR "%s: copy_to_user() failed\n",DEVNAME);    
        return 0;  
    } 
    kfree(currentString);

    // Sets numcharRead accordingly
    if(scan->inputScanned == scan->inputSize && numCharRead == 0){
        numCharRead = -1;
    }
    if(tokenFound && numCharRead == 0){
        scan->inputScanned++;
    }

    return numCharRead;
}

// <summary>Sets flag in driver to tell us that next write will be list of separators</summary>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "cmd">command to execute</param>
// <param name = "arg">arg to execute</param>
// <returns>0</returns>
static long ioctl(struct file *filp,
                  unsigned int cmd,
                  unsigned long arg)
{
    Scanner* scan = filp->private_data;
    if (cmd == 0 && arg == 0)
    {
        scan->ioctl = 0;
    }
    return 0;
}

// <summary>Writes device input into scanner memory</summary>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "line">Line written to device</param>
// <param name = "len">Length of the input</param>
// <param name = "f_pos">Offset to start reading from</param>
// <returns>Number of characters written to device</returns>
extern ssize_t write(struct file *filp, const char *line, 
size_t len,
loff_t *f_pos)
{
    Scanner* scan = filp->private_data;

    // Check if ioctl set or not
    if (!scan->ioctl)
    {
        scan->sep = kmalloc(sizeof(char) * (len + 1), GFP_KERNEL);
        if(copy_from_user(scan->sep, line, len) < 0){
            printk(KERN_ERR, "%s: write separators failed", DEVNAME);
            len = -1;
        }
        scan->sepLength = len;
        scan->ioctl = 1;
    }
    else
    {
        scan->s = kmalloc(sizeof(char) * (len + 1), GFP_KERNEL);
        if(copy_from_user(scan->s, line, len) < 0){
            printk(KERN_ERR, "%s: write failed", DEVNAME);
            len = -1;
        }
        scan->inputSize = len;
        scan->inputScanned = 0;
    }

    return len;
}

// Valid ops on device driver
static struct file_operations ops = {
    .open = open,
    .release = release,
    .read = read,
    .write = write,
    .unlocked_ioctl = ioctl,
    .owner = THIS_MODULE};

// Initializes the scanner with default values
static int __init my_init(void)
{
    // List of default separators
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
    return 0;
}

// Handles exit of the device driver
static void __exit my_exit(void) {
  cdev_del(&device.cdev);
  unregister_chrdev_region(device.devno,1);
  kfree(device.sep);
  printk(KERN_INFO "%s: exit\n", DEVNAME);
}

module_init(my_init);
module_exit(my_exit);

