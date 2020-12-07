#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>		// struct gpio, gpio_request_one(), gpio_set_value(), gpio_get_value(), gpio_free() 
#include <linux/miscdevice.h>
#include <linux/delay.h>	// udelay()
#include <linux/time.h>		// ktime_t, ktime_get(), ktime_to_us(), ktime_sub() 
#include <asm/uaccess.h>	//	put_user()

static const struct gpio trigger =
{
	.gpio  = 20,
	.flags = GPIOF_DIR_OUT,
	.label = "Key20"
};
static const struct gpio echo =
{
	.gpio  = 21,
	.flags = GPIOF_DIR_IN,
	.label = "Key21"
};

static char kbuff[100];

static ssize_t ultrasonic_read(struct file* pfile, char* buf, size_t count, loff_t* pos)
{
	static ktime_t start;
	static ktime_t end;
	int dist;
	long cnt = 0;

	pr_info("read!!\n");

	// 2-1. Send High for 10 us to Trigger PIN
	gpio_set_value(trigger.gpio, 1);
	udelay(10);	//	10 us
	gpio_set_value(trigger.gpio, 0);
	// 2-2. Wait High from Echo PIN
	while( (int)gpio_get_value(echo.gpio) == 0 )
	{
		start = ktime_get();		
		if( cnt == 100000 )
		{	
			pr_info("   timeover in echo 0 \n");
			break;
		}
		cnt++;
	}
	cnt = 0;
	// 2-2. Wait LOW from Echo PIN
	while( (int)gpio_get_value(echo.gpio) == 1 )
	{
		end   = ktime_get(); 
		cnt++;
		if( cnt==1000000 )
		{	
			pr_info("   timeover in echo 1 \n");
			break;
		}
	}
	// 3. Calculate and Print Distance 
	dist = (int)ktime_to_us(ktime_sub(end, start))/(int)58;
	pr_info("2Distance: %d cm\n", dist );

	sprintf(kbuff,"%d",dist);
	copy_to_user(buf, kbuff, 4);
	return 0;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read  = ultrasonic_read,
};

static struct miscdevice misc_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hc_sr04_2",
	.fops = &fops,
	.nodename = "hc_sr04_2",
};

static int __init init_led_module(void)
{

	pr_info("Init !!\n");


	// 1.Initialize Pin Settings.
	// 1-1. Settings for Trigger PIN
	gpio_request_one(trigger.gpio, trigger.flags, trigger.label);
	gpio_set_value(trigger.gpio, 0);
	// 1-2. Settings for Echo PIN
	gpio_request_one(echo.gpio, echo.flags, echo.label);

	misc_register(&misc_dev);


	 
	return 0;
}

static void __exit exit_led_module(void)
{
	pr_info("Exit !!\n");
	gpio_free(trigger.gpio);
	gpio_free(echo.gpio);
	misc_deregister(&misc_dev);
}


module_init(init_led_module);
module_exit(exit_led_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kang-hyuck <kang-hyuck@naver.com>");
MODULE_DESCRIPTION("ultrasonic module2");





