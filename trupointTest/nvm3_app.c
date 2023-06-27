#include <stdio.h>
#include <string.h>

#include "nvm3_app.h"
#include "nvm3_default.h"
#include "nvm3_default_config.h"
//#include "sl_cli.h"
//#include "sl_cli_instances.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

// Maximum number of data objects saved
#define MAX_OBJECT_COUNT    115

// Max and min keys for data objects
#define MIN_DATA_KEY  NVM3_KEY_MIN
#define MAX_DATA_KEY  (MIN_DATA_KEY + MAX_OBJECT_COUNT - 1)

// Key of write counter object
#define WRITE_COUNTER_KEY   MAX_OBJECT_COUNT

// Key of delete counter object
#define DELETE_COUNTER_KEY   (WRITE_COUNTER_KEY + 1)

// Use the default nvm3 handle from nvm3_default.h
#define NVM3_DEFAULT_HANDLE nvm3_defaultHandle
#define rbuffer_size  301

/*******************************************************************************
 **************************   LOCAL VARIABLES   ********************************
 ******************************************************************************/
//int NVM3_DEFAULT_MAX_OBJECT_SIZE = 1600;

// Buffer for reading from NVM3
static char buffer[NVM3_DEFAULT_MAX_OBJECT_SIZE];

//char key_data[NVM3_DEFAULT_MAX_OBJECT_SIZE];
/*******************************************************************************
 **************************   LOCAL FUNCTIONS   ********************************
 ******************************************************************************/
static void initialise_counters(void)
{
  uint32_t type;
  size_t len;
  Ecode_t err;

  // check if the designated keys contain counters, and initialise if needed.
  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, &type, &len);
  if ((err != ECODE_NVM3_OK) || (type != NVM3_OBJECTTYPE_COUNTER)) {
    nvm3_writeCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, 0);
  }

  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, &type, &len);
  if ((err != ECODE_NVM3_OK) || (type != NVM3_OBJECTTYPE_COUNTER)) {
    nvm3_writeCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, 0);
  }
}
/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Write data to NVM3
 *
 * This function implements the CLI command 'write' (see nvm3_app.slcp)
 * It stores string data at the selected NVM3 key.
 ******************************************************************************/
void nvm3_app_write(uint16_t *data, uint16_t len, uint32_t key)
{
  if (key > MAX_DATA_KEY) {
//    printf("Invalid key\r\n");
    return;
  }

  if (len > NVM3_DEFAULT_MAX_OBJECT_SIZE) {
//    printf("Maximum object size exceeded\r\n");
    return;
  }

  if (ECODE_NVM3_OK == nvm3_writeData(NVM3_DEFAULT_HANDLE,
                                        key,
                                        data,
                                        (2 * len))){
//    printf("Stored data at key %lu\r\n", key);
    // Track number of writes in counter object
    nvm3_incrementCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, NULL);
  } else {
      ;
//    printf("Error storing data\r\n");
  }
}

void nvm3_app_write_max(char *data, uint32_t key)
{
  size_t len;

  if (key > MAX_DATA_KEY) {
//    printf("Invalid key\r\n");
    return;
  }

  len = strlen(data);

  if (len > NVM3_DEFAULT_MAX_OBJECT_SIZE) {
//    printf("Maximum object size exceeded\r\n");
    return;
  }

  if (ECODE_NVM3_OK == nvm3_writeData(NVM3_DEFAULT_HANDLE,
                                      key,
                                      (unsigned char *)data,
                                      len)) {
//    printf("Stored data at key %lu\r\n", key);
    // Track number of writes in counter object
    nvm3_incrementCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, NULL);

  } else {
      ;
//    printf("Error storing data\r\n");
  }
}

/***************************************************************************//**
 * Delete data in NVM3.
 *
 * This function implements the CLI command 'delete' (see nvm3_app.slcp)
 * It deletes the data object stored at the selected NVM3 key.
 ******************************************************************************/

void nvm3_app_delete(uint32_t key)
{
  if (key > MAX_DATA_KEY) {
//    printf("Invalid key\r\n");
    return;
  }

  if (ECODE_NVM3_OK == nvm3_deleteObject(NVM3_DEFAULT_HANDLE, key)) {
//    printf("Deleted data at key %lu\r\n", key);
    // Track number or deletes in counter object
    nvm3_incrementCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, NULL);
  } else {
      ;
//    printf("Error deleting key\r\n");
  }
}

/***************************************************************************//**
 * Read data from NVM3.
 *
 * This function implements the CLI command 'read' (see nvm3_app.slcp)
 * It reads the data object stored at the selected NVM3 key.
 ******************************************************************************/
//extern uint16_t data_read_Buffer_[601];
void nvm3_app_read(uint16_t* read_Buffer_, uint32_t key)
{
  uint32_t type;
  size_t len;
  Ecode_t err;

//  memset(data_read_Buffer_, 0, sizeof(data_read_Buffer_));
  memset(read_Buffer_, 0, sizeof(read_Buffer_));

  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, key, &type, &len);
  if (err != NVM3_OBJECTTYPE_DATA || type != NVM3_OBJECTTYPE_DATA) {
//    printf("Key does not contain data object\r\n");
    return;
  }

  err = nvm3_readData(NVM3_DEFAULT_HANDLE, key, read_Buffer_, len);

  if (ECODE_NVM3_OK == err) {
      ;
//      buffer[len] = '\0';
//      memcpy(data_read_Buffer_, read_Buffer_, strlen(read_Buffer_)+1);

  } else {
;
//    printf("Error reading data from key %lu\r\n", key);
  }
}

extern char read_max[8];
extern char calibration[120];
char buffer_max[8];
void nvm3_app_read_max(uint32_t key)
{
  uint32_t type;
  size_t len;
  Ecode_t err;
  memset(read_max, 0, sizeof(read_max));
  memset(buffer_max, 0, sizeof(buffer_max));
  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, key, &type, &len);
  if (err != NVM3_OBJECTTYPE_DATA || type != NVM3_OBJECTTYPE_DATA) {
//    printf("Key does not contain data object\r\n");
    return;
  }

  err = nvm3_readData(NVM3_DEFAULT_HANDLE, key, buffer_max, len);
  if (ECODE_NVM3_OK == err) {
    buffer_max[len] = '\0';
    memcpy(read_max, buffer_max, strlen(buffer_max)+1);
    memcpy(calibration, buffer_max, strlen(buffer_max)+1);

  } else {
      ;
  }
}

/***************************************************************************//**
 * Display NVM3 data.
 *
 * This function implements the CLI command 'display' (see nvm3_app.slcp)
 * It displays:
 *     - keys of deleted objects
 *     - keys and contents of saved objects
 *     - number of objects deleted since last display
 *     - number of objects written since last display
 ******************************************************************************/
/*
void nvm3_app_display(sl_cli_command_arg_t *arguments)
{
  nvm3_ObjectKey_t keys[MAX_OBJECT_COUNT];
  size_t len, objects_count;
  uint32_t type;
  Ecode_t err;
  uint32_t counter = 0;
  size_t i;

  (void)&arguments;

  objects_count = nvm3_enumDeletedObjects(NVM3_DEFAULT_HANDLE,
                                          (uint32_t *)keys,
                                          sizeof(keys) / sizeof(keys[0]),
                                          MIN_DATA_KEY,
                                          MAX_DATA_KEY);
  if (objects_count == 0) {
    printf("No deleted objects found\r\n");
  } else {
    printf("Keys of objects deleted from NVM3:\r\n");
    for (i = 0; i < objects_count; i++) {
      printf("> %lu\r\n", keys[i]);
    }
  }

  // Retrieve the keys of stored data
  objects_count = nvm3_enumObjects(NVM3_DEFAULT_HANDLE,
                                   (uint32_t *)keys,
                                   sizeof(keys) / sizeof(keys[0]),
                                   MIN_DATA_KEY,
                                   MAX_DATA_KEY);

  if (objects_count == 0) {
    printf("No stored objects found\r\n");
  } else {
    printf("Keys and contents of objects stored in NVM3:\r\n");
    for (i = 0; i < objects_count; i++) {
      nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, keys[i], &type, &len);
      if (type == NVM3_OBJECTTYPE_DATA) {
        err = nvm3_readData(NVM3_DEFAULT_HANDLE, keys[i], buffer, len);
        EFM_ASSERT(ECODE_NVM3_OK == err);
        buffer[len] = '\0';
        printf("> %lu: %s\r\n", keys[i], buffer);
      }
    }
  }

  // Display and reset counters
  err = nvm3_readCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, &counter);
  if (ECODE_NVM3_OK == err) {
    printf("%lu objects have been deleted since last display\r\n", counter);
  }
  nvm3_writeCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, 0);
  err = nvm3_readCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, &counter);
  if (ECODE_NVM3_OK == err) {
    printf("%lu objects have been written since last display\r\n", counter);
  }
  nvm3_writeCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, 0);
}
*/

/***************************************************************************//**
 * Delete all data in NVM3.
 *
 * This function implements the CLI command 'reset' (see nvm3_app.slcp)
 * It deletes all data stored in NVM3.s
 ******************************************************************************/
/*
void nvm3_app_reset(sl_cli_command_arg_t *arguments)
{
  (void)&arguments;
  printf("Deleting all data stored in NVM3\r\n");
  nvm3_eraseAll(NVM3_DEFAULT_HANDLE);
  // This deletes the counters, too, so they must be re-initialised
  initialise_counters();
}
*/

/***************************************************************************//**
 * Initialize NVM3 example.
 ******************************************************************************/
void nvm3_app_init(void)
{
  Ecode_t err;

  // This will call nvm3_open() with default parameters for
  // memory base address and size, cache size, etc.
  err = nvm3_initDefault();
  EFM_ASSERT(err == ECODE_NVM3_OK);

  // Initialise the counter objects to track writes and deletes.
  initialise_counters();

}

/***************************************************************************//**
 * NVM3 ticking function.
 ******************************************************************************/
uint32_t battery_max_key = 1;
uint32_t die_temp_max_key = 2;
uint32_t color_variant_key = 3;
uint32_t Null_current_key = 4;
uint32_t interzone_key = 5;
uint32_t trigger_voltage_key = 6;
uint32_t data_runs_key = 7;
uint32_t peak_liq_temp_key = 8;
uint32_t peak_ware_temp_key = 9;
uint32_t peak_DC_volts_key = 10;
uint32_t peak_DC_current_key = 11;
uint32_t peak_AC_volts_key = 12;
uint32_t vXsec_key = 13;
uint32_t peak_sec_key = 14; // Peak Sec > 8V
uint32_t vdd_key = 15;
uint32_t calibration_key = 16;

uint32_t air_key = 17;
uint32_t dc_key = 18;
uint32_t ware_temp_key = 19;
uint32_t cdc_key = 20;
uint32_t vac_key = 21;
uint32_t peak_count_key = 22;
uint32_t cdc_max_key = 23;
uint32_t alit_threshold_key = 105;

volatile uint32_t key_shift = 0;
extern bool InterzoneTimeout;
extern bool stop_log_low_battery;

bool log1 = false;  // 5 min, keys_shift = 6, data_size = 600*7+8 = 368
bool log2 = false;  // 10 min, keys_shift = 15
bool log3 = false;  // 15 min, keys_shift = 21
bool log4 = false;  // 20 min, keys_shift = 27
bool log5 = false;  // 25 min, keys_shift = 33
bool log6 = false;  // 30 min, keys_shift = 39
bool log7 = false;  // 35 min, keys_shift = 45
bool log8 = false;  // 40 min, keys_shift = 51
bool log9 = false;  // 45 min, keys_shift = 57
bool log10 = false; // 50 min, keys_shift = 63
bool log11 = false; // 55 min, keys_shift = 69
bool log12 = false; // 60 min, keys_shift = 75,total data_size= 368+360*11=4328 ~ 5kB

extern uint16_t log_count;
extern bool log_continue;
extern bool arm;
extern bool mem_test;

extern bool lossOfLightTrigger;
extern bool voltageTrigger;
extern bool trigger_set;

extern uint16_t air_mon;
static uint16_t air_mon_Buffer[rbuffer_size] = {0};

extern uint16_t dc_volt_mon;
static uint16_t dc_volt_mon_Buffer[rbuffer_size] = {0};

extern uint16_t ware_temp_mon;
static uint16_t ware_temp_mon_Buffer[rbuffer_size] = {0};

//extern uint16_t cdc_mon;
static int16_t cdc_mon_Buffer[rbuffer_size] = {0};

uint16_t battery_log;
extern float battery_left;

//extern uint16_t vac_mon;
static uint16_t vac_mon_Buffer[rbuffer_size] = {0};

extern uint16_t die_temp;
//static uint16_t die_temp_Buffer[rbuffer_size];

extern uint16_t peak_counts;
static uint16_t peak_counts_Buffer[rbuffer_size] = {0};

//extern uint16_t cdc_min;
//static int16_t cdc_min_Buffer[rbuffer_size];

//extern uint16_t cdc_max;
static int16_t cdc_max_Buffer[rbuffer_size] = {0};
//static uint16_t cdc_max_Buffer[rbuffer_size];

extern uint16_t battery_max;
extern uint16_t die_temp_max;

extern uint16_t cdc_null;
extern bool log_flag;
extern bool timeoutFlag;
extern bool output_toggle;
extern uint8_t output_tog;

extern st_sensor st_air_temp;
extern st_sensor st_dc_volt;
extern st_sensor st_ware_temp;
extern st_sensor st_cdc;
//extern st_sensor st_cdc_min;
extern st_sensor st_cdc_max;
//extern st_sensor st_die_temp;
extern st_sensor st_vac_mon;
extern st_sensor timeout;
extern sl_adc_val sl_vac_val;
extern sl_adc_val sl_cdc_val;
extern sl_adc_val sl_dc_volt_val;
extern uint16_t count_sec;
extern uint16_t log_timer;
//extern uint16_t log_count;
extern uint16_t threshold_count;
extern volatile float DC_volt;
extern volatile uint8_t vol_trigger;
uint16_t read_Buffer_[rbuffer_size] = {0};
int16_t readSigned_Buffer[rbuffer_size] = {0};
//extern uint16_t data_runs;
uint16_t dataRun_x = 0;
uint32_t dataComp = 0;
char str[8] = "";
char longStr[8] = "";
uint32_t longdDataComp = 0;
uint32_t longData = 0;
uint16_t time_sec = 0;
bool time_sec_flag = false;

int peakMax(int16_t* arr)
{
  volatile int length = sizeof(read_Buffer_)/sizeof(read_Buffer_[0]);
  volatile int max = read_Buffer_[0];
  for (int i = 0; i < length; i++) {
      //Compare elements of array with max
     if(read_Buffer_[i] > max){
         max = read_Buffer_[i];
         if(time_sec_flag){
         time_sec = i + time_sec;
         }
     }
  }
//    printf("Largest element present in given array: %d\n", max);
    return max;
}

volatile uint16_t max_val = 0;
void logintoflash(){
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  nvm3_app_delete(air_key + key_shift);
  nvm3_app_write(air_mon_Buffer, rbuffer_size, air_key + key_shift);
  nvm3_app_read(read_Buffer_, air_key + key_shift);
  max_val = peakMax(read_Buffer_); // gives maximum air temp
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  memset(air_mon_Buffer, 0, sizeof(air_mon_Buffer));
  // Save the peak liquid diagnostic data
  nvm3_app_read_max(peak_liq_temp_key);
  dataComp = atoi(read_max);
  if(max_val > dataComp){
      sprintf(str, "%d", max_val);
      nvm3_app_write_max(str, peak_liq_temp_key);
      nvm3_app_read_max(peak_liq_temp_key);
      memset(str, 0, sizeof(str));
  }

  nvm3_app_delete(dc_key + key_shift);
  nvm3_app_write(dc_volt_mon_Buffer, rbuffer_size, dc_key + key_shift);
  nvm3_app_read(read_Buffer_, dc_key + key_shift);
  time_sec_flag = true;
  max_val = peakMax(read_Buffer_); // gives maximum air temp
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  // Save the peak DC Volt diagnostic data
  nvm3_app_read_max(peak_DC_volts_key);
  dataComp = atoi(read_max);
  if(max_val > dataComp){
      sprintf(str, "%d", max_val);
      nvm3_app_write_max(str, peak_DC_volts_key);
      nvm3_app_read_max(peak_DC_volts_key);
      memset(str, 0, sizeof(str));
  }
  // Save the peak VxSec diagnostic data
  nvm3_app_read_max(vXsec_key);
  longdDataComp = atoi(read_max);
  longData = max_val * time_sec; //
  if( longData > longdDataComp){
      sprintf(str, "%d", longData);
      nvm3_app_write_max(str, vXsec_key);
      nvm3_app_read_max(vXsec_key);
      memset(str, 0, sizeof(str));
  }
  time_sec_flag = false;

//  volatile size_t len2 = (sizeof(ware_temp_mon_Buffer));
  nvm3_app_delete(ware_temp_key + key_shift);
  nvm3_app_write(ware_temp_mon_Buffer, rbuffer_size, ware_temp_key + key_shift);
  nvm3_app_read(read_Buffer_, ware_temp_key + key_shift);
  max_val = peakMax(read_Buffer_); // gives maximum air temp
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  // Save the peak ware diagnostic data
  nvm3_app_read_max(peak_ware_temp_key);
  dataComp = atoi(read_max);
  if(max_val > dataComp){
      sprintf(str, "%d", max_val);
      nvm3_app_write_max(str, peak_ware_temp_key);
      nvm3_app_read_max(peak_ware_temp_key);
      memset(str, 0, sizeof(str));
  }

//  volatile size_t len3 = (sizeof(cdc_mon_Buffer));
  nvm3_app_delete(cdc_key + key_shift);
  nvm3_app_write(cdc_mon_Buffer, rbuffer_size, cdc_key + key_shift);
  nvm3_app_read(readSigned_Buffer, cdc_key + key_shift);
//  m = peakMax(read_Buffer_); // gives maximum CDC
  memset(readSigned_Buffer, 0, sizeof(readSigned_Buffer));

//  nvm3_app_write(cdc_min_Buffer, rbuffer_size, cdc_min_key + key_shift);
//  nvm3_app_read(read_Buffer_, cdc_min_key + key_shift);
//  memset(read_Buffer_, 0, sizeof(read_Buffer_));


  nvm3_app_delete(cdc_max_key + key_shift); // total - 23+77 when 60 minutes
  nvm3_app_write(cdc_max_Buffer, rbuffer_size, cdc_max_key + key_shift);
  nvm3_app_read(readSigned_Buffer, cdc_max_key + key_shift);
  max_val = peakMax(readSigned_Buffer); // gives maximum CDC
  memset(readSigned_Buffer, 0, sizeof(readSigned_Buffer));
  // Save the  peak_DC_current diagnostic data
  nvm3_app_read_max(peak_DC_current_key);
  dataComp = atoi(read_max);
  if(max_val > dataComp){
      sprintf(str, "%d", max_val);
      nvm3_app_write_max(str, peak_DC_current_key);
      nvm3_app_read_max(peak_DC_current_key);
      memset(str, 0, sizeof(str));
  }

  nvm3_app_delete(vac_key + key_shift);
  nvm3_app_write(vac_mon_Buffer, rbuffer_size, vac_key + key_shift);
  nvm3_app_read(read_Buffer_, vac_key + key_shift);
  max_val = peakMax(read_Buffer_); // gives maximum air temp
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  // Save the  peak_AC_volts diagnostic data
  nvm3_app_read_max(peak_AC_volts_key);
  dataComp = atoi(read_max);
  if(max_val > dataComp){
      sprintf(str, "%d", max_val);
      nvm3_app_write_max(str, peak_AC_volts_key);
      nvm3_app_read_max(peak_AC_volts_key);
      memset(str, 0, sizeof(str));
  }

  nvm3_app_delete(peak_count_key + key_shift);
  nvm3_app_read(read_Buffer_, peak_count_key + key_shift);
  nvm3_app_write(peak_counts_Buffer, rbuffer_size, peak_count_key + key_shift);
  nvm3_app_read(read_Buffer_, peak_count_key + key_shift);
  max_val = peakMax(read_Buffer_); // gives maximum air temp
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
  // Save the  peak_AC_volts diagnostic data
  nvm3_app_read_max(peak_sec_key);
  dataComp = atoi(read_max);
  dataComp = dataComp + max_val;
  sprintf(str, "%d", dataComp);
  nvm3_app_write_max(str, peak_sec_key);
  nvm3_app_read_max(peak_sec_key);
  memset(str, 0, sizeof(str));

  memset(air_mon_Buffer, 0, sizeof(air_mon_Buffer));
  memset(dc_volt_mon_Buffer, 0, sizeof(dc_volt_mon_Buffer));
  memset(ware_temp_mon_Buffer, 0, sizeof(ware_temp_mon_Buffer));
  memset(cdc_mon_Buffer, 0, sizeof(cdc_mon_Buffer));
//  memset(cdc_min_Buffer, 0, sizeof(cdc_min_Buffer));
  memset(cdc_max_Buffer, 0, sizeof(cdc_max_Buffer));
  memset(vac_mon_Buffer, 0, sizeof(vac_mon_Buffer));
  memset(peak_counts_Buffer, 0, sizeof(peak_counts_Buffer));

  log_continue = true;
  log_count = log_count + 1;
  key_shift =  key_shift + 7;

    if (key_shift == 84){
        key_shift = 0;
        count_sec = 0;
        timeoutFlag = false;
        log_flag = false;
        output_toggle = 1;
        output_tog = 1;
        lossOfLightTrigger = 0;
        trigger_set = false;
        voltageTrigger = 0;
        peak_counts = 0;
    //    arm = false;
        lossOfLightTrigger = 0;
        voltageTrigger = 0;
//        trigger_set = 0;

        nvm3_app_read_max(data_runs_key);
        dataRun_x = atoi(read_max);
        dataRun_x ++;
        sprintf(str, "%d", dataRun_x);
        nvm3_app_write_max(str, data_runs_key);
        nvm3_app_read_max(data_runs_key);
        memset(str, 0, sizeof(str));

        // For the next run, these three values should be zero
        battery_log = 0;
        battery_max = 0;
        die_temp = 0;
        die_temp_max = 0;
        arm = true;
//        mem_test = false;
        log_timer++;
    }
}

//char str[4] = "";
//int change_x=0;

//extern uint16_t data_counter;
uint16_t previous_count = 0;
uint16_t new_count = 0;
void nvm3_app_process_action(void)
{
//output_tog = 0; // changed on feb
/*
  nvm3_app_read(read_Buffer_, peak_count_key + key_shift);
  key_shift =  key_shift + 7;
  memset(read_Buffer_, 0, sizeof(read_Buffer_));
*/
if(mem_test){
  log_timer++;
  battery_log = battery_left*1000;
}

if (st_air_temp.log_sensor_data){
  air_mon_Buffer[count_sec] = air_mon;
//  air_mon_Buffer[count_sec] = air_avg;
  st_air_temp.log_sensor_data = 0;
}

if(st_dc_volt.log_sensor_data){
    dc_volt_mon_Buffer[count_sec] = sl_dc_volt_val.avg;
//  dc_volt_mon_Buffer[count_sec] = dc_volt_mon;
    st_dc_volt.log_sensor_data = 0;
}

if(st_ware_temp.log_sensor_data){
  ware_temp_mon_Buffer[count_sec] = ware_temp_mon;
  st_ware_temp.log_sensor_data = 0;
}

if(st_cdc.log_sensor_data){
//    cdc_mon_Buffer[count_sec] = sl_cdc_val.avg;
  cdc_mon_Buffer[count_sec] = (sl_cdc_val.avg) - cdc_null;
//  if(cdc_mon_Buffer[count_sec] < 0){
//      cdc_mon_Buffer[count_sec] = cdc_mon_Buffer[count_sec] * (-1);
//  }
  st_cdc.log_sensor_data = 0;
}

//if(st_cdc_min.log_sensor_data && count_sec > 2){
////  cdc_mon_Buffer[count_sec] = cdc_mon;
//  cdc_min_Buffer[count_sec] = sl_cdc_val.min;
//  st_cdc_min.log_sensor_data = 0;
//}

if(st_cdc_max.log_sensor_data){
//    cdc_max_Buffer[count_sec] = sl_cdc_val.max;
  cdc_max_Buffer[count_sec] = (sl_cdc_val.max) - cdc_null;
//  if(cdc_max_Buffer[count_sec] < 0){
//      cdc_max_Buffer[count_sec] = cdc_max_Buffer[count_sec] * (-1);
//  }
  st_cdc_max.log_sensor_data = 0;
}

//if(st_die_temp.log_sensor_data){
//  die_temp_Buffer[count_sec] = die_temp;
//  st_die_temp.log_sensor_data = 0;
//}

if(DC_volt >= vol_trigger && mem_test){
  if(count_sec != previous_count){
  peak_counts ++;
  peak_counts_Buffer[count_sec] = peak_counts;
  previous_count = count_sec;
  }
//  else{;}
}
//data_counter++;
//peak_counts_Buffer[count_sec] = data_counter;

if(st_vac_mon.log_sensor_data){
//  vac_mon_Buffer[count_sec] = vac_mon;
  vac_mon_Buffer[count_sec] = sl_vac_val.avg;
  st_vac_mon.log_sensor_data = 0;
}

if (battery_log > battery_max)
{
    battery_max = battery_log;
    sprintf(str, "%d", battery_max);
    nvm3_app_write_max(str, battery_max_key);
//    nvm3_app_read_max(battery_max_key);
    memset(str, 0, sizeof(str));
}

if (die_temp > die_temp_max)
{
    die_temp_max = die_temp;
    sprintf(str, "%d", die_temp_max);
    nvm3_app_write_max(str, die_temp_max_key);
//    nvm3_app_read_max(battery_max_key);
    memset(str, 0, sizeof(str));

}

// memory out of bound exception, after that data is saved and new process will be awaited
//if(count_sec >= threshold_count)
if( ((count_sec == threshold_count) && !log1) ||
    ((count_sec == threshold_count) && !log2) ||
    ((count_sec == threshold_count) && !log3) ||
    ((count_sec == threshold_count) && !log4) ||
    ((count_sec == threshold_count) && !log5) ||
    ((count_sec == threshold_count) && !log6) ||
    ((count_sec == threshold_count) && !log7) ||
    ((count_sec == threshold_count) && !log8) ||
    ((count_sec == threshold_count) && !log9) ||
    ((count_sec == threshold_count) && !log10) ||
    ((count_sec == threshold_count) && !log11) ||
    ((count_sec == threshold_count) && !log12)
    )
{
// write the data in nvm buffer
    logintoflash();

    switch(log_count) {
      case 1: log1 = true;
        break;
      case 2: log2 = true;
        break;
      case 3: log3 = true;
        break;
      case 4: log4 = true;
        break;
      case 5: log5 = true;
        break;
      case 6: log6 = true;
        break;
      case 7: log7 = true;
        break;
      case 8: log8 = true;
        break;
      case 9: log9 = true;
        break;
      case 10: log10 = true;
        break;
      case 11: log11 = true;
        break;
      case 12: log12 = true;

        break;
      default: ;
    }


// For the memory exception error if logs greater than 1200 bytes i.e 600 entries
if(log_timer > 3599){
    mem_test = false;
}

    timeoutFlag = false;
    log_flag = false;

//    count_sec = 0;
    // For the next run, these three values should be zero
    battery_log = 0;
    battery_max = 0;
    die_temp = 0;
    die_temp_max = 0;
    arm = true;
    count_sec = 0;
//    mem_test = false;
    // flash the data run
//    log_count++;
}

if(mem_test && (DC_volt < vol_trigger) && !(timeout.log_sensor_data) ){
//  nvm3_app_delete(peak_count_key);
  logintoflash();

  switch(log_count) {
       case 1: log1 = true;
         break;
       case 2: log2 = true;
         break;
       case 3: log3 = true;
         break;
       case 4: log4 = true;
         break;
       case 5: log5 = true;
         break;
       case 6: log6 = true;
         break;
       case 7: log7 = true;
         break;
       case 8: log8 = true;
         break;
       case 9: log9 = true;
         break;
       case 10: log10 = true;
         break;
       case 11: log11 = true;
         break;
       case 12: log12 = true;

         break;
       default: ;
     }


  peak_counts = 0;
//  arm = false;
  lossOfLightTrigger = 0;
  voltageTrigger = 0;
//  trigger_set = false;
  timeoutFlag = false;
  log_flag = false;
  output_toggle =1;
  output_tog = 1;

  // For the next run, these three values should be zero
  battery_log = 0;
  battery_max = 0;
  die_temp = 0;
  die_temp_max = 0;
  arm = true;

  // save the data run
  nvm3_app_read_max(data_runs_key);
  int dataRun_x = atoi(read_max);
  dataRun_x ++;
  sprintf(str, "%d", dataRun_x);
  nvm3_app_write_max(str, data_runs_key);
  nvm3_app_read_max(data_runs_key);
  memset(str, 0, sizeof(str));
  count_sec = 0;
  InterzoneTimeout = true;
}

// Check if NVM3 controller can release any out-of-date objects
// to free up memory.
// This may take more than one call to nvm3_repack()
//  printf("nvm3_repackNeeded:%d\n", nvm3_repackNeeded(NVM3_DEFAULT_HANDLE));
  while (nvm3_repackNeeded(NVM3_DEFAULT_HANDLE)) {
//  printf("Repacking NVM...\r\n");
  nvm3_repack(NVM3_DEFAULT_HANDLE);
  }
}

void save_in_low_voltage(){
  logintoflash();

  switch(log_count) {
       case 1: log1 = true;
         break;
       case 2: log2 = true;
         break;
       case 3: log3 = true;
         break;
       case 4: log4 = true;
         break;
       case 5: log5 = true;
         break;
       case 6: log6 = true;
         break;
       case 7: log7 = true;
         break;
       case 8: log8 = true;
         break;
       case 9: log9 = true;
         break;
       case 10: log10 = true;
         break;
       case 11: log11 = true;
         break;
       case 12: log12 = true;

         break;
       default: ;
     }

  peak_counts = 0;
//  arm = false;
  lossOfLightTrigger = 0;
  voltageTrigger = 0;
//  trigger_set = false;
  timeoutFlag = false;
  log_flag = false;
  output_toggle =1;
  output_tog = 1;

  // For the next run, these three values should be zero
  battery_log = 0;
  battery_max = 0;
  die_temp = 0;
  die_temp_max = 0;
  arm = true;

  // save the data run
  nvm3_app_read_max(data_runs_key);
  int dataRun_x = atoi(read_max);
  dataRun_x ++;
  sprintf(str, "%d", dataRun_x);
  nvm3_app_write_max(str, data_runs_key);
  nvm3_app_read_max(data_runs_key);
  memset(str, 0, sizeof(str));
  count_sec = 0;
  stop_log_low_battery = true;

}

