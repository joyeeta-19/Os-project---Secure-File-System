#include <linux/module.h>	 /* Needed by all modules */
#include <linux/kernel.h>	 /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/fs.h>	 /* Needed for the macros */

///< The license type -- this affects runtime behavior
MODULE_LICENSE("GPL");

///< The author -- visible when you use modinfo
MODULE_AUTHOR("Joyeeta Dey");

///< The description -- see modinfo
MODULE_DESCRIPTION("File System Module");

///< The version of the module
MODULE_VERSION("0.1");

struct dentry *myfs_mount(struct file_system_type *fs_type,int flags,const char *dev_name,void *data)
{
    struct dentry *dentry =
        mount_bdev(fs_type, flags, dev_name, data, myfs_fill_super);
    if (IS_ERR(dentry))
        pr_err("'%s' mount failure\n", dev_name);
    else
        pr_info("'%s' mount success\n", dev_name);

    return dentry;
}

void myfs_kill_sb(struct super_block *sb)
{
    kill_block_super(sb);

    pr_info("unmounted disk\n");
}

static struct file_system_type myfs_file_system_type = {
    .owner = THIS_MODULE,
    .name = "myfs",
    .mount = myfs_mount,
    .kill_sb = myfs_kill_sb,
    .fs_flags = FS_REQUIRES_DEV,
    .next = NULL,
};


static int __init myfs_start(void)
{
    //create inode cache
    int ret = myfs_init_inode_cache();
    if (ret) {
        pr_err("inode cache creation failed\n");
        goto end;
    }
	
    //register filesystem
    ret = register_filesystem(&myfs_file_system_type);	//file system type
    if (ret) {
        pr_err("register_filesystem() failed\n");
        goto end;
    }

    printk(KERN_INFO "MYFS module loaded\n");
    
end:
    return ret;
}


//exit function
static void __exit myfs_end(void)
{
    int ret = unregister_filesystem(&myfs_file_system_type);
    if (ret)
        pr_err("unregister_filesystem() failed\n");

    myfs_destroy_inode_cache();

    printk(KERN_INFO "MYFS module unloaded\n");
    printk(KERN_INFO "Goodbye Miss Joyeeta\n");
}


module_init(myfs_start);
module_exit(myfs_end);

