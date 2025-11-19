#!/usr/local/bin/luajit

-- copyright (c) 2020 chrono

local ffi = require "ffi"

local ffi_load = ffi.load
local ffi_null = ffi.null
local ffi_cdef = ffi.cdef
local ffi_typeof = ffi.typeof
local ffi_new = ffi.new
local ffi_C = ffi.C

ffi_cdef[[
int num;
int my_add(int a, int b);
]]

print(jit.version)
print(jit.os)
print(jit.arch)
print(jit.status())

-- 获取当前脚本目录
local function get_script_dir()
  local info = debug.getinfo(2, "S")
  local path = info.source:sub(2) -- 去掉开头的 '@'
  return path:match("(.*/)")
end

-- 构建 .so 路径
local so_path = get_script_dir() .. "build/liblua_shared.so"
local shared = ffi_load(so_path)

print(shared.num)

local x = shared.my_add(1, 2)
print(x)

-- 执行指令 /usr/local/bin/luajit ../shared.lua