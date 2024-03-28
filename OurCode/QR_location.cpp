#include "QR_location.h"


// ���ڽ���


int QR_Location::Main(string inputFolderPath, string outputFolderPath)
{
	std::vector<cv::String> imageFiles;
	cv::glob(inputFolderPath + "/*.png", imageFiles);
	int cnt = 0;
	for (auto imageFile : imageFiles)
	{
		while (1) {
			Mat src;
			src = imread(imageFile);
			Size dsize = Size(round(0.5 * src.cols), round(0.5 * src.rows));
			resize(src, src, dsize, 0, 0);
			//cap >> src;                                //���������һ֡ͼ��

			Mat srcCopy = src.clone();

			//canvasΪ���� ���ҵ��Ķ�λ����������
			Mat canvas;
			canvas = Mat::zeros(src.size(), CV_8UC3);

			Mat srcGray;

			//center_all��ȡ��������
			vector<Point> center_all;

			// ת��Ϊ�Ҷ�ͼ
			cvtColor(src, srcGray, COLOR_BGR2GRAY);

			// 3X3ģ��
			//blur(srcGray, srcGray, Size(3, 3));

			// ����ֱ��ͼ
			//equalizeHist(srcGray, srcGray);
			//int s = srcGray.at<Vec3b>(0, 0)[0];
			// ������ֵ����ʵ����� ����ͼ�����Ҳ������� ����������
			//threshold(srcGray, srcGray, 0, 255, THRESH_BINARY | THRESH_OTSU);

			Canny(srcGray, srcGray, 50, 150, 3);
			Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(3, 3));
			morphologyEx(srcGray, srcGray, MORPH_CLOSE, erodeStruct);
			imshow("threshold", srcGray);
			/*contours�ǵ�һ��Ѱ������*/
			/*contours2��ɸѡ��������*/
			vector<vector<Point>> contours;

			//	�����������
			vector<Vec4i> hierarchy;
			findContours(srcGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);


			// С���������
			int numOfRec = 0;

			// ��ⷽ��
			int ic = 0;
			int parentIdx = -1;
			for (int i = 0; i < contours.size(); i++)
			{
				if (hierarchy[i][2] != -1 && ic == 0)
				{
					parentIdx = i;
					ic++;
				}
				else if (hierarchy[i][2] != -1)
				{
					ic++;
				}
				else if (hierarchy[i][2] == -1)
				{
					parentIdx = -1;
					ic = 0;
				}
				if (ic == 3)
				{
					if (QR_Location::IsQrPoint(contours[parentIdx], src)) {
						RotatedRect rect = minAreaRect(Mat(contours[parentIdx]));

						// ��ͼ����
						Point2f points[4];
						rect.points(points);
						for (int j = 0; j < 4; j++) {
							line(src, points[j], points[(j + 1) % 4], Scalar(0, 255, 0), 2);
						}
						drawContours(canvas, contours, parentIdx, Scalar(0, 0, 255), -1);

						// ����������������
						center_all.push_back(rect.center);
						numOfRec++;
					}
					ic = 0;
					parentIdx = -1;
				}
			}

			// �������������εĲ���
			for (int i = 0; i < center_all.size(); i++)
			{
				line(canvas, center_all[i], center_all[(i + 1) % center_all.size()], Scalar(255, 0, 0), 3);
			}

			vector<vector<Point>> contours3;
			Mat canvasGray;
			cvtColor(canvas, canvasGray, COLOR_BGR2GRAY);
			findContours(canvasGray, contours3, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			vector<Point> maxContours;
			double maxArea = 0;
			// ��ԭͼ�л�����ά�������

			for (int i = 0; i < contours3.size(); i++)
			{
				RotatedRect rect = minAreaRect(contours3[i]);
				Point2f boxpoint[4];
				rect.points(boxpoint);
				for (int i = 0; i < 4; i++)
					line(src, boxpoint[i], boxpoint[(i + 1) % 4], Scalar(0, 0, 255), 3);

				double area = contourArea(contours3[i]);
				if (area > maxArea) {
					maxContours = contours3[i];
					maxArea = area;
				}
			}
			imshow("src", src);
			if (numOfRec < 3) {
				waitKey(10);
				continue;
			}
			// ���㡰�ء��Ĵ����ϵ
			int leftTopPointIndex = QR_Location::leftTopPoint(center_all);
			vector<int> otherTwoPointIndex = QR_Location::otherTwoPoint(center_all, leftTopPointIndex);
			// canvas�ϱ�ע�������ء��Ĵ����ϵ
			circle(canvas, center_all[leftTopPointIndex], 10, Scalar(255, 0, 255), -1);
			circle(canvas, center_all[otherTwoPointIndex[0]], 10, Scalar(0, 255, 0), -1);
			circle(canvas, center_all[otherTwoPointIndex[1]], 10, Scalar(0, 255, 255), -1);

			// ������ת��
			double angle = QR_Location::rotateAngle(center_all[leftTopPointIndex], center_all[otherTwoPointIndex[0]], center_all[otherTwoPointIndex[1]]);

			// �ó�֮ǰ�õ�����������
			RotatedRect rect = minAreaRect(Mat(maxContours));
			Mat image = QR_Location::transformQRcode(srcCopy, rect, angle);
			//imwrite("D:\\computer network\\project1\\img2\\image1", image);
			// չʾͼ��
			//imshow("QRcode", image);
			string str1 = outputFolderPath;
			string str2 = "/qrCode";
			string str3 = to_string(cnt);
			string str4 = ".png";
			string fileName = str1 + str2 + str3 + str4;
			imwrite(fileName, image);
			break;
			//imshow("canvas", canvas);

		}
	}
	
	return 0;
}

Mat QR_Location::transformCorner(Mat src, RotatedRect rect)
{
	// �����ת����
	Point center = rect.center;
	// ������ϽǺ����½ǵĽǵ㣬����Ҫ��֤������ͼƬ��Χ�����ڿ�ͼ
	Point TopLeft = Point(cvRound(center.x), cvRound(center.y)) - Point(rect.size.height / 2, rect.size.width / 2);  //��ת���Ŀ��λ��
	TopLeft.x = TopLeft.x > src.cols ? src.cols : TopLeft.x;
	TopLeft.x = TopLeft.x < 0 ? 0 : TopLeft.x;
	TopLeft.y = TopLeft.y > src.rows ? src.rows : TopLeft.y;
	TopLeft.y = TopLeft.y < 0 ? 0 : TopLeft.y;

	int after_width, after_height;
	if (TopLeft.x + rect.size.width > src.cols) {
		after_width = src.cols - TopLeft.x - 1;
	}
	else {
		after_width = rect.size.width - 1;
	}
	if (TopLeft.y + rect.size.height > src.rows) {
		after_height = src.rows - TopLeft.y - 1;
	}
	else {
		after_height = rect.size.height - 1;
	}
	// ��ö�ά���λ��
	Rect RoiRect = Rect(TopLeft.x, TopLeft.y, after_width, after_height);

	//	dst�Ǳ���ת��ͼƬ roiΪ���ͼƬ maskΪ��ģ
	double angle = rect.angle;
	Mat mask, roi, dst;
	Mat image;
	// �����н�ͼ��������ͼ��

	vector<Point> contour;
	// ��þ��ε��ĸ���
	Point2f points[4];
	rect.points(points);
	for (int i = 0; i < 4; i++)
		contour.push_back(points[i]);

	vector<vector<Point>> contours;
	contours.push_back(contour);
	// ���н�ͼ���л�������
	drawContours(mask, contours, 0, Scalar(255, 255, 255), -1);
	// ͨ��mask��Ĥ��src���ض�λ�õ����ؿ�����dst�С�
	src.copyTo(dst, mask);
	// ��ת
	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(dst, image, M, src.size());
	// ��ͼ
	roi = image(RoiRect);

	return roi;
}

// �ò������ڼ���Ƿ��ǽǵ㣬�����������������
bool QR_Location::IsQrPoint(vector<Point>& contour, Mat& img) {
	double area = contourArea(contour);
	// �ǵ㲻����̫С
	if (area < 30)
		return 0;
	RotatedRect rect = minAreaRect(Mat(contour));
	double w = rect.size.width;
	double h = rect.size.height;
	double rate = min(w, h) / max(w, h);
	if (rate > 0.7)
	{
		// ������ת���ͼƬ�����ڰѡ��ء����������ڴ���
		Mat image = transformCorner(img, rect);
		if (QR_Location::isCorner(image))
		{
			return 1;
		}
	}
	return 0;
}

// �����ڲ����а�ɫ����ռȫ���ı���
double QR_Location::Rate(Mat& count)
{
	int number = 0;
	int allpixel = 0;
	for (int row = 0; row < count.rows; row++)
	{
		for (int col = 0; col < count.cols; col++)
		{
			if (count.at<uchar>(row, col) == 255)
			{
				number++;
			}
			allpixel++;
		}
	}
	//cout << (double)number / allpixel << endl;
	return (double)number / allpixel;
}

// �����ж��Ƿ����ڽ��ϵ�������
bool QR_Location::isCorner(Mat& image)
{
	// ����mask
	Mat imgCopy, dstCopy;
	Mat dstGray;
	imgCopy = image.clone();

	// ת��Ϊ�Ҷ�ͼ��
	cvtColor(image, dstGray, COLOR_BGR2GRAY);
	// ���ж�ֵ��

	threshold(dstGray, dstGray, 0, 255, THRESH_BINARY | THRESH_OTSU);
	dstCopy = dstGray.clone();  //����

	Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(dstCopy, dstCopy, MORPH_OPEN, erodeStruct);
	// �ҵ������봫�ݹ�ϵ
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dstCopy, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);


	for (int i = 0; i < contours.size(); i++)
	{
		//cout << i << endl;
		if (hierarchy[i][2] == -1 && hierarchy[i][3] != -1)
		{

			Rect rect = boundingRect(Mat(contours[i]));
			rectangle(image, rect, Scalar(0, 0, 255), 2);

			// ������ľ�����������ľ��εĶԱ�
			if (rect.width < imgCopy.cols * 2 / 7)      //2/7��Ϊ�˷�ֹһЩ΢С�ķ���
				continue;
			if (rect.height < imgCopy.rows * 2 / 7)      //2/7��Ϊ�˷�ֹһЩ΢С�ķ���
				continue;

			// �ж����к�ɫ���ɫ�Ĳ��ֵı���
			if (Rate(dstGray) > 0.20)
			{
				return true;
			}
		}
	}
	return  false;
}

int QR_Location::leftTopPoint(vector<Point> centerPoint) {
	int minIndex = 0;
	int multiple = 0;
	int minMultiple = 10000;
	multiple = (centerPoint[1].x - centerPoint[0].x) * (centerPoint[2].x - centerPoint[0].x) + (centerPoint[1].y - centerPoint[0].y) * (centerPoint[2].y - centerPoint[0].y);
	if (minMultiple > multiple) {
		minIndex = 0;
		minMultiple = multiple;
	}
	multiple = (centerPoint[0].x - centerPoint[1].x) * (centerPoint[2].x - centerPoint[1].x) + (centerPoint[0].y - centerPoint[1].y) * (centerPoint[2].y - centerPoint[1].y);
	if (minMultiple > multiple) {
		minIndex = 1;
		minMultiple = multiple;
	}
	multiple = (centerPoint[0].x - centerPoint[2].x) * (centerPoint[1].x - centerPoint[2].x) + (centerPoint[0].y - centerPoint[2].y) * (centerPoint[1].y - centerPoint[2].y);
	if (minMultiple > multiple) {
		minIndex = 2;
		minMultiple = multiple;
	}
	return minIndex;
}

vector<int> QR_Location::otherTwoPoint(vector<Point> centerPoint, int leftTopPointIndex) {
	vector<int> otherIndex;
	double waiji = (centerPoint[(leftTopPointIndex + 1) % 3].x - centerPoint[(leftTopPointIndex) % 3].x) *
		(centerPoint[(leftTopPointIndex + 2) % 3].y - centerPoint[(leftTopPointIndex) % 3].y) -
		(centerPoint[(leftTopPointIndex + 2) % 3].x - centerPoint[(leftTopPointIndex) % 3].x) *
		(centerPoint[(leftTopPointIndex + 1) % 3].y - centerPoint[(leftTopPointIndex) % 3].y);
	if (waiji > 0) {
		otherIndex.push_back((leftTopPointIndex + 1) % 3);
		otherIndex.push_back((leftTopPointIndex + 2) % 3);
	}
	else {
		otherIndex.push_back((leftTopPointIndex + 2) % 3);
		otherIndex.push_back((leftTopPointIndex + 1) % 3);
	}
	return otherIndex;
}

double QR_Location::rotateAngle(Point leftTopPoint, Point rightTopPoint, Point leftBottomPoint) {
	double dy = rightTopPoint.y - leftTopPoint.y;
	double dx = rightTopPoint.x - leftTopPoint.x;
	double k = dy / dx;
	double angle = atan(k) * 180 / CV_PI;//ת���Ƕ�
	if (leftBottomPoint.y < leftTopPoint.y)
		angle -= 180;
	return angle;
}

Mat QR_Location::transformQRcode(Mat src, RotatedRect rect, double angle)
{
	// �����ת����
	Point center = rect.center;
	// ������ϽǺ����½ǵĽǵ㣬����Ҫ��֤������ͼƬ��Χ�����ڿ�ͼ
	Point TopLeft = Point(cvRound(center.x), cvRound(center.y)) - Point(rect.size.height / 2, rect.size.width / 2);  //��ת���Ŀ��λ��
	TopLeft.x = TopLeft.x > src.cols ? src.cols : TopLeft.x;
	TopLeft.x = TopLeft.x < 0 ? 0 : TopLeft.x;
	TopLeft.y = TopLeft.y > src.rows ? src.rows : TopLeft.y;
	TopLeft.y = TopLeft.y < 0 ? 0 : TopLeft.y;

	int after_width, after_height;
	if (TopLeft.x + rect.size.width > src.cols) {
		after_width = src.cols - TopLeft.x - 1;
	}
	else {
		after_width = rect.size.width - 1;
	}
	if (TopLeft.y + rect.size.height > src.rows) {
		after_height = src.rows - TopLeft.y - 1;
	}
	else {
		after_height = rect.size.height - 1;
	}
	// ��ö�ά���λ��
	Rect RoiRect = Rect(TopLeft.x, TopLeft.y, after_width, after_height);

	// dst�Ǳ���ת��ͼƬ��roiΪ���ͼƬ��maskΪ��ģ
	Mat mask, roi, dst;
	Mat image;
	// �����н�ͼ��������ͼ��

	vector<Point> contour;
	// ��þ��ε��ĸ���
	Point2f points[4];
	rect.points(points);
	for (int i = 0; i < 4; i++)
		contour.push_back(points[i]);

	vector<vector<Point>> contours;
	contours.push_back(contour);
	// ���н�ͼ���л�������
	drawContours(mask, contours, 0, Scalar(255, 255, 255), -1);
	// ͨ��mask��Ĥ��src���ض�λ�õ����ؿ�����dst�С�
	src.copyTo(dst, mask);
	// ��ת
	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(dst, image, M, src.size());
	// ��ͼ
	roi = image(RoiRect);

	return roi;
}
