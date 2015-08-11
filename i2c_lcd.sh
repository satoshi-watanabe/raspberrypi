#!/bin/sh
# LCD初期化
i2cset -y 1 0x3e 0 0x38 0x39 0x14 0x70 0x56 0x6c i
sleep 0.3
i2cset -y 1 0x3e 0 0x38 0x0c 0x01 i
sleep 0.3
# 1行目 "1234"
i2cset -y 1 0x3e 0 0x80 b
i2cset -y 1 0x3e 0x40 0x48 0x65 0x6c 0x6c 0x6f 0x21 0x21 i
# 2行目 "ABCDE"
i2cset -y 1 0x3e 0 0xc0 b
i2cset -y 1 0x3e 0x40 0x20 0x20 0x20 0xdc 0xb0 0xd9 0xc4 0xde i
