#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gadre Nayan A");
MODULE_DESCRIPTION("Task 01 Module Challenge");

struct dentry *dirret, *fileret;
int filevalue;

#define SIZE 13
#define EUDYPTULA_ID "03b1da8ba102"

ssize_t debugfs_read(struct file *fp, char __user *user, size_t size,
loff_t *offs)
{
        return simple_read_from_buffer(user, size, offs, EUDYPTULA_ID,
                strlen(EUDYPTULA_ID));
}

ssize_t debugfs_write(struct file *fp, const char __user *user, size_t size,
loff_t *offs)
{
        char buff[SIZE];
        int result = 0;

        if (size == SIZE) {
                result = simple_write_to_buffer(buff, SIZE-1,
                                offs, user, size) + 1;
                if (result < 0)
                        return result;
                buff[SIZE - 1] = '\0';
        } else {
                return -EINVAL;
        }

        if (!strncmp(buff, EUDYPTULA_ID, strlen(EUDYPTULA_ID)))
                return size;
        else
                return -EINVAL;
}

static const struct file_operations fops_debug = {
	.read = debugfs_read,
	.write = debugfs_write
};
static int __init hello_init(void)
{
	pr_debug("Hello world!\n");
	dirret = debugfs_create_dir("eudyptula", NULL);
	fileret = debugfs_create_file("id", 0666, dirret, &filevalue, &fops_debug);
	return 0;
}

static void __exit hello_cleanup(void)
{
	pr_debug("Cleaning up module.\n");
	debugfs_remove_recursive(dirret);
}

module_init(hello_init);
module_exit(hello_cleanup);
