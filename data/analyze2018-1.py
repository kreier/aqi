import time
import pandas as pd

start_time = time.time()

data = pd.read_csv('../input/aqi-first-data-from-april-2018/data-esp8266-1129419-2018-04-20.csv',sep=';',header=0,skiprows=1,index_col=False,names=['date','2','3','4','5','6','7','pm25','pm10','10','11','temp','hum','14','15','16','17','18','19','20','21'],usecols=['date','pm25','pm10','temp','hum'])
#data.drop(["Time","durP1","ratioP1","P1","durP2","SDS_P1","SDS_P2","BMP_temperature","BMP_pressure","BME280_temperature","BME280_humidity","BME280_pressure","Min_cycle","Max_cycle","Samples","Signal"],axis = 1, inplace = True)
print(data)

time_passed = time.time() - start_time
print('Import took %s seconds' % time_passed)
