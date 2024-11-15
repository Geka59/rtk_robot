import threading
from XInput import *
import math
import serial
import struct
import time
import serial.tools.list_ports
ports = serial.tools.list_ports.comports()
import cv2
video=cv2.VideoCapture(1)
qcd=cv2.QRCodeDetector()
import numpy as np
video.set(3, 1920)
video.set(4, 1080)
print(video.get(cv2.CAP_PROP_FRAME_WIDTH))
print(video.get(cv2.CAP_PROP_FRAME_HEIGHT))
qr_text=""
#set_deadzone(DEADZONE_TRIGGER, 10)
import struct
left=127
right=127
# class Controller:
#     def __init__(self, center):
#         self.center = center

# controllers = (Controller((150., 100.)),
#                Controller((450., 100.)),
#                Controller((150., 300.)),
#                Controller((450., 300.)))
kin_angle1=0
kin_angle2=0
kin_angle3=0
auto_mode=False
def rescale_frame(frame, percent=75):
    #width = int(frame.shape[1] * percent/ 100)
    #height = int(frame.shape[0] * percent/ 100)
    #dim = (1540, 800)
    dim = (1540, 795)
    return cv2.resize(frame, dim, interpolation = cv2.INTER_LINEAR)

timing=time.time()

cox =[100,100,198,198]
coy =[170,51,51,19]

flag_1sec=False

camera_matrix = np.array([[704.13172952, 0.0, 308.54435489],
 [0.00,669.06918148 ,223.44098369],
 [0.00,0.00,1.00]]);
dist_coefs = np.array([-1.60345488, 3.78136809, 0.03585487, 0.01674275, -7.60275866]);

b_angle=False

qr_mode=False
line_mode=False
port = "COM8"  # Replace with the appropriate COM port name 9COM 444
baudrate = 57600  # Replace with the desired baud rate

ser = serial.Serial(port, baudrate=baudrate) # открываем порт holibro
left_position=[0,0]
l_thumb_stick_pos=[127,127]
r_thumb_stick_pos=[127,127]
r_trigger_index_pos=0
l_trigger_index_pos=0
a_button=0
b_button=0
x_button=0
y_button=0
left_shoulder=0
right_shoulder=0
dpad_left = 0
dpad_right = 0
dpad_up = 0
dpad_down = 0
et=0


kr_x=0
kr_y=0

def dot(vA, vB):
    return vA[0] * vB[0] + vA[1] * vB[1]



def crc16(data : bytearray, offset , length):
    if data is None or offset < 0 or offset > len(data)- 1 and offset+length > len(data):
        return 0
    crc = 0xFFFF
    for i in range(0, length):
        crc ^= data[offset + i] << 8
        for j in range(0,8):
            if (crc & 0x8000) > 0:
                crc =(crc << 1) ^ 0x1021
            else:
                crc = crc << 1
    return crc & 0xFFFF


def read_serial():
    response = ser.readline()
    decoded_response = response.decode('utf-8')
    print(decoded_response)

I=0
prevErr=0

def compute_pid(input,dt):
    global I
    global prevErr
    kp=1.2 #1.3
    err = 330 - input
    #prevErr=err
    #out=err+prevErr
    return err*kp
    # ki=0
    # kd=0
    # err = 330 - input
    # I=I+(330-input)#*dt
    # D=(err-prevErr)#/dt
    # prevErr=err
    # return (err*kp+I*ki+D*kd)

def write_port(ser):
    while True:
        #global a_button
        #global b_button

        global qr_mode
        global line_mode
        global left_position
        global l_thumb_stick_pos
        global r_thumb_stick_pos
        global r_trigger_index_pos
        global l_trigger_index_pos
        global a_button
        global b_button
        global x_button
        global y_button
        global left_shoulder
        global right_shoulder
        global dpad_left
        global dpad_right
        global dpad_up
        global dpad_down
        events = get_events()
        for event in events:

            if event.type == EVENT_CONNECTED:
                pass
            elif event.type == EVENT_DISCONNECTED:
                pass


            elif event.type == EVENT_STICK_MOVED:
                if event.stick == LEFT:
                    l_thumb_stick_pos = (int(round(127 + (127 * event.x), 0)), int(round(127 + (127 * event.y), 0)))
                    left_position = l_thumb_stick_pos
                elif event.stick == RIGHT:

                    r_thumb_stick_pos = (int(round(127 + (127 * event.x), 0)), int(round(127 + (127 * event.y), 0)))

            elif event.type == EVENT_BUTTON_PRESSED:
                if event.button == "A":
                    if a_button == 1:
                        a_button = 0
                    else:
                        a_button = 1
                elif event.button == "Y":
                    y_button = 1
                elif event.button == "X":
                    x_button = 1
                elif event.button == "B":
                    b_button = 1

                elif event.button == "DPAD_LEFT":
                    dpad_left = 1
                elif event.button == "DPAD_RIGHT":
                    dpad_right = 1
                elif event.button == "DPAD_UP":
                    dpad_up = 1
                elif event.button == "DPAD_DOWN":
                    dpad_down = 1
                elif event.button == "BACK":
                    qr_mode = (not qr_mode)
                elif event.button == "START":
                    line_mode = (not line_mode)


                elif event.button == "LEFT_SHOULDER":
                    left_shoulder = 1
                elif event.button == "RIGHT_SHOULDER":
                    right_shoulder = 1

            elif event.type == EVENT_BUTTON_RELEASED:
                if event.button == "DPAD_LEFT":
                    dpad_left = 0
                elif event.button == "DPAD_RIGHT":
                    dpad_right = 0
                elif event.button == "DPAD_UP":
                    dpad_up = 0
                elif event.button == "DPAD_DOWN":
                    dpad_down = 0
                elif event.button == "LEFT_SHOULDER":
                    left_shoulder = 0
                elif event.button == "RIGHT_SHOULDER":
                    right_shoulder = 0
                elif event.button == "X":
                    x_button = 0
                elif event.button == "B":
                    b_button = 0
                elif event.button == "Y":
                    y_button = 0


            elif event.type == EVENT_TRIGGER_MOVED:
                if event.trigger == LEFT:
                    l_trigger_index_pos = (int(round(150 * event.value, 0)))
                elif event.trigger == RIGHT:
                    r_trigger_index_pos = (int(round(150 * event.value, 0)))

        if a_button == 1:

            control_mode = 'Manipulator control'





        else:
            control_mode = 'Driving mode'

        control_sum = 0

        if (line_mode) or auto_mode:
            str_otpr_w = [(l_thumb_stick_pos[0]), (left), (r_thumb_stick_pos[0]), (right),
                          a_button, b_button, x_button, y_button, dpad_right, dpad_left, dpad_up, dpad_down, right_shoulder,
                          left_shoulder, r_trigger_index_pos, l_trigger_index_pos, 0, line_mode, kin_angle3]


        else:
            str_otpr_w = [(l_thumb_stick_pos[0]), (l_thumb_stick_pos[1]), (r_thumb_stick_pos[0]), (r_thumb_stick_pos[1]),
                          a_button, b_button, x_button, y_button, dpad_right, dpad_left, dpad_up, dpad_down, right_shoulder,
                          left_shoulder, r_trigger_index_pos, l_trigger_index_pos, 0, line_mode, kin_angle3]

        control_crc = crc16(str_otpr_w, 0, 19)
        crc0 = control_crc & 0x00FF
        crc1 = (control_crc & 0xFF00) >> 8
        if line_mode or auto_mode:
            str_otpr = [255, (l_thumb_stick_pos[0]), left, (r_thumb_stick_pos[0]), right,
                        a_button, b_button, x_button, y_button, dpad_right, dpad_left, dpad_up, dpad_down, right_shoulder,
                        left_shoulder, r_trigger_index_pos, l_trigger_index_pos, 0, line_mode, kin_angle3, crc0,
                        crc1]
        else:
            str_otpr = [255, (l_thumb_stick_pos[0]), (l_thumb_stick_pos[1]), (r_thumb_stick_pos[0]), (r_thumb_stick_pos[1]),
                        a_button, b_button, x_button, y_button, dpad_right, dpad_left, dpad_up, dpad_down, right_shoulder,
                        left_shoulder, r_trigger_index_pos, l_trigger_index_pos, 0, line_mode, kin_angle3, crc0,
                        crc1]

        # todo разделение потоков
        # todo установка граничных условий кинематики #  типа готово
        # todo добавлениe qr кодиков
        # todo установка начальных позиций манипулятора
        # todo ручное управление некоторых звееньев
        # todo родить линию
        ser.write(str_otpr)
        print(str_otpr)

        time.sleep(0.02)

thread = threading.Thread(target=write_port, args=(ser,))
thread.daemon = True  # Завершение потока при закрытии основного потока
thread.start()

prevdt=0
while 1:
    # if (ser.inWaiting() > 0):
    #     data_str = ser.read(ser.inWaiting()).decode('ascii')  # в try excpept
    #     print(data_str, end='')

    #lambda: read_serial()




    hasFrame, frame = video.read()
    if ((a_button==1) and (line_mode==False)):
        cv2.putText(frame, "Manipulator_mode", (10, 160), cv2.FONT_HERSHEY_SIMPLEX, 0.7, ((0, 255, 0)), 2,
                    cv2.LINE_AA)
    if ((a_button == 0) and (line_mode == False)):
        cv2.putText(frame, "Driving_mode", (10, 160), cv2.FONT_HERSHEY_SIMPLEX, 0.7, ((0, 255, 0)), 2,
                    cv2.LINE_AA)
    if qr_mode:

        h, w = frame.shape[:2]
        #newcameramtx, roi = cv2.getOptimalNewCameraMatrix(camera_matrix, dist_coefs, (w, h), 1, (w, h))

        #frame = cv2.undistort(frame, camera_matrix, dist_coefs, None, newcameramtx)

        #x, y, w, h = roi
        #frame = frame[y:y + h - 50, x + 70:x + w - 20]

        cv2.putText(frame, "QR_MODE", (480, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.8, ((0, 255, 0)), 2,
                    cv2.LINE_AA)
        if (qr_text!=""):
            cv2.putText(frame, qr_text[0], (100, 200), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1,
                        cv2.LINE_AA)
        retval, decoded_info, points, straigt_qr_code = qcd.detectAndDecodeMulti(frame)
        if retval:
            cv2.polylines(frame, points.astype(int), True, (0, 255, 0),3)

            if ((decoded_info[0]!='')and(qr_text=="")):
                qr_text=decoded_info

    else:
        qr_text=""

    if line_mode==False :
        left=127
        right=127
        et = time.time()
        #print("Rewrite")


    if line_mode and time.time()-et > 0.5: # автономное управление по линии
        print('Line_mode')
        min_pwm_speed=50
        kp=1.1
        cv2.putText(frame, "LINE_MODE", (480, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.8, ((0, 0, 255)), 2,
                    cv2.LINE_AA)

        #cv2.rectangle(frame, (0, 480), (640, 400), (255, 255, 255), -1)
        cv2.rectangle(frame, (0, 0), (800, 120), (255, 255, 255), -1)
        #cv2.rectangle(frame, (640, 480), (495, 320), (255, 255, 255), -1)
        low_b = np.uint8([123, 104, 85])  #[123,,104,85]                        #low_b = np.uint8([44, 44, 46])
        high_b = np.uint8([31, 35, 10]) #[31,35,10]
        mask = cv2.inRange(frame, high_b, low_b)
        contours, hierarchy = cv2.findContours(mask, 1, cv2.CHAIN_APPROX_NONE)
        if len(contours) > 0:
            c = max(contours, key=cv2.contourArea)
            M = cv2.moments(c)
            if M["m00"] != 0:
                cx = int(M['m10'] / M['m00'])
                cy = int(M['m01'] / M['m00'])

                cv2.putText(frame, str(320-cx), (320, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.8, ((0, 0, 255)), 2,
                            cv2.LINE_AA)
                dtx=time.time()-prevdt
                prevdt = time.time()
                print(dtx)
                pid_out=round(compute_pid(cx,dtx))

                right = 127 + pid_out +40 ## 28
                left = 127 - pid_out + 40 ## 28
                #right = 127 + round(kp * (320 - cx)) + 28
                #left = 127 - round(kp * (320 - cx)) + 28

                if right>254:
                    right=254
                if right<0:
                    right=0
                if left>254:
                    left=254
                if left<0:
                    left=0

                cv2.circle(frame, (cx, cy), 5, (255, 255, 255), -1)
        else:
            print("I don't see the line")

        cv2.drawContours(frame, c, -1, (0, 255, 0), 1)

        #cv2.imshow("Mask", mask)


    frame150 = rescale_frame(frame, percent=150)


    cv2.rectangle(frame150, (10, 10), (250, 200), (255, 255, 255), -1)
    cv2.line(frame150, (10, 170), (250, 170), (0, 0, 0), 2)  # black groun
    cv2.line(frame150, (40, 142), (211, 142), (0, 0, 0), 2)
    #cv2.line(frame150, (211, 142), (211, 170), (0, 0, 0), 2)
    cv2.circle(frame150, (cox[0], coy[0]), 4, (123, 124, 155), 4)  # zero point
    #cv2.circle(frame150, (cox[0], coy[0]), 119, (123, 0, 155), 4)  # first circ
    cv2.line(frame150, (cox[0], coy[0]), (cox[1], coy[1]), (255, 0, 0), 2)
    cv2.circle(frame150, (cox[1], coy[1]), 4, (255, 124, 155), 4)
    cv2.line(frame150, (cox[1], coy[1]), (cox[2], coy[2]), (0, 255, 0), 2)
    #cv2.circle(frame150, (cox[2], coy[2]), 98, (0, 155, 155), 4)  # second circ
    cv2.line(frame150, (cox[2], coy[2]), (cox[3] + 32, coy[3] + 32), (255, 255, 89), 2)
    if b_angle:
        cv2.putText(frame150,"Bad Angle",(60,197),cv2.FONT_HERSHEY_SIMPLEX,0.8,((147, 6, 167)),2,cv2.LINE_AA)




    try:
        cv2.imshow("Face detection", frame150)
    # если кадра нет
        if not hasFrame:
        # останавливаемся и выходим из цикла
            cv2.waitKey()
    except:
        video.release()

    if cv2.waitKey(1) & 0xFF == ord('l'):
        cv2.destroyAllWindows()

    if y_button==1 and auto_mode==False and(time.time() - timing >= 1.5):
        timing = time.time()
        auto_mode=True


    if y_button==1 and auto_mode==True and (time.time() - timing >= 0.2):
        auto_mode=False
        timing=time.time()

    if auto_mode:
        left=200
        right=200
        if time.time() - timing > 4.0: # автономно едем 4 сек
            timing = time.time()
            auto_mode=False


    # считаем время исполнения

    #print('Время исполнения:',1/elapsed_time, 'секунд')
