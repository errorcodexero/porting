do
  -- support functions for both dissectors
  function bit(p)
    return 2 ^ p
  end

  function hasbit(x, p)
    return x % (p + p) >= p       
  end

  -- DSEIO input protocol and its fields
  p_eioin = Proto ("eioin", "Driver Station Enhanced I/O Input")

  local f_eioin_api_version = ProtoField.uint8("eioin.api_version", "API version", base.DEC)
  local f_eioin_fw_version  = ProtoField.uint8("eioin.fw_version", "FW version", base.DEC)
  local f_eioin_analog_0     = ProtoField.uint16("eioin.analog_0", "analog 0", base.DEC)
  local f_eioin_analog_1     = ProtoField.uint16("eioin.analog_1", "analog 1", base.DEC)
  local f_eioin_analog_2     = ProtoField.uint16("eioin.analog_2", "analog 2", base.DEC)
  local f_eioin_analog_3     = ProtoField.uint16("eioin.analog_3", "analog 3", base.DEC)
  local f_eioin_analog_4     = ProtoField.uint16("eioin.analog_4", "analog 4", base.DEC)
  local f_eioin_analog_5     = ProtoField.uint16("eioin.analog_5", "analog 5", base.DEC)
  local f_eioin_analog_6     = ProtoField.uint16("eioin.analog_6", "analog 6", base.DEC)
  local f_eioin_analog_7     = ProtoField.uint16("eioin.analog_7", "analog 7", base.DEC)
  local f_eioin_digital     = ProtoField.uint16("eioin.digital", "digital", base.HEX)
  local f_eioin_accel_0      = ProtoField.uint16("eioin.accel_0", "accel 0", base.DEC)
  local f_eioin_accel_1      = ProtoField.uint16("eioin.accel_1", "accel 1", base.DEC)
  local f_eioin_accel_2      = ProtoField.uint16("eioin.accel_2", "accel 2", base.DEC)
  local f_eioin_quad_0      = ProtoField.uint16("eioin.quad_0", "quad 0", base.DEC)
  local f_eioin_quad_1      = ProtoField.uint16("eioin.quad_1", "quad 1", base.DEC)
  local f_eioin_buttons    = ProtoField.uint8("eioin.buttons", "buttons", base.HEX)
  local f_eioin_slider     = ProtoField.uint8("eioin.slider", "slider", base.DEC)
  local f_eioin_proximity  = ProtoField.uint8("eioin.proximity", "proximity", base.DEC)

  p_eioin.fields = {
    f_eioin_api_version,
    f_eioin_fw_version,
    f_eioin_analog_0,
    f_eioin_analog_1,
    f_eioin_analog_2,
    f_eioin_analog_3,
    f_eioin_analog_4,
    f_eioin_analog_5,
    f_eioin_analog_6,
    f_eioin_analog_7,
    f_eioin_digital,
    f_eioin_accel_0,
    f_eioin_accel_1,
    f_eioin_accel_2,
    f_eioin_quad_0,
    f_eioin_quad_1,
    f_eioin_buttons,
    f_eioin_slider,
    f_eioin_proximity
  }

  function p_eioin.dissector (buf, pkt, root)
    if (buf:len() < 34) then return end
    -- pkt.cols.protocol = p_eioin.name
    local subtree = root:add(p_eioin, buf(0))

    subtree:add(f_eioin_api_version, buf(0, 1))
    subtree:add(f_eioin_fw_version, buf(1, 1))
    subtree:add(f_eioin_analog_0, buf(2, 2))
    subtree:add(f_eioin_analog_1, buf(4, 2))
    subtree:add(f_eioin_analog_2, buf(6, 2))
    subtree:add(f_eioin_analog_3, buf(8, 2))
    subtree:add(f_eioin_analog_4, buf(10, 2))
    subtree:add(f_eioin_analog_5, buf(12, 2))
    subtree:add(f_eioin_analog_6, buf(14, 2))
    subtree:add(f_eioin_analog_7, buf(16, 2))
    subtree:add(f_eioin_digital, buf(18, 2))
    subtree:add(f_eioin_accel_0, buf(20, 2))
    subtree:add(f_eioin_accel_1, buf(22, 2))
    subtree:add(f_eioin_accel_2, buf(24, 2))
    subtree:add(f_eioin_quad_0, buf(26, 2))
    subtree:add(f_eioin_quad_1, buf(28, 2))
    subtree:add(f_eioin_buttons, buf(30, 1))
    subtree:add(f_eioin_slider, buf(31, 1))
    subtree:add(f_eioin_proximity, buf(32, 1))
  end

  -- DSEIO output protocol and its fields
  p_eioout = Proto ("eioout", "Driver Station Enhanced I/O Output")

  local f_eioout_dio        = ProtoField.uint16("eioout.dio", "digital I/O", base.HEX)
  local f_eioout_oe         = ProtoField.uint16("eioout.oe", "output enable", base.HEX)
  local f_eioout_pe         = ProtoField.uint16("eioout.pe", "pullup enable", base.HEX)
  local f_eioout_pwm_cmp_1  = ProtoField.uint16("eioout.pwmcmp1", "PWM Compare 1", base.DEC)
  local f_eioout_pwm_cmp_2  = ProtoField.uint16("eioout.pwmcmp2", "PWM Compare 2", base.DEC)
  local f_eioout_pwm_cmp_3  = ProtoField.uint16("eioout.pwmcmp3", "PWM Compare 3", base.DEC)
  local f_eioout_pwm_cmp_4  = ProtoField.uint16("eioout.pwmcmp4", "PWM Compare 4", base.DEC)
  local f_eioout_pwm_per_1  = ProtoField.uint16("eioout.pwmper1", "PWM period 1", base.DEC)
  local f_eioout_pwm_per_2  = ProtoField.uint16("eioout.pwmper2", "PWM period 2", base.DEC)
  local f_eioout_dac_1      = ProtoField.uint8("eioout.dac1", "DAC out 1", base.DEC)
  local f_eioout_dac_2      = ProtoField.uint8("eioout.dac2", "DAC out 2", base.DEC)
  local f_eioout_leds       = ProtoField.uint8("eioout.leds", "LEDs", base.HEX)
  local f_eioout_enables    = ProtoField.uint8("eioout.enables", "enables", base.HEX)
  local f_eioout_fixed      = ProtoField.uint8("eioout.fixed", "fixed digital out", base.HEX)
  local f_eioout_flags      = ProtoField.uint8("eioout.flags", "flags", base.HEX)

  p_eioout.fields = {
    f_eioout_dio, 
    f_eioout_oe, 
    f_eioout_pe, 
    f_eioout_pwm_cmp_1, 
    f_eioout_pwm_cmp_2, 
    f_eioout_pwm_cmp_3, 
    f_eioout_pwm_cmp_4, 
    f_eioout_pwm_per_1, 
    f_eioout_pwm_per_2, 
    f_eioout_dac_1, 
    f_eioout_dac_2, 
    f_eioout_leds, 
    f_eioout_enables, 
    f_eioout_fixed, 
    f_eioout_flags
  }

  function p_eioout.dissector (buf, pkt, root)
    if (buf:len() < 24) then return end
    -- pkt.cols.protocol = p_eioout.name
    local subtree = root:add(p_eioout, buf(0))

    subtree:add(f_eioout_dio, buf(0, 2))
    subtree:add(f_eioout_oe, buf(2, 2))
    subtree:add(f_eioout_pe, buf(4, 2))
    subtree:add(f_eioout_pwm_cmp_1, buf(6, 2))
    subtree:add(f_eioout_pwm_cmp_2, buf(8, 2))
    subtree:add(f_eioout_pwm_cmp_3, buf(10, 2))
    subtree:add(f_eioout_pwm_cmp_4, buf(12, 2))
    subtree:add(f_eioout_pwm_per_1, buf(14, 2))
    subtree:add(f_eioout_pwm_per_2, buf(16, 2))
    subtree:add(f_eioout_dac_1, buf(18, 1))
    subtree:add(f_eioout_dac_2, buf(19, 1))
    subtree:add(f_eioout_leds, buf(20, 1))
    subtree:add(f_eioout_enables, buf(21, 1))
    -- enables should be broken into subfields
    subtree:add(f_eioout_fixed, buf(22, 1))
    subtree:add(f_eioout_flags, buf(23, 1))
  end

  -- DS-to-Robot protocol and its fields
  p_d2r = Proto ("d2r", "Driver Station To Robot Protocol")

  local f_d2r_packet    = ProtoField.uint16("d2r.packet", "Packet Index", base.DEC)
  local f_d2r_control   = ProtoField.uint8("d2r.command", "Control", base.HEX)
  local f_d2r_input     = ProtoField.uint8("d2r.input", "Digital Input", base.HEX)
  local f_d2r_team      = ProtoField.uint16("d2r.team", "Team Number", base.DEC)
  local f_d2r_alliance  = ProtoField.string("d2r.alliance", "Team Alliance")
  local f_d2r_position  = ProtoField.string("d2r.position", "Team Position")
  local f_d2r_joystick1 = ProtoField.bytes("d2r.joystick1", "Joystick 1")
  local f_d2r_js_btn1   = ProtoField.uint16("d2r.jsbtn1", "Joystick 1 Buttons", base.HEX)
  local f_d2r_joystick2 = ProtoField.bytes("d2r.joystick2", "Joystick 2")
  local f_d2r_js_btn2   = ProtoField.uint16("d2r.jsbtn2", "Joystick 2 Buttons", base.HEX)
  local f_d2r_joystick3 = ProtoField.bytes("d2r.joystick3", "Joystick 3")
  local f_d2r_js_btn3   = ProtoField.uint16("d2r.jsbtn3", "Joystick 3 Buttons", base.HEX)
  local f_d2r_joystick4 = ProtoField.bytes("d2r.joystick4", "Joystick 4")
  local f_d2r_js_btn4   = ProtoField.uint16("d2r.jsbtn4", "Joystick 4 Buttons", base.HEX)
  local f_d2r_analog_1   = ProtoField.uint16("d2r.analog_1", "Analog 1", base.DEC)
  local f_d2r_analog_2   = ProtoField.uint16("d2r.analog_2", "Analog 2", base.DEC)
  local f_d2r_analog_3   = ProtoField.uint16("d2r.analog_3", "Analog 3", base.DEC)
  local f_d2r_analog_4   = ProtoField.uint16("d2r.analog_4", "Analog 4", base.DEC)
  local f_d2r_crio_chk  = ProtoField.uint64("d2r.crio_chk", "cRIO Checksum", base.HEX)
  local f_d2r_fpga_chk_1 = ProtoField.uint32("d2r.fpga_chk_1", "FPGA Checksum 1", base.HEX)
  local f_d2r_fpga_chk_2 = ProtoField.uint32("d2r.fpga_chk_2", "FPGA Checksum 2", base.HEX)
  local f_d2r_fpga_chk_3 = ProtoField.uint32("d2r.fpga_chk_3", "FPGA Checksum 3", base.HEX)
  local f_d2r_fpga_chk_4 = ProtoField.uint32("d2r.fpga_chk_4", "FPGA Checksum 4", base.HEX)
  local f_d2r_version   = ProtoField.string("d2r.version", "Version")
  local f_d2r_ext       = ProtoField.bytes("d2r.ext", "Extension")
  local f_d2r_ext_len	= ProtoField.uint8("d2r.ext.len", "Extension Length", base.DEC)
  local d2r_ext_tags = {
    [17] = "DS Enhanced I/O Input", 
    [18] = "DS Enhanced I/O Output", 
    [19] = "Kinect_Header", 
    [20] = "Kinect_Extra1", 
    [21] = "Kinect_Vertices1", 
    [22] = "Kinect_Extra2", 
    [23] = "Kinect_Vertices2", 
    [24] = "Kinect_Joystick", 
    [25] = "Kinect_Custom"
  }
  local f_d2r_ext_tag   = ProtoField.uint8("d2r.ext.tag", "Extension Tag", base.DEC, d2r_ext_tags)

  local f_d2r_ext_val	= ProtoField.bytes("d2r.ext.val", "Extension Data")
  local f_d2r_crc       = ProtoField.uint32("d2r.crc", "CRC", base.HEX)

  p_d2r.fields = {
    f_d2r_packet, 
    f_d2r_control, 
    f_d2r_input, 
    f_d2r_team, 
    f_d2r_alliance, 
    f_d2r_position, 
    f_d2r_joystick1, 
    f_d2r_js_btn1, 
    f_d2r_joystick2, 
    f_d2r_js_btn2, 
    f_d2r_joystick3, 
    f_d2r_js_btn3, 
    f_d2r_joystick4, 
    f_d2r_js_btn4, 
    f_d2r_analog_1, 
    f_d2r_analog_2, 
    f_d2r_analog_3, 
    f_d2r_analog_4, 
    f_d2r_crio_check, 
    f_d2r_fpga_chk_1, 
    f_d2r_fpga_chk_2, 
    f_d2r_fpga_chk_3, 
    f_d2r_fpga_chk_4, 
    f_d2r_version, 
    f_d2r_ext, 
    f_d2r_ext_len, 
    f_d2r_ext_tag, 
    f_d2r_ext_val, 
    f_d2r_crc
  }

  function p_d2r.dissector (buf, pkt, root)
    if (buf:len() < 1024) then return end
    pkt.cols.protocol = p_d2r.name

    local subtree = root:add(p_d2r, buf(0))

    subtree:add(f_d2r_packet, buf(0, 2))
    
    local val = buf(2, 1):uint()
    local ctrl = subtree:add(f_d2r_control, buf(2, 1))
    ctrl:add("Reset:         " .. (hasbit(val, bit(7)) and "Set" or "Unset"))
    ctrl:add("!EStop:        " .. (hasbit(val, bit(6)) and "Set" or "Unset"))
    ctrl:add("Enabled:       " .. (hasbit(val, bit(5)) and "Set" or "Unset"))
    ctrl:add("Autonomous:    " .. (hasbit(val, bit(4)) and "Set" or "Unset"))
    ctrl:add("Unknown:       " .. (hasbit(val, bit(3)) and "Set" or "Unset"))
    ctrl:add("Resync:        " .. (hasbit(val, bit(2)) and "Set" or "Unset"))
    ctrl:add("cRIO Checksum: " .. (hasbit(val, bit(1)) and "Set" or "Unset"))
    ctrl:add("FPGA Checksum: " .. (hasbit(val, bit(0)) and "Set" or "Unset"))
    
    val = buf(3, 1):uint()
    local out = subtree:add(f_d2r_input, buf(3, 1))
    out:add("Input 1: " .. (hasbit(val, bit(0)) and "Set" or "Unset"))
    out:add("Input 2: " .. (hasbit(val, bit(1)) and "Set" or "Unset"))
    out:add("Input 3: " .. (hasbit(val, bit(2)) and "Set" or "Unset"))
    out:add("Input 4: " .. (hasbit(val, bit(3)) and "Set" or "Unset"))
    out:add("Input 5: " .. (hasbit(val, bit(4)) and "Set" or "Unset"))
    out:add("Input 6: " .. (hasbit(val, bit(5)) and "Set" or "Unset"))
    out:add("Input 7: " .. (hasbit(val, bit(6)) and "Set" or "Unset"))
    out:add("Input 8: " .. (hasbit(val, bit(7)) and "Set" or "Unset"))
    
    subtree:add(f_d2r_team, buf(4, 2))
    subtree:add(f_d2r_alliance, buf(6, 1))
    subtree:add(f_d2r_position, buf(7, 1))

    local js1 = subtree:add(f_d2r_joystick1, buf(8, 6))
    js1:add(f_d2r_js_btn1, buf(14, 2))
    js1:add("Axis 1: " .. buf(8, 1))
    js1:add("Axis 2: " .. buf(9, 1))
    js1:add("Axis 3: " .. buf(10, 1))
    js1:add("Axis 4: " .. buf(11, 1))
    js1:add("Axis 5: " .. buf(12, 1))
    js1:add("Axis 6: " .. buf(13, 1))
    
    local js2 = subtree:add(f_d2r_joystick2, buf(16, 6))
    js2:add(f_d2r_js_btn2, buf(22, 2))
    js2:add("Axis 1: " .. buf(16, 1))
    js2:add("Axis 2: " .. buf(17, 1))
    js2:add("Axis 3: " .. buf(18, 1))
    js2:add("Axis 4: " .. buf(19, 1))
    js2:add("Axis 5: " .. buf(20, 1))
    js2:add("Axis 6: " .. buf(21, 1))
    
    local js3 = subtree:add(f_d2r_joystick3, buf(24, 6))
    js3:add(f_d2r_js_btn3, buf(30, 2))
    js3:add("Axis 1: " .. buf(24, 1))
    js3:add("Axis 2: " .. buf(25, 1))
    js3:add("Axis 3: " .. buf(26, 1))
    js3:add("Axis 4: " .. buf(27, 1))
    js3:add("Axis 5: " .. buf(28, 1))
    js3:add("Axis 6: " .. buf(29, 1))
    
    local js4 = subtree:add(f_d2r_joystick4, buf(32, 6))
    js4:add(f_d2r_js_btn4, buf(38, 2))
    js4:add("Axis 1: " .. buf(32, 1))
    js4:add("Axis 2: " .. buf(33, 1))
    js4:add("Axis 3: " .. buf(34, 1))
    js4:add("Axis 4: " .. buf(35, 1))
    js4:add("Axis 5: " .. buf(36, 1))
    js4:add("Axis 6: " .. buf(37, 1))
    
    subtree:add(f_d2r_analog_1, buf(40, 2))
    subtree:add(f_d2r_analog_2, buf(42, 2))
    subtree:add(f_d2r_analog_3, buf(44, 2))
    subtree:add(f_d2r_analog_4, buf(46, 2))
    
    subtree:add(f_d2r_crio_chk, buf(48, 8))

    subtree:add(f_d2r_fpga_chk_1, buf(56, 4))
    subtree:add(f_d2r_fpga_chk_2, buf(60, 4))
    subtree:add(f_d2r_fpga_chk_3, buf(64, 4))
    subtree:add(f_d2r_fpga_chk_4, buf(68, 4))
    
    subtree:add(f_d2r_version, buf(72, 8))
    
    local ext = subtree:add(f_d2r_ext, buf(80))
    local ext_off = 80
    local ext_len = 0
    local ext_tag = 0
    local ext_val
    local ext_tlv
    while ext_off < 1020 do
	ext_len = buf(ext_off, 1):uint()
	if (ext_len == 0) then break end
	ext_tag = buf(ext_off+1, 1):uint()
	if (ext_tag == 0) then break end
	ext_val = buf(ext_off+2, ext_len-1)
	ext_tlv = ext:add("len " .. ext_len .. " tag " .. ext_tag .. " val " .. ext_val, buf(ext_off, ext_len))
	ext_tlv:add(f_d2r_ext_len, buf(ext_off, 1))
	ext_tlv:add(f_d2r_ext_tag, buf(ext_off+1, 1))
	if (ext_len > 1) then
	    if (ext_tag == 17) then
		local eioin = Dissector.get("eioin")
		eioin:call(buf(ext_off+2):tvb(), pkt, ext_tlv)
	    elseif (ext_tag == 18) then
		local eioout = Dissector.get("eioout")
		eioout:call(buf(ext_off+2):tvb(), pkt, ext_tlv)
	    else
		ext_tlv:add(f_d2r_ext_val, buf(ext_off+2, ext_len-1))
	    end
	end
	ext_off = ext_off + 1 + ext_len
    end
    subtree:add(f_d2r_crc, buf(1020, 4))
  end

  -- Robot-to-DS protocol and its fields
  p_r2d = Proto ("r2d", "Robot To Driver Station Protocol")

  local f_r2d_control = ProtoField.uint8("r2d.command", "Control", base.HEX)
  local f_r2d_battery = ProtoField.uint16("r2d.battery", "Battery", base.HEX)
  local f_r2d_output  = ProtoField.uint8("r2d.output", "Digital Outputs", base.HEX)
  local f_r2d_unk_1   = ProtoField.bytes("r2d.unknown1", "Unknown 1")
  local f_r2d_team    = ProtoField.uint16("r2d.team", "Team Number", base.DEC)
  local f_r2d_mac     = ProtoField.bytes("r2d.mac", "MAC Address")
  local f_r2d_comm_ver = ProtoField.string("r2d.comm", "FRC Comm Version")
  local f_r2d_unk_2 = ProtoField.bytes("r2d.unknown2", "Unknown 2")
  local f_r2d_mode    = ProtoField.uint16("r2d.mode", "Operating Mode", base.HEX)
  local f_r2d_packet  = ProtoField.uint16("r2d.packet", "Packet Index", base.DEC)
  local f_r2d_update  = ProtoField.uint8("r2d.packet", "Update Number", base.DEC)
  local f_r2d_user    = ProtoField.bytes("r2d.user", "User Data")
  local f_r2d_status  = ProtoField.bytes("r2d.status", "Robot Status")
  local f_r2d_eio     = ProtoField.bytes("r2d.eio", "Enhanced I/O Data")
  local f_r2d_eio_size = ProtoField.uint8("r2d.eio.size", "size")
  local f_r2d_eio_id  = ProtoField.uint8("r2d.eio.id", "tag")
  local f_r2d_crc     = ProtoField.uint32("r2d.crc", "CRC", base.HEX)
  local f_r2d_lcd     = ProtoField.string("r2d.lcd", "LCD")
  local f_r2d_lcd_cmd = ProtoField.uint16("r2d.lcd.cmd", "LCD command", base.HEX)
  local f_r2d_lcd_row1 = ProtoField.string("r2d.lcd.row1", "LCD row 1")
  local f_r2d_lcd_row2 = ProtoField.string("r2d.lcd.row2", "LCD row 2")
  local f_r2d_lcd_row3 = ProtoField.string("r2d.lcd.row3", "LCD row 3")
  local f_r2d_lcd_row4 = ProtoField.string("r2d.lcd.row4", "LCD row 4")
  local f_r2d_lcd_row5 = ProtoField.string("r2d.lcd.row5", "LCD row 5")
  local f_r2d_lcd_row6 = ProtoField.string("r2d.lcd.row6", "LCD row 6")

  p_r2d.fields = {
    f_r2d_control, 
    f_r2d_battery, 
    f_r2d_output, 
    f_r2d_unk_1, 
    f_r2d_team, 
    f_r2d_mac, 
    f_r2d_comm_ver, 
    f_r2d_unk_2, 
    f_r2d_mode, 
    f_r2d_packet, 
    f_r2d_update, 
    f_r2d_user, 
    f_r2d_status, 
    f_r2d_eio, 
    f_r2d_eio_size,
    f_r2d_eio_id,
    f_r2d_crc, 
    f_r2d_lcd, 
    f_r2d_lcd_cmd, 
    f_r2d_lcd_row1, 
    f_r2d_lcd_row2, 
    f_r2d_lcd_row3, 
    f_r2d_lcd_row4, 
    f_r2d_lcd_row5, 
    f_r2d_lcd_row6
  }

  function p_r2d.dissector (buf, pkt, root)
    if (buf:len() < 1024) then return end
    pkt.cols.protocol = p_r2d.name
    local subtree = root:add(p_r2d, buf(0))
    
    local val = buf(0, 1):uint()
    local ctrl = subtree:add(f_r2d_control, buf(0, 1))
    ctrl:add("Reset:         " .. (hasbit(val, bit(7)) and "Set" or "Unset"))
    ctrl:add("!EStop:        " .. (hasbit(val, bit(6)) and "Set" or "Unset"))
    ctrl:add("Enabled:       " .. (hasbit(val, bit(5)) and "Set" or "Unset"))
    ctrl:add("Autonomous:    " .. (hasbit(val, bit(4)) and "Set" or "Unset"))
    ctrl:add("Unknown:       " .. (hasbit(val, bit(3)) and "Set" or "Unset"))
    ctrl:add("Resync:        " .. (hasbit(val, bit(2)) and "Set" or "Unset"))
    ctrl:add("CRio Checksum: " .. (hasbit(val, bit(1)) and "Set" or "Unset"))
    ctrl:add("FPGA Checksum: " .. (hasbit(val, bit(0)) and "Set" or "Unset"))
    
    local batt_v = buf(1, 1):uint()
    local batt_mv = buf(2, 1):uint()
    local batt = subtree:add(f_r2d_battery, buf(1, 2))
    batt:add("Volts: "..batt_v)
    batt:add("Milivolts: "..batt_mv)
    
    val = buf(3, 1):uint()
    local out = subtree:add(f_r2d_output, buf(3, 1))
    out:add("Output 1: " .. (hasbit(val, bit(0)) and "Set" or "Unset"))
    out:add("Output 2: " .. (hasbit(val, bit(1)) and "Set" or "Unset"))
    out:add("Output 3: " .. (hasbit(val, bit(2)) and "Set" or "Unset"))
    out:add("Output 4: " .. (hasbit(val, bit(3)) and "Set" or "Unset"))
    out:add("Output 5: " .. (hasbit(val, bit(4)) and "Set" or "Unset"))
    out:add("Output 6: " .. (hasbit(val, bit(5)) and "Set" or "Unset"))
    out:add("Output 7: " .. (hasbit(val, bit(6)) and "Set" or "Unset"))
    out:add("Output 8: " .. (hasbit(val, bit(7)) and "Set" or "Unset"))
    
    subtree:add(f_r2d_unk_1, buf(4, 4))
    subtree:add(f_r2d_team, buf(8, 2))
    subtree:add(f_r2d_mac, buf(10, 6))
    subtree:add(f_r2d_comm_ver, buf(16, 8))
    subtree:add(f_r2d_unk_2, buf(24, 4))
    subtree:add(f_r2d_mode, buf(28, 2))
    subtree:add(f_r2d_packet, buf(30, 2))
    subtree:add(f_r2d_update, buf(32, 1))
    subtree:add(f_r2d_user, buf(33, 911))
    subtree:add(f_r2d_status, buf(944, 40))
    local eio = subtree:add(f_r2d_eio, buf(984, 36))
    local eioout_size = buf(984, 1):uint()
    if (eioout_size > 1) then
	subtree:add(f_r2d_eio_size, buf(984, 1))
	subtree:add(f_r2d_eio_id, buf(985, 1))
	local eioout_id = buf(985, 1):uint()
	if (eioout_id == 18) then
	  local dis = Dissector.get("eioout")
	  dis:call(buf(986):tvb(), pkt, eio)
	end
    end
    subtree:add(f_r2d_crc, buf(1020, 4))
    if (buf:len() > 1024) then
	local lcd = subtree:add(f_r2d_lcd, buf(1024))
	lcd:add(f_r2d_lcd_cmd, buf(1024, 2))
	lcd:add(f_r2d_lcd_row1, buf(1026, 21))
	lcd:add(f_r2d_lcd_row2, buf(1047, 21))
	lcd:add(f_r2d_lcd_row3, buf(1068, 21))
	lcd:add(f_r2d_lcd_row4, buf(1089, 21))
	lcd:add(f_r2d_lcd_row5, buf(1110, 21))
	lcd:add(f_r2d_lcd_row6, buf(1131, 19))
    end
  end

  -- install dissectors
  local FRC_ROBOT_PORT = 1110
  local FRC_DS_PORT = 1150

  local udp_dissector_table = DissectorTable.get("udp.port")
  udp_dissector_table:add(FRC_ROBOT_PORT, p_d2r)
  udp_dissector_table:add(FRC_DS_PORT, p_r2d)
end

