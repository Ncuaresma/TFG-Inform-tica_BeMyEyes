/* Program in c++
* 
* Module Frame
*
* Compilar: g++ -o exe frame_mod.cc
* Se genera el exe
* Ejecutar: ./exe
*
* Va a recibir esto del codigo externo: output_frame_mat
*
*
*/

 //Lo que queremos hacer es coger esos frames y ver que tienen, que es lo comentado a continuacion

      // Use CLASSES (Object-oriented programming)
      // SAVE THE NEW FRAME TO COMPARE WITH THE PREVIOUS ONE/s
        // Maybe COMPARE FRAMES 5 BY 5 (not between consecutive ones as
        // there is little change between then)

          // Maybe COMPARE FRAME with the previous, with the 2d previous...
          // Till the 5th, the 10th... To determine with more accuracy
          // the change in one box



      // HAVE TO IDENTIFY THE BOXES ON THE FRAME!!! How many...


      // STORE ONLY THE RED BOXES (PREV_BOX AND NOW_BOX)
        // Maybe class Box with height and length of the box, num_frame, ...
        // and some methods as COMPARE_BOXES!!!, getLength()...


      // COMPARE PREV_BOX WITH NEW_BOX
      // Measure the difference in the size of the PREV_BOX AND NOW_BOX (size_difference)


      // Translate that difference in size ((size_difference) to STATE

        // STATE: MOVING_FORWARD, MOVING_BACKWARD, NOT_MOVING
        // If (MOVING_FORWARD)
          // if (size_difference > 0.2)
              // VIBRATE_LOW
          // if (size_difference > 0.3)
              // VIBRATE_MEDIUM
          // if (size_difference > 0.4 || isReallyBig(NOW_BOX))
              // VIBRATE_HIGH
      //sleep(2);
      //LOG(INFO) << "entro"; 
      //Entra aqui como 9 veces por segundo es decir se guardan 9 fotos por segundo (jeje pero porque puse un sleep, si no lo hara más)
  //    cv::imshow(kWindowName, output_frame_mat);
      //LOG(INFO) << output_frame_mat;
      //sleep(5);
      // Press any key to exit.
  //    const int pressed_key = cv::waitKey(5);
  //    if (pressed_key >= 0 && pressed_key != 255) grab_frames = false;


/// los mismos boxes en frames disntintos esto es lo mas complejo
#include <iostream>
#include <string>
using namespace std;
#include <vector>
#include <fstream>

#include "box_mod.h"
#include <opencv2/opencv.hpp>
#include <sstream>

/*#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"*/

class Frame {

   private:
    int id;
    //imagen en si tambien tendra que estar
    cv::Mat image_mat;
    double length; // length of the frame
    double height; // height of the frame
    std::string name;
    vector<Box> boxes; // array of areas of the same box from different frames
    Box mainBox; 

   public:

    // function to initialize private variables
    //void ini_frame(cv::Mat image_from_camera, double len, double hgt) {
    //void ini_frame(cv::Mat image_from_camera) {
    //    this.output_frame_mat = image_from_camera;      // Maybe need to copy the matrix??
        // Maybe we can obtain length and heigth from matrix size
        //length = len;
        //height = hgt;
    //}

    Frame(int numero, std::string n, cv::Mat image) : mainBox(image.rows,image.cols,n){
        //Obtener las dimensiones de la imagen
        id = numero;
        name = n;
        length = image.cols;;
        height = image.rows;
        image_mat = image;
    }

    // function to set or change each value of the frame (we don't need it)
    void set_length(double len){
        length = len;
    }

    void set_height(double hgt){
        height = hgt;
    }

    // functions to get each value
    double get_length(){
        return length;
    }

    double get_height(){
        return height;
    }

    void print_frame(){
        std::cout << this->name << endl;
        std::cout << this->length << endl;
        std::cout << this->height << endl;
        std::cout << this->id << endl;
    }

    void show_image() {
        cv::imshow("Display frame", this->image_mat);
    }

    void show_matrix(string txt) {
     /*   for(int i = 0; i < this->height; i++)
        {
            for(int j = 0; j < this->length; j++)
                std::cout << image_mat[this->height * j + i ];
        std::cout << endl;
        }*/
        ofstream matrix_file;
        matrix_file.open (txt);
        for (int i = 0; i < height; i++){
            cv::Mat line = image_mat.row(i);
            //cout << line;
            matrix_file << line << endl;
        }


        //matrix_file << this->image_mat << endl << endl;
        matrix_file.close();
    
    }
    /*
    void show_matrix2(string image){
        ofstream matrix_file;
        matrix_file.open("Matrix2.txt");
        std::ifstream file(image);
        std::string str; 
        while (std::getline(file, str)) {
          matrix_file << str << endl;
        }
    }
    */

    // obtain the boxes in the frame
    void obtain_boxes(){
        // find the pixels in red = 
        int num_boxes=0;
        for(int i = 0; i < this->height; i++)
        {
            for(int j = 0; j < this->length; j++)
                std::cout << this->image_mat; //image_mat[this->height * j + i ];
        }
    }
    // check if the detected box is one of the boxes detected in the previous 3 frames
    

};
    

/*int main(int argc,char **argv) {
    //string image_path = "/home/nuria/TFG_Informatica/Pruebas/Imagenes/Image0.jpeg";
    string str = "nuria";
    
    //cv::Mat output_frame_mat = cv::imread(argv[1], cv::IMREAD_COLOR);
    //cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);


    // create object of Frame class




    Frame frm(8, 30.8, 19.2, str);
    
    // Convert back to opencv for display or saving.
    //Noo se que hace el MatView la verdad
    //Asi que voy a usar otras funciones para pasar la imagen a matrix
    //cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
    

    //
    
    std::cout << "OK";
    return 0;
}*/
