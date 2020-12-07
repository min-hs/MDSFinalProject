#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <asm/page.h>

#include "../inc/my_lib.h"

#define MOTOR_A0		5
#define MOTOR_A1		6
#define MOTOR_B0		7
#define MOTOR_B1		8

#define PWM_MA			13
#define PWM_MB			19

int			fd_mem;
pGPIO_REGS	pGpio;
pPWM_REGS	pPwm;
pCLK_REGS	pClk;

static ssize_t 	l298n_write(struct file *, const char *, size_t, loff_t *);
static void 	l298n_ctrl(char cmd1, char cmd2);

static ssize_t l298n_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char *k_buf;

	k_buf = kmalloc(count, GFP_KERNEL);
	if(copy_from_user(k_buf, buf, count))
	{
		printk("l298n_write() fail!\n");
		return -EFAULT;
	}
	l298n_ctrl(k_buf[0], k_buf[1]);
	kfree(k_buf);

	return 0;
}

static void l298n_ctrl(char cmd1, char cmd2) {	
	switch(cmd2) {
	case '1':
	  pwm2_setData(270);
	  break;
    case '2':
	  pwm2_setData(310);
      break;
    case '3':
	  pwm2_setData(350);
      break;
    }
	
	switch(cmd1) {
	case 'X':
      gpio_digitalWrite(MOTOR_A0, LOW);
      gpio_digitalWrite(MOTOR_A1, LOW);
      gpio_digitalWrite(MOTOR_B0, LOW);
      gpio_digitalWrite(MOTOR_B1, LOW);
	  break;
    case 'S':
      gpio_digitalWrite(MOTOR_A0, HIGH);
      gpio_digitalWrite(MOTOR_A1, LOW);
      gpio_digitalWrite(MOTOR_B0, HIGH);
      gpio_digitalWrite(MOTOR_B1, LOW);
      break;

    case 'W':
      gpio_digitalWrite(MOTOR_A0, LOW);
      gpio_digitalWrite(MOTOR_A1, HIGH);
      gpio_digitalWrite(MOTOR_B0, LOW);
      gpio_digitalWrite(MOTOR_B1, HIGH);
      break;

    case 'D':
      gpio_digitalWrite(MOTOR_A0, LOW);
      gpio_digitalWrite(MOTOR_A1, HIGH);
      gpio_digitalWrite(MOTOR_B0, HIGH);
      gpio_digitalWrite(MOTOR_B1, LOW);
      break;

    case 'A':
      gpio_digitalWrite(MOTOR_A0, HIGH);
      gpio_digitalWrite(MOTOR_A1, LOW);
      gpio_digitalWrite(MOTOR_B0, LOW);
      gpio_digitalWrite(MOTOR_B1, HIGH);
      break;
    }
}

static struct file_operations l298n_fops = 
{
	.owner = THIS_MODULE,
	.write = l298n_write,
};

static struct miscdevice misc_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "l298n",
	.fops = &l298n_fops,
	.nodename = "l298n",
};

static int l298n_init(void)
{
	int result;
	
	result = misc_register(&misc_dev);
	if(result < 0)
	{
		printk("L298N device driver can't be registered.\n");
		return result;
	}
	
	pGpio = ioremap(RPI_GPIO, 4096);
	pPwm = ioremap(RPI_PWM, 4096);
	pClk = ioremap(RPI_CLK, 4096);
	
	gpio_pinMode(MOTOR_A0, OUTPUT);
	gpio_pinMode(MOTOR_A1, OUTPUT);
	gpio_pinMode(MOTOR_B0, OUTPUT);
	gpio_pinMode(MOTOR_B1, OUTPUT);
	
	gpio_pinMode(PWM_MA, ALTF0);
	gpio_pinMode(PWM_MB, ALTF5);
	pwm2_setRange(350);
	pwm2_setData(100);
	clk0_setDefault();
	
	gpio_digitalWrite(MOTOR_A0, LOW);
	gpio_digitalWrite(MOTOR_A1, LOW);
	gpio_digitalWrite(MOTOR_B0, LOW);
	gpio_digitalWrite(MOTOR_B1, LOW);
	
	printk("L298N driver registered!\n");
	return 0;
}

static void l298n_exit(void)
{	
	gpio_digitalWrite(MOTOR_A0, LOW);
	gpio_digitalWrite(MOTOR_A1, LOW);
	gpio_digitalWrite(MOTOR_B0, LOW);
	gpio_digitalWrite(MOTOR_B1, LOW);
	
	iounmap(pGpio);
	iounmap(pPwm);
	iounmap(pClk);
	
	misc_deregister(&misc_dev);
	printk("L298N driver unregistered!\n");
}

module_init(l298n_init);
module_exit(l298n_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Taewon Yoon <nyuan01@gmail.com>");
MODULE_DESCRIPTION("l298n motor driver module");
