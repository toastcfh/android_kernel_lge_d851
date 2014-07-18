/*
 * AD5421 Digital to analog converters  driver
 *
 * Copyright 2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2.
 */

#include <linux/device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/sysfs.h>

#include "../iio.h"
#include "../sysfs.h"
#include "../events.h"
#include "dac.h"
#include "ad5421.h"


#define AD5421_REG_DAC_DATA		0x1
#define AD5421_REG_CTRL			0x2
#define AD5421_REG_OFFSET		0x3
#define AD5421_REG_GAIN			0x4
/*                                                                             
                                                                             */
#define AD5421_REG_LOAD_DAC		0x5
#define AD5421_REG_FAULT		0x5
#define AD5421_REG_FORCE_ALARM_CURRENT	0x6
#define AD5421_REG_RESET		0x7
#define AD5421_REG_START_CONVERSION	0x8
#define AD5421_REG_NOOP			0x9

#define AD5421_CTRL_WATCHDOG_DISABLE	BIT(12)
#define AD5421_CTRL_AUTO_FAULT_READBACK	BIT(11)
#define AD5421_CTRL_MIN_CURRENT		BIT(9)
#define AD5421_CTRL_ADC_SOURCE_TEMP	BIT(8)
#define AD5421_CTRL_ADC_ENABLE		BIT(7)
#define AD5421_CTRL_PWR_DOWN_INT_VREF	BIT(6)

#define AD5421_FAULT_SPI			BIT(15)
#define AD5421_FAULT_PEC			BIT(14)
#define AD5421_FAULT_OVER_CURRENT		BIT(13)
#define AD5421_FAULT_UNDER_CURRENT		BIT(12)
#define AD5421_FAULT_TEMP_OVER_140		BIT(11)
#define AD5421_FAULT_TEMP_OVER_100		BIT(10)
#define AD5421_FAULT_UNDER_VOLTAGE_6V		BIT(9)
#define AD5421_FAULT_UNDER_VOLTAGE_12V		BIT(8)

/*                                                */
#define AD5421_FAULT_TRIGGER_IRQ \
	(AD5421_FAULT_SPI | AD5421_FAULT_PEC | AD5421_FAULT_OVER_CURRENT | \
	AD5421_FAULT_UNDER_CURRENT | AD5421_FAULT_TEMP_OVER_140)

/* 
                                                      
                    
                                 
                                                                   
                               
                                           
 */
struct ad5421_state {
	struct spi_device		*spi;
	unsigned int			ctrl;
	enum ad5421_current_range	current_range;
	unsigned int			fault_mask;

	/*
                                                       
                                                      
  */
	union {
		u32 d32;
		u8 d8[4];
	} data[2] ____cacheline_aligned;
};

static const struct iio_chan_spec ad5421_channels[] = {
	{
		.type = IIO_CURRENT,
		.indexed = 1,
		.output = 1,
		.channel = 0,
		.info_mask = IIO_CHAN_INFO_SCALE_SHARED_BIT |
			IIO_CHAN_INFO_OFFSET_SHARED_BIT |
			IIO_CHAN_INFO_CALIBSCALE_SEPARATE_BIT |
			IIO_CHAN_INFO_CALIBBIAS_SEPARATE_BIT,
		.scan_type = IIO_ST('u', 16, 16, 0),
		.event_mask = IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING) |
			IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_FALLING),
	},
	{
		.type = IIO_TEMP,
		.channel = -1,
		.event_mask = IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING),
	},
};

static int ad5421_write_unlocked(struct iio_dev *indio_dev,
	unsigned int reg, unsigned int val)
{
	struct ad5421_state *st = iio_priv(indio_dev);

	st->data[0].d32 = cpu_to_be32((reg << 16) | val);

	return spi_write(st->spi, &st->data[0].d8[1], 3);
}

static int ad5421_write(struct iio_dev *indio_dev, unsigned int reg,
	unsigned int val)
{
	int ret;

	mutex_lock(&indio_dev->mlock);
	ret = ad5421_write_unlocked(indio_dev, reg, val);
	mutex_unlock(&indio_dev->mlock);

	return ret;
}

static int ad5421_read(struct iio_dev *indio_dev, unsigned int reg)
{
	struct ad5421_state *st = iio_priv(indio_dev);
	struct spi_message m;
	int ret;
	struct spi_transfer t[] = {
		{
			.tx_buf = &st->data[0].d8[1],
			.len = 3,
			.cs_change = 1,
		}, {
			.rx_buf = &st->data[1].d8[1],
			.len = 3,
		},
	};

	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	spi_message_add_tail(&t[1], &m);

	mutex_lock(&indio_dev->mlock);

	st->data[0].d32 = cpu_to_be32((1 << 23) | (reg << 16));

	ret = spi_sync(st->spi, &m);
	if (ret >= 0)
		ret = be32_to_cpu(st->data[1].d32) & 0xffff;

	mutex_unlock(&indio_dev->mlock);

	return ret;
}

static int ad5421_update_ctrl(struct iio_dev *indio_dev, unsigned int set,
	unsigned int clr)
{
	struct ad5421_state *st = iio_priv(indio_dev);
	unsigned int ret;

	mutex_lock(&indio_dev->mlock);

	st->ctrl &= ~clr;
	st->ctrl |= set;

	ret = ad5421_write_unlocked(indio_dev, AD5421_REG_CTRL, st->ctrl);

	mutex_unlock(&indio_dev->mlock);

	return ret;
}

static irqreturn_t ad5421_fault_handler(int irq, void *data)
{
	struct iio_dev *indio_dev = data;
	struct ad5421_state *st = iio_priv(indio_dev);
	unsigned int fault;
	unsigned int old_fault = 0;
	unsigned int events;

	fault = ad5421_read(indio_dev, AD5421_REG_FAULT);
	if (!fault)
		return IRQ_NONE;

	/*                                                                   
                                                                    
                                                                       
                                                                        
                  */
	ad5421_update_ctrl(indio_dev, 0, 0);


	/*                                                                     
                                                                  
                                                                   
                                                                        
                                                                    
                                                                        
                                           
  */
	do {
		/*                                                            
                                                  */
		if (fault == 0xffff)
			fault = 0;

		/*                                      */
		events = (old_fault ^ fault) & fault;
		events &= st->fault_mask;

		if (events & AD5421_FAULT_OVER_CURRENT) {
			iio_push_event(indio_dev,
				IIO_UNMOD_EVENT_CODE(IIO_CURRENT,
					0,
					IIO_EV_TYPE_THRESH,
					IIO_EV_DIR_RISING),
			iio_get_time_ns());
		}

		if (events & AD5421_FAULT_UNDER_CURRENT) {
			iio_push_event(indio_dev,
				IIO_UNMOD_EVENT_CODE(IIO_CURRENT,
					0,
					IIO_EV_TYPE_THRESH,
					IIO_EV_DIR_FALLING),
				iio_get_time_ns());
		}

		if (events & AD5421_FAULT_TEMP_OVER_140) {
			iio_push_event(indio_dev,
				IIO_UNMOD_EVENT_CODE(IIO_TEMP,
					0,
					IIO_EV_TYPE_MAG,
					IIO_EV_DIR_RISING),
				iio_get_time_ns());
		}

		old_fault = fault;
		fault = ad5421_read(indio_dev, AD5421_REG_FAULT);

		/*                                         */
		if (fault & AD5421_FAULT_TRIGGER_IRQ)
			msleep(1000);

	} while (fault & AD5421_FAULT_TRIGGER_IRQ);


	return IRQ_HANDLED;
}

static void ad5421_get_current_min_max(struct ad5421_state *st,
	unsigned int *min, unsigned int *max)
{
	/*                                                               
                                                    */
	switch (st->current_range) {
	case AD5421_CURRENT_RANGE_4mA_20mA:
		*min = 4000;
		*max = 20000;
		break;
	case AD5421_CURRENT_RANGE_3mA8_21mA:
		*min = 3800;
		*max = 21000;
		break;
	case AD5421_CURRENT_RANGE_3mA2_24mA:
		*min = 3200;
		*max = 24000;
		break;
	default:
		*min = 0;
		*max = 1;
		break;
	}
}

static inline unsigned int ad5421_get_offset(struct ad5421_state *st)
{
	unsigned int min, max;

	ad5421_get_current_min_max(st, &min, &max);
	return (min * (1 << 16)) / (max - min);
}

static inline unsigned int ad5421_get_scale(struct ad5421_state *st)
{
	unsigned int min, max;

	ad5421_get_current_min_max(st, &min, &max);
	return ((max - min) * 1000) / (1 << 16);
}

static int ad5421_read_raw(struct iio_dev *indio_dev,
	struct iio_chan_spec const *chan, int *val, int *val2, long m)
{
	struct ad5421_state *st = iio_priv(indio_dev);
	int ret;

	if (chan->type != IIO_CURRENT)
		return -EINVAL;

	switch (m) {
	case 0:
		ret = ad5421_read(indio_dev, AD5421_REG_DAC_DATA);
		if (ret < 0)
			return ret;
		*val = ret;
		return IIO_VAL_INT;
	case IIO_CHAN_INFO_SCALE:
		*val = 0;
		*val2 = ad5421_get_scale(st);
		return IIO_VAL_INT_PLUS_MICRO;
	case IIO_CHAN_INFO_OFFSET:
		*val = ad5421_get_offset(st);
		return IIO_VAL_INT;
	case IIO_CHAN_INFO_CALIBBIAS:
		ret = ad5421_read(indio_dev, AD5421_REG_OFFSET);
		if (ret < 0)
			return ret;
		*val = ret - 32768;
		return IIO_VAL_INT;
	case IIO_CHAN_INFO_CALIBSCALE:
		ret = ad5421_read(indio_dev, AD5421_REG_GAIN);
		if (ret < 0)
			return ret;
		*val = ret;
		return IIO_VAL_INT;
	}

	return -EINVAL;
}

static int ad5421_write_raw(struct iio_dev *indio_dev,
	struct iio_chan_spec const *chan, int val, int val2, long mask)
{
	const unsigned int max_val = 1 << 16;

	switch (mask) {
	case 0:
		if (val >= max_val || val < 0)
			return -EINVAL;

		return ad5421_write(indio_dev, AD5421_REG_DAC_DATA, val);
	case IIO_CHAN_INFO_CALIBBIAS:
		val += 32768;
		if (val >= max_val || val < 0)
			return -EINVAL;

		return ad5421_write(indio_dev, AD5421_REG_OFFSET, val);
	case IIO_CHAN_INFO_CALIBSCALE:
		if (val >= max_val || val < 0)
			return -EINVAL;

		return ad5421_write(indio_dev, AD5421_REG_GAIN, val);
	default:
		break;
	}

	return -EINVAL;
}

static int ad5421_write_event_config(struct iio_dev *indio_dev,
	u64 event_code, int state)
{
	struct ad5421_state *st = iio_priv(indio_dev);
	unsigned int mask;

	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_CURRENT:
		if (IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			IIO_EV_DIR_RISING)
			mask = AD5421_FAULT_OVER_CURRENT;
		else
			mask = AD5421_FAULT_UNDER_CURRENT;
		break;
	case IIO_TEMP:
		mask = AD5421_FAULT_TEMP_OVER_140;
		break;
	default:
		return -EINVAL;
	}

	mutex_lock(&indio_dev->mlock);
	if (state)
		st->fault_mask |= mask;
	else
		st->fault_mask &= ~mask;
	mutex_unlock(&indio_dev->mlock);

	return 0;
}

static int ad5421_read_event_config(struct iio_dev *indio_dev,
	u64 event_code)
{
	struct ad5421_state *st = iio_priv(indio_dev);
	unsigned int mask;

	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_CURRENT:
		if (IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			IIO_EV_DIR_RISING)
			mask = AD5421_FAULT_OVER_CURRENT;
		else
			mask = AD5421_FAULT_UNDER_CURRENT;
		break;
	case IIO_TEMP:
		mask = AD5421_FAULT_TEMP_OVER_140;
		break;
	default:
		return -EINVAL;
	}

	return (bool)(st->fault_mask & mask);
}

static int ad5421_read_event_value(struct iio_dev *indio_dev, u64 event_code,
	int *val)
{
	int ret;

	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_CURRENT:
		ret = ad5421_read(indio_dev, AD5421_REG_DAC_DATA);
		if (ret < 0)
			return ret;
		*val = ret;
		break;
	case IIO_TEMP:
		*val = 140000;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static const struct iio_info ad5421_info = {
	.read_raw =		ad5421_read_raw,
	.write_raw =		ad5421_write_raw,
	.read_event_config =	ad5421_read_event_config,
	.write_event_config =	ad5421_write_event_config,
	.read_event_value =	ad5421_read_event_value,
	.driver_module =	THIS_MODULE,
};

static int __devinit ad5421_probe(struct spi_device *spi)
{
	struct ad5421_platform_data *pdata = dev_get_platdata(&spi->dev);
	struct iio_dev *indio_dev;
	struct ad5421_state *st;
	int ret;

	indio_dev = iio_allocate_device(sizeof(*st));
	if (indio_dev == NULL) {
		dev_err(&spi->dev, "Failed to allocate iio device\n");
		return  -ENOMEM;
	}

	st = iio_priv(indio_dev);
	spi_set_drvdata(spi, indio_dev);

	st->spi = spi;

	indio_dev->dev.parent = &spi->dev;
	indio_dev->name = "ad5421";
	indio_dev->info = &ad5421_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = ad5421_channels;
	indio_dev->num_channels = ARRAY_SIZE(ad5421_channels);

	st->ctrl = AD5421_CTRL_WATCHDOG_DISABLE |
			AD5421_CTRL_AUTO_FAULT_READBACK;

	if (pdata) {
		st->current_range = pdata->current_range;
		if (pdata->external_vref)
			st->ctrl |= AD5421_CTRL_PWR_DOWN_INT_VREF;
	} else {
		st->current_range = AD5421_CURRENT_RANGE_4mA_20mA;
	}

	/*                                   */
	ad5421_update_ctrl(indio_dev, 0, 0);

	if (spi->irq) {
		ret = request_threaded_irq(spi->irq,
					   NULL,
					   ad5421_fault_handler,
					   IRQF_TRIGGER_HIGH | IRQF_ONESHOT,
					   "ad5421 fault",
					   indio_dev);
		if (ret)
			goto error_free;
	}

	ret = iio_device_register(indio_dev);
	if (ret) {
		dev_err(&spi->dev, "Failed to register iio device: %d\n", ret);
		goto error_free_irq;
	}

	return 0;

error_free_irq:
	if (spi->irq)
		free_irq(spi->irq, indio_dev);
error_free:
	iio_free_device(indio_dev);

	return ret;
}

static int __devexit ad5421_remove(struct spi_device *spi)
{
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	iio_device_unregister(indio_dev);
	if (spi->irq)
		free_irq(spi->irq, indio_dev);
	iio_free_device(indio_dev);

	return 0;
}

static struct spi_driver ad5421_driver = {
	.driver = {
		   .name = "ad5421",
		   .owner = THIS_MODULE,
	},
	.probe = ad5421_probe,
	.remove = __devexit_p(ad5421_remove),
};
module_spi_driver(ad5421_driver);

MODULE_AUTHOR("Lars-Peter Clausen <lars@metafoo.de>");
MODULE_DESCRIPTION("Analog Devices AD5421 DAC");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("spi:ad5421");