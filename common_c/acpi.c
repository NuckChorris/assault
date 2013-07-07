#include <stdio.h>
#include <string.h>
#include "acpi.h"

gboolean acpi_get_property
(gchar *out, gchar *battery_id, gchar *property)
{
	FILE *fp;
	gchar filename[64];
	snprintf(filename, 64, ACPI_PATH, battery_id, property);
	fp = fopen(filename, "r");
	if (fp != NULL && fgets(out, 64, fp)) {
		fclose(fp);
		return TRUE;
	}
	return FALSE;
}

gboolean acpi_get_charging
(gchar *battery_id)
{
	gchar value[64];
	if (acpi_get_property(value, battery_id, "status")) {
		if (strcmp(value, "Charging") == 0) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

gdouble acpi_get_percent
(gchar *battery_id)
{
	gchar val_full[64], val_now[64];
	guint full;
	guint now;

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
	
	// If we're here, we don't have anything, so return 0
	return 0.0;
}

// vim: set noexpandtab
