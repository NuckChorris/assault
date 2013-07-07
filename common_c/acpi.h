#include <gtk/gtk.h>

#define ACPI_PATH "/sys/class/power_supply/%s/%s"

gboolean acpi_get_property (gchar *out, gchar *battery_id, gchar *property);
gboolean acpi_get_charging (gchar *battery_id);
gdouble acpi_get_percent (gchar *battery_id);
