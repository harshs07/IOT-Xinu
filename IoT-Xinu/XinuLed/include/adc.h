/* Clock Module Memory Registers */
#define CM_WKUP_ADC_TSC_CLKCTRL (0x44E004BC)
#define CM_WKUP_MODULEMODE_ENABLE (0x02)
#define CM_WKUP_IDLEST_DISABLED (0x03<<16)

#define CM_PER (0x44E00000)
#define CM_PER_EPWMSS1_CLKCTRL (CM_PER+0xCC)
#define CM_PER_EPWMSS0_CLKCTRL (CM_PER+0xD4)
#define CM_PER_EPWMSS2_CLKCTRL (CM_PER+0xD8)


/* Analog Digital Converter Memory Registers */
// ADC control register.
#define ADC_SETUP (~(0x03<<5))
#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_ENABLE (0x01)

// ADC set enable register.
#define ADC_RESET (0x0)
#define ADC_STEP_ENABLE_1  (0x01<<1)

// Step Mode Configuration.
#define STEP_MODE_SW_ONE_SHOT (0x00)
#define STEP_MODE_SW_CONT     (0x01)

// Step Averaging.
#define STEP_AVG0  (0x0<<2)
#define STEP_AVG2  (0x1<<2)
#define STEP_AVG4  (0x2<<2)
#define STEP_AVG8  (0x3<<2)
#define STEP_AVG16 (0x4<<2)

// Step Channel
#define STEP_CHANNEL_0  ((0x0)<<19)
#define STEP_CHANNEL_1  ((0x1)<<19)
#define STEP_CHANNEL_2  ((0x2)<<19)
#define STEP_CHANNEL_3  ((0x3)<<19)
#define STEP_CHANNEL_4  ((0x4)<<19)
#define STEP_CHANNEL_5  ((0x5)<<19)
#define STEP_CHANNEL_6  ((0x6)<<19)
#define STEP_CHANNEL_7  ((0x7)<<19)

// Step Select output fifo.
#define STEP_FIFO0  ~((0x1)<<26)
#define STEP_FIFO1  ((0x1)<<26)

//#define ADC_FIFO0DATA (ADC_TSC+0x100)
#define ADC_FIFO0DATA (0x100)
#define ADC_FIFO_MASK (0xFFF)

// FIFO threshold interrupt.
#define THRESHOLD_INTERRUPT (0x01<<2)

struct step_struct
{
  volatile uint32 step_config;
  volatile uint32 step_delay;
};

struct fifo_struct
{
  volatile uint32 fifo_count;
  volatile uint32 fifo_threshold;
  volatile uint32 dma_req;
};

struct adc_csr
{
	volatile uint32 revison;
	volatile uint32 reserved1[3];
	volatile uint32 sysconfig;
	volatile uint32 reserved2[4];
	volatile uint32 irqstatus_raw;
	volatile uint32 irq_status;
	volatile uint32 irq_enable_set;
	volatile uint32 irq_enable_clr;
	volatile uint32 irq_wakeup;
	volatile uint32 dma_enable_set;
	volatile uint32 dma_enable_clr;
	volatile uint32 ctrl;
	volatile uint32 adc_stat;
	volatile uint32 adc_range;
	volatile uint32 adc_clk_div;
	volatile uint32 adc_misc;
	volatile uint32 step_enable;
	volatile uint32 idle_config;
	volatile uint32 ts_charge_step_config;
	volatile uint32 ts_charge_delay;
	struct step_struct step[16];
	struct fifo_struct fifoInfo[2];
	//volatile uint32 reserved3; --check this!!!!
	volatile uint32 fifo_data0[64];
  volatile uint32 fifo_data1[64];
};

// semaphore for co-ordination.
extern sid32 semADC;
extern uint32 tempVoltage;
