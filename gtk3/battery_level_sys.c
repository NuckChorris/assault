#include <stdio.h>
#include "main.h"
#include "utils.h"
#include "battery_level_sys.h"

gboolean battery_get_charging_from_sys
()
{
	FILE *fp;
	char b[9];

	// Look for the status file
	fp = fopen("/sys/class/power_supply/BAT0/status", "r");
	if (fp != NULL) {
		if (fgets(b, 9, fp)) {
			fclose(fp);

			printf("[battery_get_charging_from_sys] status: %s\n", b);
			if (strcmp(b, "Charging") == 0) {
				printf("[battery_get_charging_from_sys] returning TRUE\n");
				return TRUE;
			} else {
				printf("[battery_get_charging_from_sys] returning FALSE\n");
				return FALSE;
			}
		}
	}

	// If we're here, we don't know what's happening, so return FALSE
	return FALSE;
}

gdouble battery_get_percent_from_sys
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

			printf("[battery_get_percent_from_sys] charge_{full,now} level: %d / %d\n", now, full);
			printf("[battery_get_percent_from_sys] percentage: %f\n", (((double)now / (double)full) * 100.0));
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

			printf("[battery_get_percent_from_sys] energy_{full,now} level: %d / %d\n", now, full);
			printf("[battery_get_percent_from_sys] percentage: %f\n", (((double)now / (double)full) * 100.0));
			return ((double)now / (double)full);
		}
	}
	
	// If we're here, we don't have anything, so return 0
	printf("[battery_get_percent_from_sys] unknown battery level\n");
	return 0.0;
}

// vim: set noexpandtab
