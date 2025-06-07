#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "karakter"
#define BUFFER_SIZE 1024

static int major_number;
static char *kernel_buffer;
static struct class *karakter_class = NULL;
static struct device *karakter_device = NULL;

static int karakter_open(struct inode *inode, struct file *file)
{
    pr_info("karakter: device opened\n");
    return 0;
}

static int karakter_release(struct inode *inode, struct file *file)
{
    pr_info("karakter: device closed\n");
    return 0;
}

static ssize_t karakter_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    ssize_t bytes_read = 0;
    if (*offset >= BUFFER_SIZE)
        return 0;

    if (*offset + len > BUFFER_SIZE)
        len = BUFFER_SIZE - *offset;

    if (copy_to_user(buf, kernel_buffer + *offset, len) == 0) {
        *offset += len;
        bytes_read = len;
        pr_info("karakter: read %zu bytes\n", len);
    } else {
        pr_err("karakter: failed to read to user\n");
        bytes_read = -EFAULT;
    }

    return bytes_read;
}

static ssize_t karakter_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    ssize_t bytes_written = 0;
    if (*offset >= BUFFER_SIZE)
        return -ENOMEM;

    if (*offset + len > BUFFER_SIZE)
        len = BUFFER_SIZE - *offset;

    if (copy_from_user(kernel_buffer + *offset, buf, len) == 0) {
        *offset += len;
        bytes_written = len;
        pr_info("karakter: wrote %zu bytes\n", len);
    } else {
        pr_err("karakter: failed to write from user\n");
        bytes_written = -EFAULT;
    }

    return bytes_written;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = karakter_open,
    .read = karakter_read,
    .write = karakter_write,
    .release = karakter_release,
};

static int __init karakter_init(void)
{
    kernel_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_err("karakter: failed to allocate buffer\n");
        return -ENOMEM;
    }

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("karakter: failed to register device\n");
        kfree(kernel_buffer);
        return major_number;
    }

    karakter_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(karakter_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        kfree(kernel_buffer);
        pr_err("karakter: failed to create class\n");
        return PTR_ERR(karakter_class);
    }

    karakter_device = device_create(karakter_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(karakter_device)) {
        class_destroy(karakter_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        kfree(kernel_buffer);
        pr_err("karakter: failed to create device\n");
        return PTR_ERR(karakter_device);
    }

    pr_info("karakter: module loaded, major number %d\n", major_number);
    return 0;
}

static void __exit karakter_exit(void)
{
    device_destroy(karakter_class, MKDEV(major_number, 0));
    class_destroy(karakter_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    kfree(kernel_buffer);
    pr_info("karakter: module unloaded\n");
}

module_init(karakter_init);
module_exit(karakter_exit);

MODULE_LICENSE("GPL"); // type dari license
MODULE_AUTHOR("Hikmah gumelar"); //jangan lupa di add agar semua tau kalau modules yang di buat adalah buatan lo
MODULE_DESCRIPTION("Tutorial membuat kernel modules");

