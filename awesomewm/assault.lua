local setmetatable = setmetatable
local ipairs = ipairs
local math = math
local base = require('wibox.widget.base')
local color = require("gears.color")
local cairo = require("lgi").cairo

local assault = { mt = {} }

local data = setmetatable({}, { __mode = "k" })

local properties = { "width", "height" }

function assault.fit (assault, width, height)
	local width = math.min(width, data[assault].width)
	local height = math.min(height, data[assault].height)
	return width, height
end

local battery_bolt_generate = function (width, height)
   local surface = cairo.ImageSurface(cairo.Format.A8, width, height)
   local cr = cairo.Context(surface)
	
	cr:new_path()

	cr:move_to(width * ( 0.0/19), height * ( 3.0/11))
	cr:line_to(width * (11.0/19), height * (11.0/11))
	cr:line_to(width * (11.0/19), height * ( 5.5/11))
	cr:line_to(width * (19.0/19), height * ( 8.0/11))
	cr:line_to(width * ( 8.0/19), height * ( 0.0/11))
	cr:line_to(width * ( 8.0/19), height * ( 5.5/11))

	cr:close_path()

	return cr:copy_path()
end

local battery_border_generate = function (args)
	local args = args or {}
	local surface = cairo.ImageSurface(cairo.Format.A8, args.width, args.height)
	local cr = cairo.Context(surface)

	local outside_width  = args.width  + args.stroke_width * 2;
	local outside_height = args.height + args.stroke_width * 2;

	cr:new_path()

	cr:move_to(0                             , 0                             )
	cr:line_to(outside_width                 , 0                             )
	cr:line_to(outside_width                 , args.peg_top                  )
	cr:line_to(outside_width + args.peg_width, args.peg_top                  )
	cr:line_to(outside_width + args.peg_width, args.peg_top + args.peg_height)
	cr:line_to(outside_width                 , args.peg_top + args.peg_height)
	cr:line_to(outside_width                 , outside_height                )
	cr:line_to(0                             , outside_height                )

	cr:rectangle(args.stroke_width, args.stroke_width, args.width, args.height);

	cr:close_path()

	return cr:copy_path()

end

local properties = {
	"width", "height", "border_color", "peg_top", "peg_height", "peg_width",
	"stroke_width"
}

function assault.draw (assault, wibox, cr, width, height)
	cr:translate(2, 2)
	cr:append_path(battery_border_generate({
		width = 24,
		height = 12,
		stroke_width = 2,
		peg_top = 6,
		peg_height = 4,
		peg_width = 2
	}))
	cr.fill_rule = "EVEN_ODD"
--	cr:append_path(battery_bolt_generate(width, height))
	cr:fill()
end

-- Build properties function
for _, prop in ipairs(properties) do
    if not assault["set_" .. prop] then
        assault["set_" .. prop] = function(widget, value)
            data[widget][prop] = value
            widget:emit_signal("widget::updated")
            return widget
        end
    end
end

--- Create an assault widget
function assault.new (args)
	local args = args or {}
	local width = args.width or 36
	local height = args.height or 15

	args.type = "imagebox"

	local widget = base.make_widget()

	data[widget] = {
		width = width,
		height = height
	}

	-- Set methods
	for _, prop in ipairs(properties) do
		widget["set_" .. prop] = assault["set_" .. prop]
	end

	widget.draw = assault.draw
	widget.fit = assault.fit

	return widget
end

function assault.mt:__call(...)
    return assault.new(...)
end


return setmetatable(assault, assault.mt)
