while getopts p:b: option; do
  case "${option}"
  in
    b) BAUD_RATE=${OPTARG};;
    p) PORT=${OPTARG};;
  esac
done

if [ "$PORT" = "" ]; then
  PORT="/dev/ttyUSB0"
fi

if [ "$BAUD_RATE" = "" ]; then
  BAUD_RATE="115200"
fi

espruino -p $PORT -w dist/main.js --config SAVE_ON_SEND=1 --config SERIAL_THROTTLE_SEND=1 --config BAUD_RATE=$BAUD_RATE
