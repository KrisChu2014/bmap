#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/genhd.h>
#include <linux/buffer_head.h>

#include "mapper.h"
#include "init.h"

MODULE_LICENSE("GPL");

#define BDEV_TO_HOOK MKDEV(8, 49)	/* Testing, hardcode /dev/sdd1 */
#define BDEV_TO_STORE MKDEV(8, 33)	/* Testing, hardcode /dev/sdc1 */

struct block_mapper bm;

static int __init init_bm(void)
{
	int retval = -1;

#ifdef __BM_PRE_READ__
	printk("pre read support\n");
#endif

#ifdef __BM_SYNC_BH_WRITEBACK__
	printk("sync bh wb support\n");
#endif

	retval = bm_bdev_resume(&bm, BDEV_TO_HOOK, BDEV_TO_STORE);
	if(retval)
 	{
		printk("failed to resume bm [%d], restart\n", retval);

		retval = bm_bdev_create(&bm, BDEV_TO_HOOK, BDEV_TO_STORE);

		if(retval)
		{
			printk("failed to create bm [%d], cleanup\n", retval);

			goto __return;
		}
	}

	retval = bm_bdev_start(&bm);

	if(retval)
	{
		printk("failed to start bm [%d]\n", retval);
		bm_bdev_cleanup(&bm);
	}


__return:
	return retval;
}

static void exit_bm(void)
{
	printk("exit_bm\n");

	bm_bdev_stop(&bm);
	bm_bdev_cleanup(&bm);

	return;
}

module_init(init_bm);
module_exit(exit_bm);




