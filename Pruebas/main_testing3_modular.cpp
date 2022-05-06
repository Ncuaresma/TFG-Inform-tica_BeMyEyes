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

//Line detection from a input picture
std::vector<cv::Vec4i> detect_lines(cv::Mat input){
    // convert to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(input, hsvImage, CV_BGR2HSV);

    // split the channels
    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);

    // hue channels tells you the color tone, if saturation and value aren't too low.

    // red color is a special case, because the hue space is circular and red is exactly at the beginning/end of the circle.
    // in literature, hue space goes from 0 to 360 degrees, but OpenCV rescales the range to 0 up to 180, because 360 does not fit in a single byte. Alternatively there is another mode where 0..360 is rescaled to 0..255 but this isn't as common.
    int hueValue = 0; // red color
    int hueRange = 1; // how much difference from the desired color we want to include to the result If you increase this value, for example a red color would detect some orange values, too.

    int minSaturation = 50; // I'm not sure which value is good here...
    int minValue = 50; // not sure whether 50 is a good min value here...

    cv::Mat hueImage = hsvChannels[0]; // [hue, saturation, value]

    // is the color within the lower hue range?
    cv::Mat hueMask;
    cv::inRange(hueImage, hueValue - hueRange, hueValue + hueRange, hueMask);

    // if the desired color is near the border of the hue space, check the other side too:
    // TODO: this won't work if "hueValue + hueRange > 180" - maybe use two different if-cases instead... with int lowerHueValue = hueValue - 180
    if (hueValue - hueRange < 0 || hueValue + hueRange > 180)
    {
        cv::Mat hueMaskUpper;
        int upperHueValue = hueValue + 180; // in reality this would be + 360 instead
        cv::inRange(hueImage, upperHueValue - hueRange, upperHueValue + hueRange, hueMaskUpper);

        // add this mask to the other one
        hueMask = hueMask | hueMaskUpper;
    }

    // now we have to filter out all the pixels where saturation and value do not fit the limits:
    cv::Mat saturationMask = hsvChannels[1] > minSaturation;
    cv::Mat valueMask = hsvChannels[2] > minValue;

    hueMask = (hueMask & saturationMask) & valueMask;

    cv::imshow("desired color", hueMask);

    // now perform the line detection
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(hueMask, lines, 1, CV_PI / 360, 50, 50, 10);

    std::cout<<"Num lineas: "<<lines.size()<<endl;

    return lines;
}

//Draw the lines in green in the picture
void print_lines(std::vector<cv::Vec6d> vector, cv::Mat input){
    // draw the result as big green linles:
    for (unsigned int i = 0; i < vector.size(); ++i)
    {
        cv::line(input, cv::Point(vector[i][0], vector[i][1]), cv::Point(vector[i][2], vector[i][3]), cv::Scalar(0, 255, 0), 5);
    }
    cv::imshow("input2", input);
    //cv::imwrite("Imagenes/coloredLines_mask_4.png", hueMask);
    //cv::imwrite("Imagenes/Testing/coloredLines_detection_4_SemiHorVert.png", input);
}
void print_in_file(std::ofstream& file, std::string n, std::vector<cv::Vec6d> lines){
    file.open(n);
    for (int i = 0; i < lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        file << lines[i] << endl;

    }
}
/*
combine_lines(std::vector<cv::Vec6d> vertical_lines, std::vector<int> irrelevant_lines){
    //Consideramos irrelevante aquella linea que ya hemos unido a otra y por tanto ya se ha visitado
    for(int j = i; j < vertical_lines.size(); j++){
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), j)!= irrelevant_lines.end()) {
            std::cout << "Irrelevant line" << j << endl;
        }
        else{
            std::cout << j<<endl;
            check_line = vertical_lines[j];
            //vemos que la y sea similar, es decir que difieran una d ela otra en 10 pixels como mucho
            // y este una justo debajo a la otra con respecto a las x

            //Primero comprobamos si tienen x parecida
            if( (abs(check_line[2] - unique_line[2]) < 15)){
                close_lines.push_back(j);
            
                //si además las y's están entrelazadas entonces se pueden unir
                 if( ( (unique_line[3] <= check_line[3]) and (check_line[3] <= unique_line[5]) or
                    (unique_line[3] <= check_line[5]) and (check_line[5] <= unique_line[5]) ) ){
                    unique_line[0]++;
                    std::cout << vertical_lines[j];
                    //hacemos la linea más larga (la x mayor)
                    if(check_line[3] < unique_line[3]){
                        unique_line[3] = check_line[3];
                        flag = 1;
                    }
                    if(check_line[5] > unique_line[5]){
                        unique_line[5] = check_line[5];
                        flag = 1;
                    }
                    //esta linea ya esta compuesta en la otra
                    irrelevant_lines.push_back(j);
                    std::cout << unique_line[0] << endl;
                }
            }
        }
    }
    return close_lines;
}
*/

vector<cv::Vec6d> get_relevant_horizontal_lines(std::vector<cv::Vec6d> horizontal_lines){

    //vamos a plantearlo como que si hay una linea justo debajo de esa aunque las x sean diferentes coges la x menor de x1 y mayor de x2
    //Relevant horizontal lines (boxes lines)
    std::vector<cv::Vec6d> relevant_h_lines;
    //identify the lines from the boxes in the image
    cv::Vec6d check_line, unique_line;
    //to discart the irrelevant lines
    std::vector<int> irrelevant_lines ={};
    //to get the lines close to the one we are checking 
    std::vector<int> close_lines ={};
    //to discart the irrelevant lines
    std::vector<int> irrelevant_close_lines = {};

    //Identify the relevant horizontal lines    
    for(int i = 0; i < horizontal_lines.size(); i++){
        unique_line = horizontal_lines[i];
        std::cout << horizontal_lines[i];
        //We will no check if we identified that the line is irrelevant
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), i)!= irrelevant_lines.end()) {
            std::cout << "Irrelevant line" << i << endl;
        }
        else {
            /*-----COMMENT DE REVISION---- 
            j=0 y no i+1, creo que mejor porque puede haber lineas pequeñitas que al principio no estén debajo peor al unirse a otras se vea que son la misma*/
            std::cout << "Checking line" << i << endl;
            for(int j = 0; j < horizontal_lines.size(); j++){
                check_line = horizontal_lines[j];
                //vemos que la y sea similar, es deciir que difieran una d ela otra en 10 pixels como mucho
                // y este una justo debajo a la otra con respecto a las x
                if( (abs(check_line[3] - unique_line[3]) < 10) and 
                    ( (unique_line[2] <= check_line[2]) and (check_line[2] <= unique_line[4]) or
                    (unique_line[2] <= check_line[4]) and (check_line[4] <= unique_line[4]) ) ){
                    unique_line[0]++;
                    std::cout << horizontal_lines[j];
                    //hacemos la linea más larga (la x mayor)
                    if(check_line[2] < unique_line[2]){
                        unique_line[2] = check_line[2];
                    }if(check_line[4] > unique_line[4]){
                        unique_line[4] = check_line[4];
                    }
                    //esta linea ya esta compuesta en la otra
                    irrelevant_lines.push_back(j);
                    std::cout << unique_line[0] << endl;
                }
            }
            //The red boxes that select te objects are 2 pixels thickness
            if(unique_line[0] > 3){                
                //Initialized to 0 again for the next test
                unique_line[0] = 0;
                //Identified as a relevant horizontal line
                relevant_h_lines.push_back(unique_line);
            }
        }
    }

    //Imprimir la lineas en un fichero, que ya lo borraremos o si no lo metemos en el bucle anterior
    ofstream relevant_h_file;
    relevant_h_file.open("Relevant_horizontal_lines.txt");
    std::cout<<relevant_h_lines.size();
    for (int i = 0; i < relevant_h_lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        relevant_h_file << relevant_h_lines[i] << endl;
    }
    return relevant_h_lines;
}

vector<cv::Vec6d> get_relevant_vertical_lines(std::vector<cv::Vec6d> vertical_lines, cv::Mat input){
//identify the lines from the boxes in the image
    cv::Vec6d check_line, unique_line;
    //to discart the irrelevant lines
    std::vector<int> irrelevant_lines ={};
    //to get the lines close to the one we are checking 
    std::vector<int> close_lines ={};
    //to discart the irrelevant lines
    std::vector<int> irrelevant_close_lines = {};
    //Relevant vertical lines (boxes lines)
    std::vector<cv::Vec6d> relevant_v_lines;
    //to discart the irrelevant lines
    irrelevant_lines ={};
    irrelevant_close_lines ={};
    close_lines = {};
    //flag
    int flag = 0;

    //Identify the relevant vertical lines    
    for(int i = 0; i < vertical_lines.size(); i++){
        unique_line = vertical_lines[i];
        close_lines = {};

        //We will no check if we identified that the line is irrelevant
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), i)!= irrelevant_lines.end()) {
            std::cout << "Irrelevant line" << i << endl;
        }
        else {
            std::cout << "Checking line" << i << endl;

            //////////////////////////////////////////////////////////////////////////////////////////////
            //close_lines = combine_lines();
            ///////////////////////////////////////////////////////////////////////////////////////////////


            //Consideramos irrelevante aquella linea que ya hemos unido a otra y por tanto ya se ha visitado
            for(int j = i; j < vertical_lines.size(); j++){
                if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), j)!= irrelevant_lines.end()) {
                    std::cout << "Irrelevant line" << j << endl;
                }
                else{
                    std::cout << j<<endl;
                    check_line = vertical_lines[j];
                    //vemos que la y sea similar, es decir que difieran una d ela otra en 10 pixels como mucho
                    // y este una justo debajo a la otra con respecto a las x

                    //Primero comprobamos si tienen x parecida
                    if( (abs(check_line[2] - unique_line[2]) < 15)){
                        close_lines.push_back(j);
                    
                        //si además las y's están entrelazadas entonces se pueden unir
                         if( ( (unique_line[3] <= check_line[3]) and (check_line[3] <= unique_line[5]) or
                            (unique_line[3] <= check_line[5]) and (check_line[5] <= unique_line[5]) ) ){
                            unique_line[0]++;
                            std::cout << vertical_lines[j];
                            //hacemos la linea más larga (la x mayor)
                            if(check_line[3] < unique_line[3]){
                                unique_line[3] = check_line[3];
                                flag = 1;
                            }
                            if(check_line[5] > unique_line[5]){
                                unique_line[5] = check_line[5];
                                flag = 1;
                            }
                            //esta linea ya esta compuesta en la otra
                            irrelevant_lines.push_back(j);
                            std::cout << unique_line[0] << endl;
                        }
                    }
                }
            }
            //vemos si al haber alargado las lineas ahora coincide con otra
            //to discart the irrelevant lines
            irrelevant_close_lines = {}; 
            while(flag == 1){
                flag = 0;
                for(int k = 0; k < close_lines.size(); k++){

                    cv::line(input, cv::Point(vertical_lines[close_lines[k]][2], vertical_lines[close_lines[k]][3]), cv::Point(vertical_lines[close_lines[k]][4], vertical_lines[close_lines[k]][5]), cv::Scalar(0, 255, 0), 5);


                    if (std::find(irrelevant_close_lines.begin(), irrelevant_close_lines.end(), k)!= irrelevant_close_lines.end()) {
                        std::cout << "Irrelevant line" << k << endl;
                    }
                    else{
                        std::cout << k <<endl;
                        check_line = close_lines[k];
                        //vemos que la y sea similar, es decir que difieran una d ela otra en 10 pixels como mucho
                        // y este una justo debajo a la otra con respecto a las x

                        //si además las y's están entrelazadas entonces se pueden unir
                         if( ( (unique_line[3] <= check_line[3]) and (check_line[3] <= unique_line[5]) or
                            (unique_line[3] <= check_line[5]) and (check_line[5] <= unique_line[5]) ) ){
                            unique_line[0]++;
                            std::cout << close_lines[k];
                            //hacemos la linea más larga (la x mayor)
                            if(check_line[3] <= unique_line[3]){
                                unique_line[3] = check_line[3];
                                flag = 1;
                            }
                            if(check_line[5] >= unique_line[5]){
                                unique_line[5] = check_line[5];
                                flag = 1;
                            }
                            //esta linea ya esta compuesta en la otra
                            irrelevant_close_lines.push_back(k);
                            std::cout << unique_line[0] << endl;
                        }
                    }
                }
            }   
            //The red boxes that select the objects are 4 or more pixels thickness
            if(unique_line[0] > 3){
                //Initialized to 0 again for the next test
                unique_line[0] = 0;
                //Identified as a relevant horizontal line
                relevant_v_lines.push_back(unique_line);
            }
        }
    }

    //Imprimir la lineas en un fichero, que ya lo borraremos
    ofstream relevant_v_file;
    std::string name = "Relevant_vertical_lines.txt";
    print_in_file(relevant_v_file, name, relevant_v_lines);

    return relevant_v_lines;
}

void obtain_boxes(std::vector<cv::Vec4i> lines, cv::Mat input){
    //Creamos los ficheros donde vamos a escribir las lineas
    ofstream horizontal_file;
    ofstream vertical_file;
    //Horizontal lines with num of similar lines, lenght, points
    std::vector<cv::Vec6d> horizontal_lines;
    //Vertical lines with num of similar lines, lenght, points
    std::vector<cv::Vec6d> vertical_lines;

    horizontal_file.open ("Horizontal_lines.txt");
    vertical_file.open ("Vertical_lines.txt");

    // Seleccionamos las lineas semi horizontales y verticales
    for (int i = 0; i < lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        if(abs(lines[i][1] - lines[i][3]) < 5) {
            horizontal_file << lines[i] << endl;
            horizontal_lines.push_back(Vec6d(0, lines[i][2] - lines[i][0], lines[i][0], lines[i][1],lines[i][2], lines[i][3]));
        }else if(abs(lines[i][0] - lines[i][2]) < 5) {
            vertical_file << lines[i] << endl;
            vertical_lines.push_back(Vec6d(0, lines[i][2] - lines[i][0], lines[i][0], lines[i][1],lines[i][2], lines[i][3]));
        }        
    }

    //check how many vertical and horizontal lines we have
    std::cout<<"Num horizontales: "<<horizontal_lines.size()<<endl;
    std::cout<<"Num verticales: "<<vertical_lines.size()<<endl;

 /*   // draw the result as big green lines:
    print_lines(horizontal_lines, input);
    print_lines(vertical_lines, input);
*/

    //----------------------------------------------------------HORIZONTAL LINES-------------------------------------------------------
   //Relevant horizontal lines (boxes lines)

    std::vector<cv::Vec6d> relevant_h_lines = get_relevant_horizontal_lines(horizontal_lines);

    //print_lines(relevant_h_lines, input);

    //----------------------------------------------------------VERTICAL LINES-------------------------------------------------------
    
    std::vector<cv::Vec6d> relevant_v_lines = get_relevant_vertical_lines(vertical_lines, input);
    //pintar las lineas     
    // draw the result as big green lines:
    print_lines(relevant_v_lines, input);
    

    /*
    //--------------------------------------------------------Horizontales y verticales-----------------------------------------------
    // --------------------------------------------Muy ineficiente-------------------------------------------
    //comprobar de las relavantes horizontales y verticales cuales son las que finalmente forman el cuadrado
    //Relevant lines (boxes lines)
    std::vector<cv::Vec6d> relevant_h2_lines;
    std::vector<cv::Vec6d> relevant_v2_lines;
    //to discart the irrelevant lines
    irrelevant_lines ={};
    std::cout << relevant_h_lines.size() << endl;
    std::cout << relevant_v_lines.size() << endl;

    //Identify the relevant vertical lines    
    for(int i = 0; i < relevant_h_lines.size(); i++){
        unique_line = relevant_h_lines[i];
        std::cout << "Checking line" << i << endl;
        for(int j = 0; j < relevant_v_lines.size(); j++){
            check_line = vertical_lines[j];
            std::cout << "Checking vertical line" << j << endl;
            if((abs(check_line[2] - unique_line[2]) < 50 and (abs(check_line[3] - unique_line[3])< 50  or abs(check_line[5] - unique_line[5])< 50 ))
             or (abs(check_line[4] - unique_line[4]) <  50)and (abs(check_line[3] - unique_line[3])< 50  or abs(check_line[5] - unique_line[5])< 50 )){
                unique_line[0]++;
                check_line[0]++;
                std::cout << unique_line[0] << endl;
                std::cout << check_line[0] << endl;
            }
            if(i == relevant_h_lines.size()){
                //Each vertical line has to mach with two horizontal lines
                if(check_line[0] > 1 or check_line[2] == 0 or check_line[3]==0 or check_line[4]==0 or check_line[5]==0){
                    //Identified as a relevant horizontal line
                    relevant_v2_lines.push_back(check_line);
                }
            }
        }
        //Each horizontal line has to mach with two vertical lines
        //---------------------Cuidado en estas comprobaciones que si alguna coordenada es 0 ya no hay linea que las una
        if(unique_line[0] > 1 or unique_line[2] == 0 or unique_line[3]==0 or unique_line[4]==0 or unique_line[5]==0){
            //Identified as a relevant horizontal line
            relevant_h2_lines.push_back(unique_line);
        }
        
    }
    //--------------------------//Algo sale mal porque pone que ninguna es relevante
    ofstream relevant_v2_file;
    relevant_v2_file.open("Relevant_vertical2_lines.txt");
    std::cout<<relevant_v2_lines.size();
    for (int i = 0; i < relevant_v2_lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        relevant_v2_file << relevant_v2_lines[i] << endl;
    }
    ofstream relevant_h2_file;
    relevant_h2_file.open("Relevant_horizontal2_lines.txt");
    std::cout<<relevant_h2_lines.size();
    for (int i = 0; i < relevant_h2_lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        relevant_h2_file << relevant_h2_lines[i] << endl;
    }*/

    //print the lines
   //print_lines(lines, input);
    
}

int main(int argc, char* argv[]){
    cv::Mat input = cv::imread("Imagenes/Prueba5.jpeg");//----------------ALGO PASA CON PRUEBA1.jpeg

    //Detection of the lines in a picture
    std::vector<cv::Vec4i> lines = detect_lines(input);

    //Obtain the semi vertical and horizontal lines
    obtain_boxes(lines, input);

    
    cv::waitKey(0);
    return 0;
}
