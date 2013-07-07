#include <stdio.h>
#include <string.h>
#include "acpi.h"

bool acpi_get_property
(char *out, char *battery_id, char *property)
{
	FILE *fp;
	char filename[64];

	snprintf(filename, 64, ACPI_PATH, battery_id, property);
	fp = fopen(filename, "r");
	if (fp != NULL && fgets(out, 64, fp)) {
		fclose(fp);
		return true;
	}
	return false;
}

bool acpi_get_charging
(char *battery_id)
{
	char value[64];

	if (acpi_get_property(value, battery_id, "status")) {
		if (strcmp(value, "Charging") == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

double acpi_get_percent
(char *battery_id)
{
	char val_full[64], val_now[64];
	unsigned int full;
	unsigned int now;

	if (acpi_get_property(val_full, battery_id, "charge_full")
	 && sscanf(val_full, "%d", &full) == 1) {
		if (acpi_get_property(val_now, battery_id, "charge_now")
		 && sscanf(val_now, "%d", &now)) {
			return ((double)now / (double)full);
		}
	}

	if (acpi_get_property(val_full, battery_id, "energy_full")
	 && sscanf(val_full, "%d", &full) == 1) {
		if (acpi_get_property(val_now, battery_id, "energy_now")
		 && sscanf(val_now, "%d", &now)) {
			return ((double)now / (double)full);
		}
	}
	
	return 0.0;
}

// vim: set noexpandtab
