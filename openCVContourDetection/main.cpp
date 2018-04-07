#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/imgproc/imgproc.hpp" 
#include <string> 
#include <iostream>
#include <vector>
#include <io.h> 
#include <cstdint>

using namespace cv;
using namespace std;

vector<vector<Point>> getVectorOfContours(Mat image, int minRange = 0, int maxRange = 80);
vector<vector<Point>> getContourDrawing(Mat image, Mat& result, int minRange = 0, int maxRange = 80);
vector<vector<Point>> getContourDrawing(vector<vector<Point>> vectorOfContours, Mat image, Mat& result);
vector<Point> getContourDrawingPattern(Mat image, Mat& result, int minRange = 0, int maxRange = 80);
vector<Point> getContourDrawingPattern(vector<vector<Point>> vectorOfContours, Mat image, Mat& result);

int main()
{
	_finddata_t danePliku;
	intptr_t uchwyt = _findfirst("data/photos/*.jpg", &danePliku);

	if (uchwyt == -1)
	{
		cout << "B£¥D! - nie wczytano pliku" << endl;
	}
	else
	{
		do
		{
			// Nazwa œcie¿ki zdjêcia
			string leftSrc = "data/photos/";
			string imageSrc = leftSrc + danePliku.name;

			// Kontur wzorca
			Mat patternMat = imread("data/pattern/pattern.jpg");
			Mat helpPatternResult;
			// Utorzenie konturu wzorca
			vector<Point> patternContour = getContourDrawingPattern(patternMat, helpPatternResult);

			// Definicja obszaru poszukiwania
			//Rect rectArea(0, 220, 1209, 394 - 182);

			// Zmniejszenie obrazu do 30%
			// Za³adowanie obrazu
			Mat photo = imread(imageSrc);
			/*Mat sizePhoto = imread("data/photos/size/size.jpg");
			resize(photo, photo, sizePhoto.size());*/

			// Wyciêcie czêœci zdjêcia g³ównego na podstawie prostok¹ta area
		//	Mat areaMat(photo, rectArea);
			Mat helpResult;
			vector<vector<Point>> areaContour = getContourDrawing(photo, helpResult);

			// Algorytm wynajdowania konturów
			for (int i = 0; i < areaContour.size(); i++)
			{
				// Utworzenie okna
				leftSrc = "contour" + std::to_string(i) + "_";
				string windowName = leftSrc + danePliku.name;

				// Znalezienie konturu
				double match = matchShapes(patternContour, areaContour[i], CV_CONTOURS_MATCH_I1, 0);
				cout << "Wynik " << i << " (" << windowName << ") : " << match << endl;

				// Wyœwietlenie konturu
				if (match < 0.05)
				{
					namedWindow(windowName, CV_WINDOW_AUTOSIZE);

					// TODO
					// Dodaæ przybli¿enie prostok¹tem
					Mat helpik = Mat::zeros(helpResult.size(), CV_8UC3);;
					drawContours(helpik, areaContour, i, Scalar(125, 125, 250), 2);
					imshow(windowName, helpik);
				}
			}
		} while (_findnext(uchwyt, &danePliku) != -1);

		cv::waitKey(0);
	}
}

vector<vector<Point>> getVectorOfContours(Mat image, int minRange, int maxRange)
{
	Mat imageGray, imageBinary, imageContour;
	// Zamiana z kolorowego obrazka na czarno bialy
	cvtColor(image, imageGray, CV_BGR2GRAY);

	// Progowanie, polegajace na zmianie na biale punkty, ktorych odcien szarosci jest w wydzielone przedziale. Reszta pozostaje czarna
	inRange(imageGray, minRange, maxRange, imageBinary);

	// **************************************************
	// ******** Tutaj sekcja transformacji obrazu *******
	blur(imageBinary, imageBinary, cv::Size(3, 3));
	erode(imageBinary, imageBinary, cv::Mat(), cv::Point(-1, -1), 3);

	// ******** Koniec transformacji obrazu *************
	// **************************************************

	// Deklaracja wektora, ktory przechowuje zbiory (wektory) punktow, tworzacych kontur
	vector<vector<Point>> vectorOfContours;
	imageBinary.copyTo(imageContour);

	// Znajdowanie konturow.
	// Pierwszy parametr - zdjêcie, gdzie szukamy konturów
	// drugi parametr - wektor, gdzie przechowujemy kontury
	// CV_RETR_EXTERNAL oznacza, ¿e wyszukiwane sa tylko kontury zewnetrzne
	// Pozostale okreslaja sposob aproksymacji kontyrow oraz przesuniecie kazdego z punktow konturu
	findContours(imageContour, vectorOfContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	// Metoda zwracaj¹ca obrys (zdjêcie) na podstawie wektora konturów oraz zdjêcia, którego rozmiar bêdzie determinowa³ rozmiar zdjêcia z obrysem
	//	imageContour = getContourDrawing(vectorOfContours, imageContour);

	return vectorOfContours;
}

vector<Point> getContourDrawingPattern(Mat image, Mat& result, int minRange, int maxRange)
{
	vector<vector<Point>> vectorOfContours = getVectorOfContours(image, minRange, maxRange);

	return getContourDrawingPattern(vectorOfContours, image, result);
}

vector<Point> getContourDrawingPattern(vector<vector<Point>> vectorOfContours, Mat image, Mat& result)
{
	vector<Point> contour;
	Mat drawing = Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i < vectorOfContours.size(); i++)
	{
		approxPolyDP(Mat(vectorOfContours[i]), contour, 3, true);
		drawContours(drawing, vectorOfContours, i, Scalar(125, 125, 250), 2);
	}
	drawing.copyTo(result);
	return contour;
}

vector<vector<Point>> getContourDrawing(Mat image, Mat& result, int minRange, int maxRange)
{
	vector<vector<Point>> vectorOfContours = getVectorOfContours(image, minRange, maxRange);

	return getContourDrawing(vectorOfContours, image, result);
}

vector<vector<Point>> getContourDrawing(vector<vector<Point>> vectorOfContours, Mat image, Mat& result)
{
	vector<vector<Point>> contour;
	vector<Point> helpVector;
	Mat drawing = Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i < vectorOfContours.size(); i++)
	{

		approxPolyDP(Mat(vectorOfContours[i]), helpVector, 3, true);
		drawContours(drawing, vectorOfContours, i, Scalar(125, 125, 250), 2);
		contour.push_back(helpVector);
	}
	drawing.copyTo(result);
	return contour;
}