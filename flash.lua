#!/usr/bin/env lua
local files = {
	"l0dables/test.c1d",
}

local force = true -- Skipps questions

local dmesg_p = io.popen("dmesg")
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

local mountpoint = "/mnt/tmp/" .. dev
os.execute("mkdir -p " .. mountpoint)
os.execute("mount " .. dev .. " " .. mountpoint)
for _, file in ipairs(files) do
	os.execute("cp " .. file .. " " .. mountpoint)
end
os.execute("umount " .. mountpoint)
os.execute("sync")
print("Ok! Device is ready to be unplugged!")
