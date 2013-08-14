#pragma once

#include <stdbool.h>

#define ACPI_PATH "/sys/class/power_supply/%s/%s"
#define ACPI_AVERAGE_SIZE 10

typedef struct {
	int length;
	double current;
	double items[ACPI_AVERAGE_SIZE];
	int i;
} acpi_average;

bool acpi_average_create ();
double acpi_average_push (acpi_average *avg, double item);

bool acpi_get_property (char *out, char *battery_id, char *property);
bool acpi_get_charging (char *battery_id);
double acpi_get_percent (char *battery_id);
