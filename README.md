# ENFT

ENFT (**E**fficient **N**on-consecutive **F**eature **T**racking) is a method to find feature point correspondences among one or multiple video sequences. Our framework consists of steps of solving the feature ‘dropout’ problem when indistinctive structures, noise or even large image distortion exist, and of rapidly recognizing and joining common features located in different subsequences or even in different videos. We provide our GPU implementation using GLSL. 

###Related Publications:

Guofeng Zhang, Haomin Liu, Zilong Dong, Jiaya Jia, Tien-Tsin Wong, and Hujun Bao. **Efficient Non-Consecutive Feature Tracking for Robust Structure-from-Motion**. Submitted to *IEEE Transactions on Image Processing*. [**[arXiv report](http://arxiv.org/abs/1510.08012)**][**[video](https://drive.google.com/open?id=0B82Mv44r3F25LTh3ZERTRkZMVXc)**]

This source only provides feature tracking. The whole executable SfM system can be found at http://www.zjucvg.net/ls-acts/ls-acts.html.

##1. License

ENFT is released under a [GPLv3 license](http://choosealicense.com/licenses/gpl-3.0/). **It is for non-commercial research and educational use ONLY. Not for reproduction, distribution or commercial use.** If you use this executable for your academic publication, please acknowledge our work.

##2. Dependencies

* [GLUT](https://www.opengl.org/resources/libraries/glut/)
* [GLEW](http://glew.sourceforge.net/)
* [CVD](https://www.edwardrosten.com/cvd/)
* [CLAPACK](http://www.netlib.org/clapack/)
* [LEVMAR](http://www.ics.forth.gr/~lourakis/levmar/)

##3. Installation & usage

The project is built by VS2010. All the dependent libraries must be built and linked. We also provide the x64 libraries built by VS2010 in `lib/`.
	
The entry to the program is in `ENFT/ENFT.cpp`. Here is an example:

	int _tmain(int argc, _TCHAR* argv[])
	{
		std::vector<Video> videos(2);
		videos[0].imgFileName = "../data/0/0000.jpg";
		videos[0].start = 10;
		videos[0].step = 2;
		videos[0].end = 150;
		videos[1].imgFileName = "../data/1/0000.jpg";
		videos[1].start = 0;
		videos[1].step = 2;
		videos[1].end = 100;
		RunENFT(videos, "./param/", "../data/result.txt");
		return 0;
	}
	
`Video` is a struct specifying the input video sequence.

	struct Video
	{
		char *imgFileName;
		int start, step, stop;
	} Video;

In this example, inputs are two video sequences. The first video sequence consists of `../data/0/0010.jpg`, `../data/0/0012.jpg`, ..., `../data/0/0150.jpg` and the second consists of `../data/1/0000.jpg`, `../data/1/0002.jpg`, ..., `../data/1/0100.jpg`. We call `RunENFT` to run the algorithm:

	void RunENFT(const std::vector<Video> &videos, const char *paramDir, const char *outputFileName);
	
The first argument is a vector of input video sequences. The second argument `paramDir` specifies a directory containing the files of all the parameters the algorithm needs. The third argument `outputFileName` specifies the output file saving the feature tracking result. The output file containts multiple lines, one feature track for each line. Each line starts with an integer N, followed by N feature correspondences. Each correspondence is defined by `iSeq iFrm x y`. The two integers `iSeq` and `iFrm` are respectively the index of video sequence and image frame in the sequence, and the two floats `(x, y)` is the feature location in the image.

