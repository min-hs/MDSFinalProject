#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>		// struct gpio, gpio_request_one(), gpio_set_value(), gpio_get_value(), gpio_free() 
#include <linux/miscdevice.h>
#include <linux/delay.h>	// udelay()
#include <linux/time.h>		// ktime_t, ktime_get(), ktime_to_us(), ktime_sub() 
#include <asm/uaccess.h>	//	put_user()

static const struct gpio trigger1 =
{
	.gpio  = 4,
	.flags = GPIOF_DIR_OUT,
	.label = "Key4"
};
static const struct gpio echo1 =
{
	.gpio  = 17,
	.flags = GPIOF_DIR_IN,
	.label = "Key17"
};

static const struct gpio trigger2 =
{
	.gpio  = 20,
	.flags = GPIOF_DIR_OUT,
	.label = "Key4"
};
static const struct gpio echo2 =
{
	.gpio  = 21,
	.flags = GPIOF_DIR_IN,
	.label = "Key17"
};

static char kbuff[10];

static ssize_t ultrasonic_read(struct file* pfile, char* buf, size_t count, loff_t* pos)
{
	static ktime_t start;
	static ktime_t end;
	int dist, ret = 0;
	long cnt = 0;

	pr_info("read!!\n");
	
	// 2-1. Send High for 10 us to Trigger PIN
	gpio_set_value(trigger1.gpio, 1);
	udelay(10);	//	10 us
	gpio_set_value(trigger1.gpio, 0);
	// 2-2. Wait High from Echo PIN
	while( (int)gpio_get_value(echo1.gpio) == 0 )
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
	while( (int)gpio_get_value(echo1.gpio) == 1 )
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
	if (dist > 999 || dist < 0) dist = 999;
	ret = dist * 1000;
	pr_info("1Distance: %d cm\n", dist);
	
	//
	
	// 2-1. Send High for 10 us to Trigger PIN
	gpio_set_value(trigger2.gpio, 1);
	udelay(10);	//	10 us
	gpio_set_value(trigger2.gpio, 0);
	// 2-2. Wait High from Echo PIN
	while( (int)gpio_get_value(echo2.gpio) == 0 )
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
	while( (int)gpio_get_value(echo2.gpio) == 1 )
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
	if (dist > 999 || dist < 0) dist = 999;
	ret += dist;
	pr_info("2Distance: %d cm\n", dist);
	
	sprintf(kbuff,"%d", ret);

	copy_to_user(buf, kbuff, 6);
	//pr_info("kernel sends to user int(4B) values: %d\n", cnt);
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
	.name = "hc_sr04_1",
	.fops = &fops,
	.nodename = "hc_sr04_1",
};

static int __init init_led_module(void)
{

	pr_info("Init !!\n");

	// 1.Initialize Pin Settings.
	// 1-1. Settings for Trigger PIN
	gpio_request_one(trigger1.gpio, trigger1.flags, trigger1.label);
	gpio_set_value(trigger1.gpio, 0);
	gpio_request_one(trigger2.gpio, trigger2.flags, trigger2.label);
	gpio_set_value(trigger2.gpio, 0);
	// 1-2. Settings for Echo PIN
	gpio_request_one(echo1.gpio, echo1.flags, echo1.label);
	gpio_request_one(echo2.gpio, echo2.flags, echo2.label);

	misc_register(&misc_dev);
	 
	return 0;
}

static void __exit exit_led_module(void)
{
	pr_info("Exit !!\n");
	gpio_free(trigger1.gpio);
	gpio_free(echo1.gpio);
	gpio_free(trigger2.gpio);
	gpio_free(echo2.gpio);
	misc_deregister(&misc_dev);
}


module_init(init_led_module);
module_exit(exit_led_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kang-hyuck <kang-hyuck@naver.com>");
MODULE_DESCRIPTION("hc_sr04 module1");





