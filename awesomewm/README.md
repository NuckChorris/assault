Assault for AwesomeWM
=====================

Installation
------------

1. Copy `assault.lua` into your Awesome config directory

2. Require Assault at the top of your `rc.lua`:

   ```lua
   local assault = require('assault')
   ```

3. Configure it: (see below for a full list of options)

   ```lua
   myassault = assault({
      critical_level = 0.15,
      critical_color = "#ff0000",
      charging_color = "#00ff00"
   })
   ```

4. Add it to a wibox:

   ```lua
   right_layout:add(myassault)
   ```

Configuration options
---------------------

```lua
assault({
   battery = "BAT0", -- battery ID to get data from
   adapter = "AC", -- ID of the AC adapter to get data from
   width = 36, -- width of battery
   height = 15, -- height of battery
   bolt_width = 19, -- width of charging bolt
   bolt_height = 11, -- height of charging bolt
   stroke_width = 2, -- width of battery border
   peg_top = (calculated), -- distance from the top of the battery to the start of the peg
   peg_height = (height / 3), -- height of the peg
   peg_width = 2, -- width of the peg
   font = beautiful.font, -- font to use
   critical_level = 0.10, -- battery percentage to mark as critical (between 0 and 1, default is 10%)
   normal_color = beautiful.fg_normal, -- color to draw the battery when it's discharging
   critical_color = "#ff0000", -- color to draw the battery when it's at critical level
   charging_color = "#00ff00" -- color to draw the battery when it's charging
})
```
