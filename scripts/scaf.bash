#!/usr/bin/env bash
name="$1"

cat >"src/${name}.c" <<EOS
#include "maf/${name}.h"

EOS

cat >"include/maf/${name}.h" <<EOS
#pragma once

EOS

cat >"test/${name}_test.c" <<EOS
#include "maf/${name}.h"

#include "test.h" // IWYU pragma: keep

TEST(maf_${name}) {
}
EOS
