/* Program in c++
* 
* Module Frame
*
* Compilar: g++ -o main main.cpp `pkg-config opencv --cflags --libs`

* Se genera el exe
* Ejecutar: ./exe
*
* Va a recibir esto del codigo externo: output_frame_mat
*
*
*/


#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

#include <unistd.h>

#include "frame_mod.h"
#include <time.h>


using namespace cv;
using namespace std;

int main() {
    //string image_path = "/home/nuria/TFG_Informatica/Pruebas/Imagenes/Image0.jpeg";
    string str = "nuria";

    time_t current_time;
    int time_id;
    /* Obtain current time in seconds as frame ID. */
    current_time = time(NULL);
    time_id = current_time;


    //std::string image_path = cv::samples::findFile("Imagenes/Image01.jpeg");
    std::string image_path = cv::samples::findFile("Imagenes/Prueba0.jpeg");


    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Display window", img);

    // convert to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(img, hsvImage, CV_BGR2HSV);
    imshow("Display window2", hsvImage);

    //cv::Mat output_frame_mat = cv::imread(argv[1], cv::IMREAD_COLOR);
    //cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
    //imshow("Display window2", img);


    // create object of Frame class
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        //imwrite("Imagenes/Image0.jpeg", img);
        imwrite("Imagenes/Prueba_result0.jpeg", img);
    }

    
    Frame frm(time_id, str, img);
    frm.print_frame();
    frm.show_matrix("Matrix.txt");

    Frame frm2(time_id, str, hsvImage);
    frm2.print_frame();
    frm2.show_matrix("HSV_Matrix.txt");
    //frm.show_matrix2("Imagenes/Prueba0.jpeg");

    //Detect the boxes of the frame
    //frm.obtain_boxes();//TODO

    //Ver como podemos ver que de una imagen a otra esta el mismo objeto, para poder comparar los boxes correspondientes.
    //Obtener el nombre leyendo el frame con org de cada uno de los boxes
    //Obtner las dimensiones de las cajas
    //Comprobar si esa caja ya la tengo o es nueva y entonces añadir la info o crearla en cada caso
    //Sacamos el main box que será en cada caso el box de mayor tamaño
    //Comprobar primero con el main box y lugo con el resto el porcentaje que se ha agrandado



    //Convert back to opencv for display or saving.
    //No se que hace el MatView la verdad
    //Asi que voy a usar otras funciones para pasar la imagen a matrix
    //cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
    

    //
    
    std::cout << "OK";
    return 0;
}