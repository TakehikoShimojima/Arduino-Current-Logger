# Arduino-Current-Logger
Arduino ESP8266 と電流計モジュールINA226PRCを使った消費電力ロガー。
測定対象の機器の消費電流を５m秒間隔で測定し、電流が±3mA以上流れたら記録を開始。
20秒間測定、記録し、測定が終わると結果をシリアルに出力します。
