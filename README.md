# waterIdCard
water Reg ID Card



![image](https://github.com/caiyishui/waterIdCard/blob/master/raw/id_01.png)
![image](https://github.com/caiyishui/waterIdCard/blob/master/raw/id_02.png)


本项目主要是对身份证号码的识别：
主要利用的是Opencv+OCR 技术

实现的流程主要通过以下步骤：

1.第一步： 分类器，身份证号码的文字的分类器 OCR jTessBoxEditor
可以参考我简书文章：http://www.jianshu.com/p/0353867aa301
	
2.第二步： 找到身份证号码位置，OpenCV 
主要通过匹配图片找到对应的身份证的位置，提取出身份证号码图片
	
3.第三步： 号码识别 OCR jTessBoxEditor
利用训练好的分类器进行识别。