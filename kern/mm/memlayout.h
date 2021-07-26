#ifndef __KERN_MM_MEMLAYOUT_H__
#define __KERN_MM_MEMLAYOUT_H__

#define KSTACKPAGE          2                           // # of pages in kernel stack
#define KSTACKSIZE          (KSTACKPAGE * 4096)       // sizeof kernel stack

#endif /* !__KERN_MM_MEMLAYOUT_H__ */