#include <linux/module.h>   /* Needed by all modules */
#include <linux/init.h>		/* Needed for the macros */

#include <mach/hardware.h>
#include <mach/cgu.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <mach/gpio.h>

static unsigned int gpioVal = GPIO_GPIO20;
static char *gpio = "GPIO_GPIO20";
module_param(gpio, charp, S_IRUGO);

static int value = 0;
module_param(value, int, S_IRUGO);

extern void (*pm_power_off)(void);
extern int lpc313x_gpio_direction_output(unsigned gpio, int value);
void (*old_pm_power_off)(void);

typedef struct {
	const char *name;
	unsigned int gpioID;
	} gpioListSt;

gpioListSt gpioList[] = 
	{
		{"GPIO_MGPIO9"              ,GPIO_MGPIO9        },
		{"GPIO_MGPIO6"              ,GPIO_MGPIO6        },
		{"GPIO_MLCD_DB_7"           ,GPIO_MLCD_DB_7     },
		{"GPIO_MLCD_DB_4"           ,GPIO_MLCD_DB_4     },
		{"GPIO_MLCD_DB_2"           ,GPIO_MLCD_DB_2     },
		{"GPIO_MNAND_RYBN0"         ,GPIO_MNAND_RYBN0   },
		{"GPIO_MI2STX_CLK0"         ,GPIO_MI2STX_CLK0   },
		{"GPIO_MI2STX_BCK0"         ,GPIO_MI2STX_BCK0   },
		{"GPIO_EBI_A_1_CLE"         ,GPIO_EBI_A_1_CLE   },
		{"GPIO_EBI_NCAS_BLOUT"      ,GPIO_EBI_NCAS_BLOUT},
		{"GPIO_MLCD_DB_0"           ,GPIO_MLCD_DB_0     },
		{"GPIO_EBI_DQM_0_NOE"       ,GPIO_EBI_DQM_0_NOE },
		{"GPIO_MLCD_CSB"            ,GPIO_MLCD_CSB      },
		{"GPIO_MLCD_DB_1"           ,GPIO_MLCD_DB_1     },
		{"GPIO_MLCD_E_RD"           ,GPIO_MLCD_E_RD     },
		{"GPIO_MLCD_RS"             ,GPIO_MLCD_RS       },
		{"GPIO_MLCD_RW_WR"          ,GPIO_MLCD_RW_WR    },
		{"GPIO_MLCD_DB_3"           ,GPIO_MLCD_DB_3     },
		{"GPIO_MLCD_DB_5"           ,GPIO_MLCD_DB_5     },
		{"GPIO_MLCD_DB_6"           ,GPIO_MLCD_DB_6     },
		{"GPIO_MLCD_DB_8"           ,GPIO_MLCD_DB_8     },
		{"GPIO_MLCD_DB_9"           ,GPIO_MLCD_DB_9     },
		{"GPIO_MLCD_DB_10"          ,GPIO_MLCD_DB_10    },
		{"GPIO_MLCD_DB_11"          ,GPIO_MLCD_DB_11    },
		{"GPIO_MLCD_DB_12"          ,GPIO_MLCD_DB_12    },
		{"GPIO_MLCD_DB_13"          ,GPIO_MLCD_DB_13    },
		{"GPIO_MLCD_DB_14"          ,GPIO_MLCD_DB_14    },
		{"GPIO_MLCD_DB_15"          ,GPIO_MLCD_DB_15    },
		{"GPIO_MGPIO5"              ,GPIO_MGPIO5        },
		{"GPIO_MGPIO7"              ,GPIO_MGPIO7        },
		{"GPIO_MGPIO8"              ,GPIO_MGPIO8        },
		{"GPIO_MGPIO10"             ,GPIO_MGPIO10       },
		{"GPIO_MNAND_RYBN1"         ,GPIO_MNAND_RYBN1   },
		{"GPIO_MNAND_RYBN2"         ,GPIO_MNAND_RYBN2   },
		{"GPIO_MNAND_RYBN3"         ,GPIO_MNAND_RYBN3   },
		{"GPIO_MUART_CTS_N"         ,GPIO_MUART_CTS_N   },
		{"GPIO_MUART_RTS_N"         ,GPIO_MUART_RTS_N   },
		{"GPIO_MI2STX_DATA0"        ,GPIO_MI2STX_DATA0  },
		{"GPIO_MI2STX_WS0"          ,GPIO_MI2STX_WS0    },
		{"GPIO_EBI_NRAS_BLOUT"      ,GPIO_EBI_NRAS_BLOUT},
		{"GPIO_EBI_A_0_ALE"         ,GPIO_EBI_A_0_ALE   },
		{"GPIO_EBI_NWE"             ,GPIO_EBI_NWE       },
		{"GPIO_CGU_SYSCLK_O"        ,GPIO_CGU_SYSCLK_O  },
		{"GPIO_I2SRX_BCK0"          ,GPIO_I2SRX_BCK0    },
		{"GPIO_I2SRX_DATA0"         ,GPIO_I2SRX_DATA0   },
		{"GPIO_I2SRX_WS0"           ,GPIO_I2SRX_WS0     },
		{"GPIO_I2SRX_DATA1"         ,GPIO_I2SRX_DATA1   },
		{"GPIO_I2SRX_BCK1"          ,GPIO_I2SRX_BCK1    },
		{"GPIO_I2SRX_WS1"           ,GPIO_I2SRX_WS1     },
		{"GPIO_I2STX_DATA1"         ,GPIO_I2STX_DATA1   },
		{"GPIO_I2STX_BCK1"          ,GPIO_I2STX_BCK1    },
		{"GPIO_I2STX_WS1"           ,GPIO_I2STX_WS1     },
		{"GPIO_I2STX_256FS_O"       ,GPIO_I2STX_256FS_O },
		{"GPIO_EBI_D_9"             ,GPIO_EBI_D_9       },
		{"GPIO_EBI_D_10"            ,GPIO_EBI_D_10      },
		{"GPIO_EBI_D_11"            ,GPIO_EBI_D_11      },
		{"GPIO_EBI_D_12"            ,GPIO_EBI_D_12      },
		{"GPIO_EBI_D_13"            ,GPIO_EBI_D_13      },
		{"GPIO_EBI_D_14"            ,GPIO_EBI_D_14      },
		{"GPIO_EBI_D_4"             ,GPIO_EBI_D_4       },
		{"GPIO_EBI_D_0"             ,GPIO_EBI_D_0       },
		{"GPIO_EBI_D_1"             ,GPIO_EBI_D_1       },
		{"GPIO_EBI_D_2"             ,GPIO_EBI_D_2       },
		{"GPIO_EBI_D_3"             ,GPIO_EBI_D_3       },
		{"GPIO_EBI_D_5"             ,GPIO_EBI_D_5       },
		{"GPIO_EBI_D_6"             ,GPIO_EBI_D_6       },
		{"GPIO_EBI_D_7"             ,GPIO_EBI_D_7       },
		{"GPIO_EBI_D_8"             ,GPIO_EBI_D_8       },
		{"GPIO_EBI_D_15"            ,GPIO_EBI_D_15      },
		{"GPIO_GPIO1"               ,GPIO_GPIO1         },
		{"GPIO_GPIO0"               ,GPIO_GPIO0         },
		{"GPIO_GPIO2"               ,GPIO_GPIO2         },
		{"GPIO_GPIO3"               ,GPIO_GPIO3         },
		{"GPIO_GPIO4"               ,GPIO_GPIO4         },
		{"GPIO_GPIO11"              ,GPIO_GPIO11        },
		{"GPIO_GPIO12"              ,GPIO_GPIO12        },
		{"GPIO_GPIO13"              ,GPIO_GPIO13        },
		{"GPIO_GPIO14"              ,GPIO_GPIO14        },
		{"GPIO_GPIO15"              ,GPIO_GPIO15        },
		{"GPIO_GPIO16"              ,GPIO_GPIO16        },
		{"GPIO_GPIO17"              ,GPIO_GPIO17        },
		{"GPIO_GPIO18"              ,GPIO_GPIO18        },
		{"GPIO_GPIO19"              ,GPIO_GPIO19        },
		{"GPIO_GPIO20"              ,GPIO_GPIO20        },
		{"GPIO_I2C_SDA1"            ,GPIO_I2C_SDA1      },
		{"GPIO_I2C_SCL1"            ,GPIO_I2C_SCL1      },
		{"GPIO_SPI_MISO"            ,GPIO_SPI_MISO      },
		{"GPIO_SPI_MOSI"            ,GPIO_SPI_MOSI      },
		{"GPIO_SPI_CS_IN"           ,GPIO_SPI_CS_IN     },
		{"GPIO_SPI_SCK"             ,GPIO_SPI_SCK       },
		{"GPIO_SPI_CS_OUT0"         ,GPIO_SPI_CS_OUT0   },
		{"GPIO_NAND_NCS_3"          ,GPIO_NAND_NCS_3    },
		{"GPIO_NAND_NCS_0"          ,GPIO_NAND_NCS_0    },
		{"GPIO_NAND_NCS_1"          ,GPIO_NAND_NCS_1    },
		{"GPIO_NAND_NCS_2"          ,GPIO_NAND_NCS_2    },
		{"GPIO_PWM_DATA"            ,GPIO_PWM_DATA      },
		{"GPIO_UART_RXD"            ,GPIO_UART_RXD      },
		{"GPIO_UART_TXD"            ,GPIO_UART_TXD      },
		{NULL,0}
};
	


int poweroff_str_to_gpio(void)
{
	
	gpioListSt *p=&(gpioList[0]);
	
	while (p->name)
	{
		if (strcmp(p->name,gpio)==0)
		{
			gpioVal = p->gpioID;
			return 1;
		}
		p++;
	}
	return -1;
	
}


void poweroff_gpio_handler(void)
{
	printk("power off, setting gpio pin to '%d'\n",value);
	lpc313x_gpio_direction_output(gpioVal,value);
	
	mdelay(5000);
	
	// Fallback to arch reset in case that poweroff system doesn't work...
	printk("power off hardware doesn't seem to work, fall back to reset..\n");
	
		/* Disable watchdog */
	WDT_TCR = 0;
	WDT_MCR = WDT_MCR_STOP_MR1 | WDT_MCR_INT_MR1;

	/*  If TC and MR1 are equal a reset is generated. */
	WDT_PR  = 0x00000002;
	WDT_TC  = 0x00000FF0;
	WDT_MR0 = 0x0000F000;
	WDT_MR1 = 0x00001000;
	WDT_EMR = WDT_EMR_CTRL1(0x3);
	/* Enable watchdog timer; assert reset at timer timeout */
	WDT_TCR = WDT_TCR_CNT_EN;
	
	
}

int init_module(void)
{

	if (poweroff_str_to_gpio()>0)
	{
		old_pm_power_off = pm_power_off;
		pm_power_off = poweroff_gpio_handler;
		printk("NB31xx External Poweroff handler installed for pin %s...\n",gpio);
		return 0;
	} else
	{
		printk("NB31xx External Poweroff: Unknown pin %s...\n",gpio);
		
		return -EINTR;
	}
	
	
	
}

void cleanup_module(void)
{
//	pm_power_off = old_pm_power_off;
}

MODULE_LICENSE("GPL");       
MODULE_AUTHOR("Miguel Angel Ajo Pelayo");         
MODULE_DESCRIPTION("Power off handler that will send a GPIO output at power off");  
