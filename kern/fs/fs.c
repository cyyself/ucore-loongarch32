#include <defs.h>
#include <kmalloc.h>
#include <sem.h>
#include <vfs.h>
#include <dev.h>
#include <file.h>
#include <sfs.h>
#include <inode.h>
#include <assert.h>
//called when init_main proc start
void
fs_init(void) {
    vfs_init();
    dev_init();
    sfs_init();
}

void
fs_cleanup(void) {
    vfs_cleanup();
}

void
lock_fs(struct fs_struct *fs_struct) {
    down(&(fs_struct->fs_sem));
}

void
unlock_fs(struct fs_struct *fs_struct) {
    up(&(fs_struct->fs_sem));
}
//Called when a new proc init
struct fs_struct *
fs_create(void) {
	//kprintf("[fs_create]\n");
    static_assert((int)FS_STRUCT_NENTRY > 128);
    struct fs_struct *fs_struct;
    if ((fs_struct = kmalloc(sizeof(struct fs_struct) + FS_STRUCT_BUFSIZE)) != NULL) {
        fs_struct->pwd = NULL;
        fs_struct->filemap = (void *)(fs_struct + 1);
        atomic_set(&(fs_struct->fs_count), 0);
        sem_init(&(fs_struct->fs_sem), 1);
        filemap_init(fs_struct->filemap);
    }
    return fs_struct;
}
//Called when a proc exit
void
fs_destroy(struct fs_struct *fs_struct) {
//	kprintf("[fs_destroy]\n");
    assert(fs_struct != NULL && fs_count(fs_struct) == 0);
    if (fs_struct->pwd != NULL) {
        vop_ref_dec(fs_struct->pwd);
    }
    int i;
    struct file *file = fs_struct->filemap;
    for (i = 0; i < FS_STRUCT_NENTRY; i ++, file ++) {
        if (file->status == FD_OPENED) {
            filemap_close(file);
        }
        assert(file->status == FD_NONE);
    }
    kfree(fs_struct);
}

void
fs_closeall(struct fs_struct *fs_struct) {
//	kprintf("[fs_closeall]\n");
    assert(fs_struct != NULL && fs_count(fs_struct) > 0);
    int i;
    struct file *file = fs_struct->filemap;
	//skip the stdin & stdout
    for (i = 2, file += 2; i < FS_STRUCT_NENTRY; i ++, file ++) {
        if (file->status == FD_OPENED) {
            filemap_close(file);
        }
    }
}

int
dup_fs(struct fs_struct *to, struct fs_struct *from) {
//	kprintf("[dup_fs]\n");
    assert(to != NULL && from != NULL);
    assert(fs_count(to) == 0 && fs_count(from) > 0);
    if ((to->pwd = from->pwd) != NULL) {
        vop_ref_inc(to->pwd);
    }
    int i;
    struct file *to_file = to->filemap, *from_file = from->filemap;
    for (i = 0; i < FS_STRUCT_NENTRY; i ++, to_file ++, from_file ++) {
        if (from_file->status == FD_OPENED) {
            /* alloc_fd first */
            to_file->status = FD_INIT;
            filemap_dup(to_file, from_file);
        }
    }
    return 0;
}

