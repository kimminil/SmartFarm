import serial   # for Serial Comm.
import time     # use for sleep
from datetime import datetime   # return current time
import cv2   # conn. Cam
import requests
import json
Flask_access_test = 'http://192.168.0.102:5000/record_access/test_insert'
Flask_sensor_insert = 'http://192.168.0.102:5000/record_data/sensor_insert'
Flask_data_load = 'http://192.168.0.102:5000/record_data/data_sensor_load'
Flask_lstm_load = 'http://192.168.0.102:5000/test/predict'

STM32 = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1) #conn.serial esp32
NextionHmi = serial.Serial('/dev/ttyAMA2', baudrate=9600, timeout=1) #conn.serial NextionHmi

STM32_check_flag = False    #sensor_check_flag
cam_check_flag = False       #cam_check_flag
flask_check_flag = False        #db_check_flag 

#=================================NextionHmi===================================================================
def NextionHmi_send_num(component: str, num: int):  # NextionHmi_send_number component
    command = f'{component}.val={num}'
    NextionHmi.write(command.encode('utf-8'))
    NextionHmi.write(b'\xFF\xFF\xFF')
def NextionHmi_send_txt(component: str, txt: str): # NextionHmi_send_text component
    command = f'{component}.txt="{txt}"'
    NextionHmi.write(command.encode('utf-8'))
    NextionHmi.write(b'\xFF\xFF\xFF')
def NextionHmi_send_wave(component: int, num: int, num_min : int,num_max : int):  # NextionHmi_send_number component
    num = int((num-num_min)/(num_max-num_min)*255)
    command = f'add {component},0,{num}'
    NextionHmi.write(command.encode('utf-8'))
    NextionHmi.write(b'\xFF\xFF\xFF')
#=================================STM32===================================================================
def STM32_send(cmd):       #esp32 message send by serial
    #msg = f"{cmd}\n"
    STM32.write(cmd.encode('utf-8')) #esp32 message send
    
def check_STM32():   # check_sensor test start message send & receive complete massage
    STM32.write(b'STM32 Test Mode\n')  # check_sensor test start message
    time.sleep(0.1)  # small delay for response
    if STM32.in_waiting > 0:         # waiting serial input from esp32
        line = STM32.readline().decode('utf-8').rstrip()       # save serial input
        print(line)
        if line == "Pass":        # compare input with sensor_check_msg
            NextionHmi_send_txt("n9"," ")      # if true , send  blank massage to NextionHmi 
            print("Sensor Connection OK")   # debugging
            return True       # sensor_check_flag change  (F->T)
        else:
            NextionHmi_send_txt("n9","Sensor Connection Fail") # if false , send  fail massage to NextionHmi 
            print("Sensor check Fail") # debugging
    else:
        print("No response from sensor")   # debugging 
    return False   # sensor_check_flag not change  (F->F)

def receive_STM32():  # receive_sensor result from esp32
    if STM32.in_waiting > 0:
        result_sensor = STM32.readline().decode('utf-8').rstrip()
        try:
            temp, humi, co2, light,height = map(float, result_sensor.split(','))
            print(f"온도: {temp}℃, 습도: {humi}%, CO2: {co2}ppm, 채광률: {light}%, 수위: {height}") #debugging
            STM32_check_flag = True
            NextionHmi_send_txt("n9"," ")
            return temp, humi, co2, light,height
        except ValueError:
            print("센서 데이터 파싱 실패:", result_sensors)
            NextionHmi_send_txt("n9","Sensor Connection Fail")
            STM32_check_flag = False
    return 0, 0, 0, 0,0
#=================================CAMERA===================================================================
def check_camera():  # cam connect test
    cap = cv2.VideoCapture(0)  # Cam open
    if cap.isOpened():   # Cam conn.?
        NextionHmi_send_txt("n9"," ")   #if true , send  blank massage to NextionHmi 
        print("Camera Connection OK") #debugging
        cap.release() # Cam conn close
        return True # sensor_check_flag change  (F->T)
    else:
        NextionHmi_send_txt("n9","Camera Connection Fail")  # if false , send  fail massage to NextionHmi  
        print("Camera Connection Fail") #debugging
        cap.release() # Cam conn close
        return False # cam_check_flag not change  (F->F)
    
def receive_camera():  # cam connect test
    cap = cv2.VideoCapture(0)  # Cam open
    if cap.isOpened():   # Cam conn.?
        cam = "1번 작물 생장 완료"
        cap.release() # Cam conn close
        NextionHmi_send_txt("n9"," ")
        cam_check_flag = True 
        return cam
    else:
        NextionHmi_send_txt("n9","Camera Connection Fail")  # if false , send  fail massage to NextionHmi  
        print("Camera Connection Fail") #debugging
        cap.release() # Cam conn close
        cam_check_flag = False  # cam_check_flag not change  (F->F)
        return 0
#=================================FLASK===================================================================
def check_flask():  # cam connect test
    msg = {"access_time": datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
    response = requests.post(Flask_access_test, json=msg)
    if response.status_code == 200:
        data = response.json()
        if data.get('message') == 'Pass':
            print("Flask Connection OK") #debugging
            NextionHmi_send_txt("n9"," ")
            return True
        else:
            print("Flask Connection Fail(msg)") #debugging
            NextionHmi_send_txt("n9","DB Connection Fail")
            return False
    else:
        print("Flask Connection Fail") #debugging
        NextionHmi_send_txt("n9","DB Connection Fail")
        return False


def send_sensor_flask(temp,humi,co2,light,w_height):  # cam connect test
    msg = {
        "log_time": datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
        "temp": temp,
        "humi": humi,
        "co2": co2,
        "light": light,
        "w_height": w_height
    }
    response = requests.post(Flask_sensor_insert, json=msg)
    try :   
        if response.status_code == 200:
            data = response.json()
            if data.get('message') == 'Pass':
                print("send_data_flask : Success")
                flask_check_flag = True
                NextionHmi_send_txt("n9"," ")
    except Exception as e:
        print("send_data_flask : Fail,", e)
        NextionHmi_send_txt("n9","DB Connection Fail")
        flask_check_flag = False
        
def load_data_flask():  # cam connect test
    response = requests.post(Flask_data_load)  
    try :
        if response.status_code == 200:
            data = response.json()
            data_tuple = (
                data['No'],
                data['log_time'],
                data['temp'],
                data['humi'],
                data['co2'],
                data['light'],
                data['w_height'],
                data['cmd_Peltier'],
                data['cmd_fan'],
                data['cmd_light'],
                data['cmd_w_pump']
            )
            flask_check_flag = True
            print("Data Tuple:", data_tuple)
            NextionHmi_send_txt("n9"," ")
            return data_tuple
    except Exception as e:
        print("load_data_flask : Fail,", e)
        NextionHmi_send_txt("n9","DB Connection Fail")
        flask_check_flag = False
def request_prediction():
    try:
        response = requests.post(Flask_lstm_load)
        if response.status_code == 200:
            result = response.json()
            temp_data = result.get("temp")
            predict_data = result.get("predict")
            
            # Extract temperature list after colon
            temps_clean = temp_data.split(":")[-1].strip() if temp_data else "No data"
            
            # Extract predicted temperature after colon
            pred_value = predict_data.split(":")[-1].strip() if predict_data else "No prediction"
            
            print("Stored temperatures:", temps_clean)
            print("Predicted next temperature:", pred_value)
        else:
            print("Server error:", response.status_code, response.text)
    except requests.exceptions.RequestException as e:
        print("Request failed:", e)
#=================================MAIN===================================================================
while True:  
    try:
        current_time = datetime.now().strftime("%H:%M:%S")
        NextionHmi_send_txt("n8", current_time)
        if not STM32_check_flag:
            STM32_check_flag = check_STM32()
            if not STM32_check_flag:
                time.sleep(0.5)
        elif not cam_check_flag:
            cam_check_flag = check_camera()
            if not cam_check_flag:
                time.sleep(0.5)
        elif not flask_check_flag:
            flask_check_flag = check_flask()
            if not flask_check_flag:
                time.sleep(0.5)        
        else: # All checks passed
            #print("All devices connected. ") # debugging
            temp,humi,co2,light,w_height = receive_STM32()
            if temp != 0  :  #receive_sensor data empty?
                cam = receive_camera()
                if(cam) : #receive_cam data empty?
                    send_sensor_flask(temp,humi,co2,light,w_height)
                    result_flask = load_data_flask()
                    print(result_flask)  #debugging
                    for idx,data in enumerate(result_flask) :
                        idx = idx-2
                        match idx:
                            case 0 :  #temp
                                component = f"n{idx}"
                                NextionHmi_send_num(component, int(data))
                                #NextionHmi_send_wave(2, int(data),0,30)
                            case 1 : #humi
                                component = f"n{idx}"
                                NextionHmi_send_num(component, int(data))
                                #NextionHmi_send_wave(1, int(data),80,100)
                            case 2 : #co2
                                component = f"n{idx}"
                                NextionHmi_send_num(component, int(data))
                                #NextionHmi_send_wave(3, int(data),0,1500)
                            case 3 : #Light
                                component = f"n{idx}"
                                NextionHmi_send_num(component, int(data))
                                #NextionHmi_send_wave(4, int(data),0,1000)
                            case _:
                                component = f"n{idx}"
                                NextionHmi_send_txt(component, data)
                    STM32_msg = f'{result_flask[7]},{result_flask[8]},{result_flask[9]},{result_flask[10]}\r\n'   #return,save cmd mode value
                    STM32_send(STM32_msg)  # send cmd mode msg to esp32
                    STM32.write(STM32_msg.encode('utf-8')) # 이거 확인할것 STM32_send + Write

    except Exception as e:
        try:
            STM32.close()
        except:
            pass
        try:
            NextionHmi.close()
        except:
            pass
        print("Unexpected error:", e)
        break #break loop

