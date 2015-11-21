#!/usr/bin/env lua
-- This script is designed to copy files to the 2015 CCC rad1o-badge in MSC mode, mount it before if it's not already, and unmount it & sync.

-- Values in the table below are copied to the rad1o. Basicly "cp [value from table] [rad1o mountpoint]", so wildcards etc are allowed.
local files = {
	"l0dables/test.c1d",
}
local force = true -- Skipps questions


-- Find dev by matching a pattern, including block ammount & size, in dmesg.
local dmesg_p = io.popen("dmesg")
if not dmesg_p then
	print("Can't open dmesg, check your privileges!")
	os.exit(1)
end
local dev
while true do
	local cline = dmesg_p:read("*l")
	if cline then
		local m = cline:match(" %[(.-)%] 3072 512%-byte logical blocks:")
		if m then
			dev = "/dev/" .. m
		end
	else
		break
	end
end
dmesg_p:close()


if dev == nil then
	print("No device found! Make sure it's in MSC mode!")
	os.exit(2)
end
print("Device: ", dev)
print("Continue?")
if force then
	print("Force mode...")
else
	io.write("Y/n> ")
	local ans = io.read("*l")
	if ans:lower() == "y" or ans == "" then
		print("Please wait...")
	else
		print("Aborted!")
		os.exit(1)
	end
end


-- Check if we're already mounted
local mounts_p = io.popen("mount")
local mount = mount_p:read("*a"):match(dev .. " on (.-) type")
if mount then
	print("Already mounted!")
end


local mountpoint = "/mnt/tmp/" .. dev
if mount then
	-- We're already mounted
	mountpoint = mount
else
	-- Not mounted yet, create mount point
	if not os.execute("mkdir -p " .. mountpoint) then
		print("Can't create mount directory!")
		os.exit(3)
	end

	-- mount
	if not os.execute("mount " .. dev .. " " .. mountpoint) then
		print("Can't mount!")
		os.exit(4)
	end
end


-- Copy files
for _, file in ipairs(files) do
	if not os.execute("cp " .. file .. " " .. mountpoint) then
		print("Can't cp!")
		os.exit(5)
	end
end


-- Unmount
if not os.execute("umount " .. mountpoint) then
	print("Can't unmount!")
	os.exit(6)
end


-- Sync
if not os.execute("sync") then
	print("Can't sync!")
	os.exit(7)
end

print("Ok! Device is ready to be unplugged!")
