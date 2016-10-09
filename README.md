# ENFT

ENFT (**E**fficient **N**on-consecutive **F**eature **T**racking) is a method to find feature point correspondences among one or multiple video sequences. Our framework consists of steps of solving the feature ‘dropout’ problem when indistinctive structures, noise or even large image distortion exist, and of rapidly recognizing and joining common features located in different subsequences or even in different videos. We provide our GPU implementation using GLSL. We build the project using VS2010 under Windows 7.

###Related Publications:

Guofeng Zhang, Haomin Liu, Zilong Dong, Jiaya Jia, Tien-Tsin Wong, and Hujun Bao. **Efficient Non-Consecutive Feature Tracking for Robust Structure-from-Motion**. Submitted to *IEEE Transactions on Image Processing*. [**[arXiv report](http://arxiv.org/abs/1510.08012)**][**[video](https://drive.google.com/open?id=0B82Mv44r3F25LTh3ZERTRkZMVXc)**]

This source only provides feature tracking. The whole executable SfM system can be found at http://www.zjucvg.net/ls-acts/ls-acts.html.

##1. License

ENFT is released under a [GPLv3 license](http://choosealicense.com/licenses/gpl-3.0/). **It is for non-commercial research and educational use ONLY. Not for reproduction, distribution or commercial use.** If you use this executable for your academic publication, please acknowledge our work.

##2. Dependencies

* [GLUT]
* [GLEW]
* [CVD](https://www.edwardrosten.com/cvd/)
* [CLAPACK](http://www.netlib.org/clapack/)
* [LEVMAR](http://www.ics.forth.gr/~lourakis/levmar/)

##3. Building & Running
