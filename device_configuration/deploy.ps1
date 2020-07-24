
$repoDir = "$PSScriptRoot/../"

$env:PLATFORMIO_UPLOAD_FLAGS="--auth=<YOUR OTA PASSWORD HERE>"
$env:PATH="~/.platformio/penv/bin:~/.platformio/penv:$env:PATH"

Push-Location $repoDir

$device_configuration = Import-Csv "$repoDir/device_configuration/device_configuration.csv"

foreach($device in $device_configuration) {

    Write-Progress $device.hostname

    $device_constants_file_path = "$repoDir/src/device_constants.h"
    $device_constants_content = Get-Content $device_constants_file_path

    $device_constants_content = $device_constants_content -replace 'const bool enableMpl311a2 = (true|false);',
        "const bool enableMpl311a2 = $($device.enableMpl311a2);"
    $device_constants_content = $device_constants_content -replace 'const char[*] hostname = ".*";',
        "const char* hostname = `"$($device.hostname)`";"

    $device_constants_content | Set-Content $device_constants_file_path

    platformio run --target upload --upload-port $device.ip_address

}

Pop-Location
