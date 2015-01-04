rawScreenConvert
================

Converts raw buffer screen data from android command "screencap" to bitmap image.

The raw buffer format is really simple:
 Offset | Size(B) |    Description
 
--------+---------+--------------------

 0x00   |    4    |  Image width  (w)
 
 0x04   |    4    |  Image height (h)
 
 0x08   |    4    | Unknown / Useless?
 
 0x0C   |  4*w*h  |    Pixels array
 

The pixels are stored with the format RGBa, but the alpha value is always 0xFF, so it can be skipped.
