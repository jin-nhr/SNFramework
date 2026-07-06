param(
    [string]$InputFile,
    [string]$OutputHeader,
    [string]$OutputSource,
    [string]$VarName
)

# cso を読み込む
$bytes = [System.IO.File]::ReadAllBytes($InputFile)
$len = $bytes.Length

# ヘッダ生成（extern 宣言）
$header = @"
#pragma once
extern const unsigned char $VarName[];
extern const unsigned int ${VarName}_len;
"@
Set-Content -Path $OutputHeader -Value $header -Encoding UTF8

# cpp 生成（バイナリ配列）
$hex = $bytes | ForEach-Object { "0x{0:X2}" -f $_ }
$hexList = $hex -join ", "

$source = @"
#include "$OutputHeader"

const unsigned char $VarName[] = { $hexList };
const unsigned int ${VarName}_len = $len;
"@
Set-Content -Path $OutputSource -Value $source -Encoding UTF8