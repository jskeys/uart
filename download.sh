ARCH=`uname -m`

URL="https://www.ftdichip.com/Drivers/D2XX/Linux/"

case $ARCH in
    armv7l)
        FILE="libftd2xx-arm-v7-hf-1.4.8.gz"
        ;;
    x86_64)
        FILE="libftd2xx-x86_64-1.4.8.gz"
esac

wget ${URL}${FILE}
tar -xaf ./${FILE}
