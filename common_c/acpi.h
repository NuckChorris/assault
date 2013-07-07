#pragma once

#include <stdbool.h>

#define ACPI_PATH "/sys/class/power_supply/%s/%s"

bool acpi_get_property (char *out, char *battery_id, char *property);
bool acpi_get_charging (char *battery_id);
double acpi_get_percent (char *battery_id);
