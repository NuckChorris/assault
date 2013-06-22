#include <stdio.h>
#include <string.h>
#include "acpi.h"

gboolean acpi_get_charging
()
{
	FILE *fp;
	char b[9];

	// Look for the status file
	fp = fopen("/sys/class/power_supply/BAT0/status", "r");
	if (fp != NULL) {
		if (fgets(b, 9, fp)) {
			fclose(fp);

			if (strcmp(b, "Charging") == 0) {
				return TRUE;
			} else {
				return FALSE;
			}
		}
	}

	// If we're here, we don't know what's happening, so return FALSE
	return FALSE;
}

gdouble acpi_get_percent
()
{
	FILE *fp_full;
	FILE *fp_now;
	unsigned int full;
	unsigned int now;

	// Check for the charge_{full,now} entries
	fp_full = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
	if (fp_full != NULL) {
		if (fscanf(fp_full, "%d", &full) == 1) {
			// Looks like we have what we want.
			fp_now = fopen("/sys/class/power_supply/BAT0/charge_now", "r");
			fscanf(fp_now, "%d", &now);

			fclose(fp_full);
			fclose(fp_now);

			return ((double)now / (double)full);
		}
	}

	// Check for the energy_{full,now} entries
	fp_full = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
	if (fp_full != NULL) {
		if (fscanf(fp_full, "%d", &full) == 1) {
			// Looks like we have what we want.
			fp_now = fopen("/sys/class/power_supply/BAT0/energy_now", "r");
			fscanf(fp_now, "%d", &now);

			fclose(fp_full);
			fclose(fp_now);

			return ((double)now / (double)full);
		}
	}
	
	// If we're here, we don't have anything, so return 0
	return 0.0;
}

// vim: set noexpandtab
