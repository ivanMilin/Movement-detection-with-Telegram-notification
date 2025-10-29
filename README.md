# Movement detection with Telegram notification

![IoT-Architecture-1](https://github.com/user-attachments/assets/b4c4dd44-2e17-4022-8040-77fc61f9e79b)

## Project description
- The system is composed of two Raspberry Pi devices that communicate wirelessly with each other inside local network, one in the role of Edge node and the other Cloud.
- One Raspberry Pi functions as an Edge device, with a connected camera and an IR sensor that detects movement in the room.
- In case of motion detection, the edge device sends the video to the cloud (another Raspberry Pi), where the video is saved.
- After detection, the second Raspberry Pi, which is in the role of Cloud, sends a push Telegram notification to my phone, so that I can access the recording via the Streamlit web application on the phone.
- All Edge node related logic is implemented using C++ while all Cloud logic is implemented using Python.
