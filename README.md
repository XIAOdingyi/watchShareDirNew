/*
* 因项目需要用QT来读写共享文件夹
* 共享文件夹的设定可百度
* 路径设置为 "\\\\共享文件夹电脑IP/共享文件夹路径/"即可.
* 路径也可以设置为 "\\\\共享文件夹电脑计算机名/共享文件夹路径/"
* 设置ip时不在同一网段无法访问共享文件夹，设置电脑计算机名时即使不在同一网段也可以访问
* 难点项：
*		在监控局域网内的共享文件夹时directoryChanged信号会触发两次，暂未解决
* 可监控目录亦可监控目录下的文件
* 2024修改 去掉单例模式
* 将文件变化信号进行拆分，区别新增、删除、修改三种方式
*/