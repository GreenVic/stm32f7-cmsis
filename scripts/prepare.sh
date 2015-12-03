#! /bin/bash
set -euo pipefail
IFS=$'\n\t'

# Archives are to be downloaded from:
# http://www.keil.com/pack/Keil.STM32F7xx_DFP.x.x.x.pack

# RELEASE_VERSION="2.0.0"
# RELEASE_VERSION="2.1.0"
# RELEASE_VERSION="2.2.0"
RELEASE_VERSION="2.3.0"

FAMILY="STM32F7"
GITHUB_PROJECT="xpacks/stm32f7-cmsis"
NAME_PREFIX="Keil.${FAMILY}xx_DFP"
ARCHIVE_NAME="${NAME_PREFIX}.${RELEASE_VERSION}.pack"
ARCHIVE_URL="http://www.keil.com/pack/${ARCHIVE_NAME}"

LOCAL_ARCHIVE_FILE="/tmp/xpacks/${ARCHIVE_NAME}"

echo "Cleaning previous files..."
for f in *
do
  if [ "${f}" == "scripts" ]
  then
    :
  else
    rm -rf "${f}"
  fi
done

if [ ! -f "${LOCAL_ARCHIVE_FILE}" ]
then
  mkdir -p $(dirname ${LOCAL_ARCHIVE_FILE})
  curl -o "${LOCAL_ARCHIVE_FILE}" -L "${ARCHIVE_URL}"
fi

echo "Unpacking '${ARCHIVE_NAME}'..."
unzip -q "${LOCAL_ARCHIVE_FILE}"

echo "Removing unnecessary files..."
rm -rf \
_htmresc \
CMSIS/Flash \
Documentation \
Drivers/BSP/ \
Drivers/CMSIS/CMSIS?END*.* \
Drivers/CMSIS/index.html \
Drivers/CMSIS/README.txt \
Drivers/CMSIS/Documentation \
Drivers/CMSIS/DSP_Lib \
Drivers/CMSIS/Include \
Drivers/CMSIS/Lib \
Drivers/CMSIS/RTOS \
Drivers/STM32F?xx_HAL_Driver/ \
Flash \
MDK \
Middlewares \
Projects \
Utilities \
package.xml


find . -name '*.exe' -exec rm \{} \;

echo "Creating README.md..."
cat <<EOF >README.md
# ${FAMILY} CMSIS

This project, available from [GitHub](https://github.com/${GITHUB_PROJECT}),
includes the ${FAMILY} CMSIS files.

## Version

* v${RELEASE_VERSION}

## Documentation

The latest CMSIS documentation is available from
[keil.com](http://www.keil.com/cmsis).

## Original files

The original files are available from the \`originals\` branch.

These files were extracted from \`${ARCHIVE_NAME}\`.

To save space, the following folders/files were removed:

* all non-portable *.exe files
* _htmresc
* CMSIS/Flash/
* Documentation/
* Drivers/BSP/
* Drivers/CMSIS/CMSIS?END*.*
* Drivers/CMSIS/index.html
* Drivers/CMSIS/README.txt
* Drivers/CMSIS/Documentation
* Drivers/CMSIS/DSP_Lib/
* Drivers/CMSIS/Include/
* Drivers/CMSIS/Lib/
* Drivers/CMSIS/RTOS/
* Drivers/STM32F?xx_HAL_Driver/
* Flash/
* MDK/
* Middlewares/
* Projects/
* Utilities/
* package.xml

EOF

echo "Done."
