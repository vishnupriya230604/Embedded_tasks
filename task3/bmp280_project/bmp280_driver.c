/**
 * @file bmp280_driver.c
 * @brief A driver for reading out a BMP280 temperature sensor using I2C
 * @author Vishnupriya K
 * @date 2025
 * @license GPL
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define DRIVER_NAME "bmp280"
#define DRIVER_CLASS "bmp280Class"

static struct i2c_adapter *bmp_i2c_adapter = NULL;
static struct i2c_client *bmp280_i2c_client = NULL;

/** 
 * @brief Module metadata
 */
MODULE_AUTHOR("Vishnupriya K");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out a BMP280 temperature sensor");
MODULE_SUPPORTED_DEVICE("NONE");


/**
 * @brief Defines for device identification
 */
#define I2C_BUS_AVAILABLE    1       /* The I2C Bus available on the raspberry */
#define SLAVE_DEVICE_NAME    "BMP280"    /* Device and Driver Name */
#define BMP280_SLAVE_ADDRESS 0x76     /* BMP280 I2C address */

/**
 * @brief Variables for Device and Deviceclass
 */
static dev_t myDeviceNr;
static struct class *myClass;
static struct cdev myDevice;

/**
 * @brief Variables for BMP280 temperature Calibration
 */
static s32 dig_T1, dig_T2, dig_T3;

/**
 * @brief Read current temperature from BMP280 sensor
 *
 * @return temperature in degree
 */
static s32 read_temperature(void) {
    int var1, var2;
    s32 raw_temp;
    s32 d1, d2, d3;

    /* Read Temperature */
    d1 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFA);
    d2 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFB);
    d3 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFC);
    
    if (d1 < 0 || d2 < 0 || d3 < 0) {
        pr_err("Failed to read temperature data\n");
        return -EIO;
    }
    
    raw_temp = ((d1<<16) | (d2<<8) | d3) >> 4;

    /* Calculate temperature in degree */
    var1 = ((((raw_temp >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;
    var2 = (((((raw_temp >> 4) - (dig_T1)) * ((raw_temp >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;
    
    return ((var1 + var2) * 5 + 128) >> 8;
}

/**
 * @brief Read function for the driver
 *
 * @param File File pointer
 * @param user_buffer Buffer to store data
 * @param count Amount of bytes to read
 * @param offs Offset
 * @return Amount of bytes read
 */

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
    int to_copy, not_copied, delta;
    char out_string[20];
    int temperature;

    /* Get amount of bytes to copy */
    to_copy = min(sizeof(out_string), count);

    /* Get temperature */
    temperature = read_temperature();
    if (temperature < 0)
        return temperature;

    snprintf(out_string, sizeof(out_string), "%d.%d\n", temperature/100, temperature%100);

    /* Copy Data to user */
    not_copied = copy_to_user(user_buffer, out_string, to_copy);

    /* Calculate delta */
    delta = to_copy - not_copied;

    return delta;
}

/**
 * @brief Open function for the driver
 *
 * @param deviceFile Device file pointer
 * @param instance File pointer
 * @return 0
 */
static int driver_open(struct inode *deviceFile, struct file *instance) {
    pr_info("BMP280 - Open was called\n");
    return 0;
}

/**
 * @brief Close function for the driver
 *
 * @param deviceFile Device file pointer
 * @param instance File pointer
 * @return 0
 */
static int driver_close(struct inode *deviceFile, struct file *instance) {
    pr_info("BMP280 - Close was called\n");
    return 0;
}

/**
 * @brief File operations for the driver
 */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
};

/**
 * @brief Probe function for the driver
 *
 * @param client I2C client
 * @param id I2C device ID
 * @return 0
 */
static int bmp280_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    u8 chip_id;
    
    /* Read Chip ID */
    chip_id = i2c_smbus_read_byte_data(client, 0xD0);
    if (chip_id < 0) {
        pr_err("Failed to read chip ID\n");
        return -EIO;
    }
    pr_info("BMP280 Chip ID: 0x%x\n", chip_id);

    /* Read Calibration Values */
    dig_T1 = i2c_smbus_read_word_data(client, 0x88);
    dig_T2 = i2c_smbus_read_word_data(client, 0x8a);
    dig_T3 = i2c_smbus_read_word_data(client, 0x8c);

    if (dig_T2 > 32767)
        dig_T2 -= 65536;
    if (dig_T3 > 32767)
        dig_T3 -= 65536;

    /* Initialize the sensor */
    i2c_smbus_write_byte_data(client, 0xf5, 5<<5);
    i2c_smbus_write_byte_data(client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));

    return 0;
}

/**
 * @brief Remove function for the driver
 *
 * @param client I2C client
 * @return 0
 */
static int bmp280_remove(struct i2c_client *client) {
    pr_info("BMP280 removed\n");
    return 0;
}

/**
 * @brief I2C Device ID
 */
static const struct i2c_device_id bmp280_id[] = {
    { SLAVE_DEVICE_NAME, 0 },
    { }
};

/**
 * @brief I2C Device Table
 */
MODULE_DEVICE_TABLE(i2c, bmp280_id);

static struct i2c_driver bmp280_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE,
    },
    .probe = bmp280_probe,
    .remove = bmp280_remove,
    .id_table = bmp280_id,
};

/**
 * @brief I2C Board Info
 */
static struct i2c_board_info bmp_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, BMP280_SLAVE_ADDRESS)
};

/**
 * @brief Module initialization function
 *
 * @return 0
 */
static int __init ModuleInit(void) {
    int ret = -1;
    pr_info("BMP280 - Initializing\n");

    /* Allocate Device Nr */
    if (alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME) < 0) {
        pr_err("Device Nr. could not be allocated!\n");
        return -EIO;
    }

    /* Create Device Class */
    if ((myClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        pr_err("Device Class cannot be created!\n");
        goto ClassError;
    }

    /* Create Device file */
    if (device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME) == NULL) {
        pr_err("Cannot create device file!\n");
        goto FileError;
    }

    /* Initialize Device file */
    cdev_init(&myDevice, &fops);

    /* Register device to kernel */
    if (cdev_add(&myDevice, myDeviceNr, 1) == -1) {
        pr_err("Registering of device to kernel failed!\n");
        goto KernelError;
    }

    /* Setup I2C */
    bmp_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (bmp_i2c_adapter != NULL) {
        bmp280_i2c_client = i2c_new_client_device(bmp_i2c_adapter, &bmp_i2c_board_info);
        if (bmp280_i2c_client != NULL) {
            if (i2c_add_driver(&bmp280_driver) != -1) {
                ret = 0;
            } else {
                pr_err("Can't add driver\n");
            }
        }
        i2c_put_adapter(bmp_i2c_adapter);
    }

    if (ret == 0) {
        pr_info("BMP280 Driver added!\n");
        return 0;
    }
// Error handling
KernelError:
    device_destroy(myClass, myDeviceNr);
FileError:
    class_destroy(myClass);
ClassError:
    unregister_chrdev_region(myDeviceNr, 1);
    return -1;
}

/**
 * @brief Module exit function
 */
static void __exit ModuleExit(void) {
    i2c_unregister_device(bmp280_i2c_client);
    i2c_del_driver(&bmp280_driver);
    cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNr);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNr, 1);
    pr_info("BMP280 - Goodbye, Kernel!\n");
}

// Define module initialization and exit functions
module_init(ModuleInit);
module_exit(ModuleExit);