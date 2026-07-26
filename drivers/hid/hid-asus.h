/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  HID driver for Asus ROG Ally handheld gaming console and shared driver data.
 *
 *  Copyright (c) 2023-2026 Luke Jones <luke@ljones.dev>
 *  Copyright (c) 2024-2026 Denis Benato <denis.benato@linux.dev>
 */

#ifndef __HID_ASUS_H
#define __HID_ASUS_H

#include <linux/cleanup.h>
#include <linux/device.h>
#include <linux/hid.h>
#include <linux/input.h>
#include <linux/platform_data/x86/asus-wmi.h>
#include <linux/power_supply.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#define FEATURE_REPORT_ID 0x0d
#define INPUT_REPORT_ID 0x5d
#define FEATURE_KBD_REPORT_ID 0x5a
#define FEATURE_KBD_LED_REPORT_ID1 0x5d
#define FEATURE_KBD_LED_REPORT_ID2 0x5e
#define FEATURE_KBD_REPORT_SIZE 64

#define ROG_ALLY_REPORT_SIZE 64
#define ROG_ALLY_X_MIN_MCU 313
#define ROG_ALLY_MIN_MCU 319

#define HID_ALLY_INTF_KEYBOARD_IN 0x81
#define HID_ALLY_INTF_CFG_IN 0x83
#define HID_ALLY_X_INTF_IN 0x87

#define HID_ALLY_GET_REPORT_ID 0x0D
#define HID_ALLY_SET_REPORT_ID 0x5A
#define HID_ALLY_FEATURE_CODE_PAGE 0xD1

#define HID_ALLY_X_INPUT_REPORT_SIZE 16
#define HID_ALLY_X_INPUT_REPORT 0x0B

#define HID_ALLY_READY_MAX_TRIES 6

struct asus_kbd_leds {
	struct asus_hid_listener listener;
	struct hid_device *hdev;
	struct work_struct work;
	unsigned int brightness;
	spinlock_t lock;
	bool removed;
};

struct asus_touchpad_info {
	int max_x;
	int max_y;
	int res_x;
	int res_y;
	int contact_size;
	int max_contacts;
	int report_size;
};

struct ally_handheld;

struct asus_drvdata {
	unsigned long quirks;
	struct hid_device *hdev;
	struct input_dev *input;
	struct input_dev *tp_kbd_input;
	struct asus_kbd_leds *kbd_backlight;
	struct ally_handheld *rog_ally;
	const struct asus_touchpad_info *tp;
	struct power_supply *battery;
	struct power_supply_desc battery_desc;
	int battery_capacity;
	int battery_stat;
	bool battery_in_query;
	unsigned long battery_next_query;
	struct work_struct fn_lock_sync_work;
	bool fn_lock;
};

struct ally_handheld *hid_asus_ally_probe(struct hid_device *hdev);
void hid_asus_ally_remove(struct hid_device *hdev, struct ally_handheld *ally);
bool hid_asus_ally_raw_event(struct hid_device *hdev, struct ally_handheld *ally,
			     struct hid_report *report, u8 *data, int size);
int hid_asus_ally_reset_resume(struct hid_device *hdev, struct ally_handheld *ally);
void validate_mcu_fw_version(struct hid_device *hdev, int idProduct);

#endif /* __HID_ASUS_H */
