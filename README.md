# Human visualization on 3d Persistence of Vision display
The program gets disparity map from Asus Xtion Live Pro sensor, estimates 3d positions of human key points (head, shoulders, elbows and hands) and sends their coordinates to MCU based transmitter with nRF24l01, which transmitts them to 3d persistence of vision display

### Dependencies
[Qt] - C++ Framework\
[OpenNI2] - library for Asus Xtion Live Pro sensor\
[NiTE2] - human pose estimation library

[Qt]: https://www.qt.io
[OpenNI2]: https://github.com/occipital/OpenNI2
[NiTE2]: http://openni.ru/files/nite/index.html
