###SecDupCloud-支持密文去重的安全云数据共享系统 

####@auther 陈思依

---
系统运行环境：

- JDK 1.7.0及以上
- JRE 1.7.0及以上
- VC++ 6.0

---

注意！！！：

由于美国贸易出口协定限制，Java密钥长度限制为128位，本系统使用AES-256算法，使用256位密钥，需将Java环境中security文件夹下的Local_Policy与US_Export_Policy文件进行替换。替换文件可在Oracle官网上依据版本进行下载

下载文件名： Java Cryptography Extension Unlimited Strength Jurisdiction Policy File

JCE 8 下载地址：
http://www.oracle.com/technetwork/java/javase/downloads/jce8-download-2133166.html
