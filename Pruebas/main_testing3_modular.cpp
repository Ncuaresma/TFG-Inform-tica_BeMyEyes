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


/*******************************************************************************************
    Function that detect the lines from a input picture converted to a matrix
********************************************************************************************/
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

    return lines;
}

/*******************************************************************************************
    Function that prints the lines in green in the picture
********************************************************************************************/
void print_lines(std::vector<cv::Vec6d> vector, cv::Mat input){
    // draw the result as big green linles:
 
    cv::imshow("Lineas", input);
    for (unsigned int i = 0; i < vector.size(); ++i)
    {
         cv::line(input, cv::Point(vector[i][2], vector[i][3]), cv::Point(vector[i][4], vector[i][5]), cv::Scalar(0, 255, 0), 5);
    }
    cv::imshow("Lineas relevantes", input);
    //cv::imwrite("Imagenes/coloredLines_mask_4.png", hueMask);
    //cv::imwrite("Imagenes/Testing/coloredLines_detection_4_SemiHorVert.png", input);
}

/*******************************************************************************************
    Function that prints the lines into a file
********************************************************************************************/
void print_in_file(std::ofstream& file, std::string n, std::vector<cv::Vec6d> lines){
    file.open(n);
    for (int i = 0; i < lines.size(); i++) {
        //Print the lines on a file
        //print only the lines that are horizontal(Compare if y1 == y2)
        file << lines[i] << endl;
    }
}

/*******************************************************************************************
    Function that tests if the lines are criss-crossed
********************************************************************************************/
bool mixed_lines(cv::Vec6d check_line, cv::Vec6d unique_line){
    if(check_line[1] == 1)
        return (((unique_line[3] <= check_line[3]) and (check_line[3] <= unique_line[5])) or
                    ((unique_line[3] <= check_line[5]) and (check_line[5] <= unique_line[5])) or 
                    ((unique_line[3] >= check_line[3]) and (check_line[3] >= unique_line[5])) or
                    ((unique_line[3] >= check_line[5]) and (check_line[5] >= unique_line[5])));
    else if(check_line[1] == 0)
        return (((unique_line[2] <= check_line[2]) and (check_line[2] <= unique_line[4])) or
                    ((unique_line[2] <= check_line[4]) and (check_line[4] <= unique_line[4])) or 
                    ((unique_line[2] >= check_line[2]) and (check_line[2] >= unique_line[4])) or
                    ((unique_line[2] >= check_line[4]) and (check_line[4] >= unique_line[4])));
    
    return false;
}

/*******************************************************************************************
    Function that joins two lines that are close
********************************************************************************************/
void join_lines(cv::Vec6d check_line, cv::Vec6d &unique_line, std::vector<double> coordenates, int &flag){
    unique_line[0]++;
    
    if(check_line[1] == 1){
        coordenates = {unique_line[3], unique_line[5], check_line[3], check_line[5]};

        double min_value = *std::min_element(coordenates.begin(), coordenates.end());
        double max_value = *std::max_element(coordenates.begin(), coordenates.end());
        //hacemos la linea más larga (la x mayor)
        if(unique_line[3] > min_value){
            unique_line[3] = min_value;
            flag = 1;
        }
        if(unique_line[5] < max_value){
            unique_line[5] = max_value;
            flag = 1;
        }
    }else if(check_line[1] == 0){
        coordenates = {unique_line[2], unique_line[4], check_line[2], check_line[4]};

        double min_value = *std::min_element(coordenates.begin(), coordenates.end());
        double max_value = *std::max_element(coordenates.begin(), coordenates.end());
        //hacemos la linea más larga (la x mayor)
        if(unique_line[2] > min_value){
            unique_line[2] = min_value;
            flag = 1;
        }
        if(unique_line[4] < max_value){
            unique_line[4] = max_value;
            flag = 1;
        }
    }
}

/*******************************************************************************************
    Function that tests if the lines are close from echa other
********************************************************************************************/
bool similar_lines(cv::Vec6d check_line, cv::Vec6d unique_line){
    if(check_line[1] == 1)
        return ((abs(check_line[2] - unique_line[2]) < 15) or (abs(check_line[4] - unique_line[4]) < 15));
    else if(check_line[1] == 0)
        return ((abs(check_line[3] - unique_line[3]) < 15) or (abs(check_line[5] - unique_line[5]) < 15));
    return false;
}

/*******************************************************************************************
    Function that detects the close lines and combines them to make the relevant line
********************************************************************************************/
cv::Vec6d combine_lines(int i, std::vector<cv::Vec6d> vertical_lines, std::vector<int> &irrelevant_lines, cv::Mat input){
    //identify the lines from the boxes in the image
    cv::Vec6d check_line, unique_line;
    //to get the lines close to the one we are checking 
    std::vector<int> close_lines ={};
    //to discart the irrelevant lines
    std::vector<int> irrelevant_close_lines = {};
    std::vector<double> coordenates ={};
    //flag
    int flag = 0;

    unique_line = vertical_lines[i];
    //Consideramos irrelevantes aquella linea que ya hemos unido a otra y por tanto ya se ha visitado
    for(int j = i+1; j < vertical_lines.size(); j++){
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), j)!= irrelevant_lines.end()) {
        }
        else{
            check_line = vertical_lines[j];
            //vemos que la y sea similar, es decir que difieran una de la otra en 10 pixels como mucho
            // y este una justo debajo a la otra con respecto a las x

            //Primero comprobamos si tienen x parecida
            if( similar_lines(check_line, unique_line) ){
                close_lines.push_back(j);
                //si además las y's están entrelazadas entonces se pueden unir
                 if( mixed_lines(check_line, unique_line) ){
                    join_lines(check_line, unique_line, coordenates, flag);
                    //esta linea ya esta compuesta en la otra
                    irrelevant_lines.push_back(j);
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
            if (std::find(irrelevant_close_lines.begin(), irrelevant_close_lines.end(), k)!= irrelevant_close_lines.end()) {
            }
            else{
                check_line = close_lines[k];
                //vemos que la y sea similar, es decir que difieran una d ela otra en 10 pixels como mucho
                // y este una justo debajo a la otra con respecto a las x

                //si además las y's están entrelazadas entonces se pueden unir
                 if( mixed_lines(check_line, unique_line) ){
                    join_lines(check_line, unique_line, coordenates, flag);
                    //esta linea ya esta compuesta en la otra
                    irrelevant_close_lines.push_back(k);
                }
            }
        }
    }
    return unique_line;
}

/*******************************************************************************************
    Function that gets the most relevant horizontal lines to detect each object of the picture
********************************************************************************************/
vector<cv::Vec6d> get_relevant_horizontal_lines(std::vector<cv::Vec6d> horizontal_lines, cv::Mat input){
    //identify the lines from the boxes in the image
    cv::Vec6d unique_line;
    //to discart the irrelevant lines
    std::vector<int> irrelevant_lines = {};
    //Relevant vertical lines (boxes lines)
    std::vector<cv::Vec6d> relevant_h_lines = {};

    //Identify the relevant vertical lines    
    for(int i = 0; i < horizontal_lines.size(); i++){
        //We will no check if we identified that the line is irrelevant<< i
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), i)!= irrelevant_lines.end()) {
        }
        else {

            unique_line = combine_lines(i, horizontal_lines, irrelevant_lines, input);

            //The red boxes that select the objects are 3 or more pixels thickness
            if(unique_line[0] > 3){
                //Initialized to 0 again for the next test
                unique_line[0] = 0;
                //Identified as a relevant horizontal line
                relevant_h_lines.push_back(unique_line);
            }
        }
    }

    //Imprimir la lineas en un fichero, que ya lo borraremos
    ofstream relevant_h_file;
    std::string name = "Relevant_horizontal_lines.txt";
    print_in_file(relevant_h_file, name, relevant_h_lines);
    return relevant_h_lines;

}

/*******************************************************************************************
    Function that gets the most relevant vertical lines to detect each object of the picture
********************************************************************************************/
vector<cv::Vec6d> get_relevant_vertical_lines(std::vector<cv::Vec6d> vertical_lines, cv::Mat input){
    //identify the lines from the boxes in the image
    cv::Vec6d unique_line;
    //to discart the irrelevant lines
    std::vector<int> irrelevant_lines = {};
    //Relevant vertical lines (boxes lines)
    std::vector<cv::Vec6d> relevant_v_lines = {};

    //Identify the relevant vertical lines    
    for(int i = 0; i < vertical_lines.size(); i++){
        //We will no check if we identified that the line is irrelevant<< i
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), i)!= irrelevant_lines.end()) {
        }
        else {

            unique_line = combine_lines(i, vertical_lines, irrelevant_lines, input);

            //The red boxes that select the objects are 3 or more pixels thickness
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

/********************************************************************************************************
    Function that identify the real boxes in the image and creates the objects box that are in the frame
*********************************************************************************************************/
void create_boxes(std::vector<cv::Vec6d> relevant_h_lines, std::vector<cv::Vec6d> relevant_v_lines, cv::Mat input, Frame &frm){
    //to discart the irrelevant lines
    std::vector<int> irrelevant_lines = {};
    int num_box = 0;
    std::vector<double> coordenates = {};//x1, y1, x2, y2
    std::vector<cv::Vec6d> box_lines = {};
    cv::Vec6d first_line, check_h_line, check_v1_line, check_v2_line;
    for(int i = 0; i < relevant_h_lines.size(); i++){
        if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), i)!= irrelevant_lines.end()) {
        }else{
            first_line = relevant_h_lines[i];
            for(int j = i+1; j<relevant_h_lines.size();j++){
                if (std::find(irrelevant_lines.begin(), irrelevant_lines.end(), j)!= irrelevant_lines.end()) {
                }else{
                    check_h_line = relevant_h_lines[j];
                    if((abs(check_h_line[2] - first_line[2]) < 30) and (abs(check_h_line[4] - first_line[4]) <  30)){
                        box_lines.push_back(first_line);
                        box_lines.push_back(check_h_line);
                        irrelevant_lines.push_back(j);
                        //cuidadoo que si no encuentra a su paralela tambien tien que entrar aquii
                        first_line[0] = 2;//2
                        check_h_line[0]++;
                        break;
                    }
                }
            }
            for (int k = 0; k < relevant_v_lines.size(); k++){
                check_v1_line = relevant_v_lines[k];
                //first_line[3] = first_line[5] as is horizontal y1=y2
                if(((abs(check_v1_line[3] - first_line[3]) < 30) or (abs(check_v1_line[5] - first_line[3]) <  30))
                    and ((abs(check_v1_line[2] - first_line[2]) < 30) or (abs(check_v1_line[2] - first_line[4]) <  30))){
                    
                    //si entra ya hay 3 lineas que componen la caja, nos es suficiente
                    if((first_line[0] == 2)and((abs(check_v1_line[3] - check_h_line[3]) < 30) or (abs(check_v1_line[5] - check_h_line[3]) <  30))){
                        first_line[0]++;//3
                        box_lines.push_back(check_v1_line);
                        coordenates.push_back(first_line[2]);//x1
                        coordenates.push_back(check_v1_line[3]);
                        coordenates.push_back(first_line[4]);
                        coordenates.push_back(check_v1_line[5]);
                        std:: cout<<"dos horizontales y una vertical"<<endl;
                        break;
                    }
                    //Solo ha identificado una linea vertical y una horizontal y ahora ve otra vertical posible
                    if(first_line[0] == 1){

                        if((abs(check_v1_line[3] - check_v2_line[3]) < 30) and (abs(check_v1_line[5] - check_v2_line[5]) <  30)){
                            first_line[0]= 3;//3
                            box_lines.push_back(first_line);
                            box_lines.push_back(check_v1_line);
                            coordenates.push_back(first_line[2]);//x1
                            coordenates.push_back(check_v1_line[3]);
                            coordenates.push_back(first_line[4]);
                            coordenates.push_back(check_v1_line[5]);
                            std:: cout<<"dos verticales y una horizontal"<<endl;
                            break;
                        }
                    }
                    first_line[0]++;//1
                    box_lines.push_back(check_v1_line);
                    //la anterior linea para el siguiente bucle
                    check_v2_line = relevant_v_lines[k];
                }
            }
            //consideramos si es un objeto box, y esto pasa si está formado al menos por 3 líneas
            
            if(first_line[0] == 3){
                print_lines(box_lines, input);
                Box box1(abs(first_line[4]-first_line[2]), abs(check_v1_line[5]-check_v1_line[3]), std::to_string(num_box), coordenates);
                frm.add_box_frame(box1);
                num_box++;
                //CREAMOS LA CAJA CON LAS COORDENADAS
            }
        }
    }
    std::cout<<"Hay "<< num_box<<" cajas en este frame"<< endl;
}

/*******************************************************************************************
    Function that identify the coordenades of the boxes that select the objects in the image
********************************************************************************************/
void obtain_boxes(std::vector<cv::Vec4i> lines, cv::Mat input, Frame &frm){
    //Creamos los ficheros donde vamos a escribir las lineas
    ofstream horizontal_file;
    ofstream vertical_file;
    /*Vectores de las lineas de 6 coordenadas, 
    [0] = se emplea para ver la relevancia de la linea
    [1] = Horizontal 0 vertical 1
    [2] = x1
    [3] = y1
    [4] = x2
    [5] = y2
    */
    //Horizontal lines with num of similar lines, lenght, points
    std::vector<cv::Vec6d> horizontal_lines;
    //Vertical lines with num of similar lines, lenght, points
    std::vector<cv::Vec6d> vertical_lines;

    horizontal_file.open ("Horizontal_lines.txt");
    vertical_file.open ("Vertical_lines.txt");

    // Seleccionamos las lineas semi horizontales y verticales
    for (int i = 0; i < lines.size(); i++) {
        //¡¡IMPORTANTE el vector lines es de 4 coordenadas!!
        //Print the lines on a file
        //print only the lines that are semi horizontal or vertical (Horizontal if y1 - y2 < 5, vertical x1 - x2 <5)
        if(abs(lines[i][1] - lines[i][3]) < 7) {
            horizontal_file << lines[i] << endl;
            horizontal_lines.push_back(Vec6d(0, 0, lines[i][0], lines[i][1],lines[i][2], lines[i][3]));
        }else if(abs(lines[i][0] - lines[i][2]) < 7) {
            vertical_file << lines[i] << endl;
            vertical_lines.push_back(Vec6d(0, 1, lines[i][0], lines[i][1],lines[i][2], lines[i][3]));
        }        
    }

    //check how many vertical and horizontal lines we have
    std::cout<<"Num horizontales: "<<horizontal_lines.size()<<endl;
    std::cout<<"Num verticales: "<<vertical_lines.size()<<endl;

    //----------------------------------------------------------HORIZONTAL LINES-------------------------------------------------------

    std::vector<cv::Vec6d> relevant_h_lines = get_relevant_horizontal_lines(horizontal_lines, input);
  
    // draw the result as big green lines:
    if(relevant_h_lines.size() != 0){
        //print_lines(relevant_h_lines, input);
        std::cout<<"Hay lineas horizontales relevantes ->"<<relevant_h_lines.size()<<endl;
    }else{       
        std::cout<<"No hay líneas horizontales ->"<<relevant_h_lines.size()<<endl;
    }
    //----------------------------------------------------------VERTICAL LINES-------------------------------------------------------

    std::vector<cv::Vec6d> relevant_v_lines = get_relevant_vertical_lines(vertical_lines, input);
  
    // draw the result as big green lines:
    if(relevant_v_lines.size() != 0){
        //print_lines(relevant_v_lines, input);
        std::cout<<"Hay lineas verticales relevantes ->"<<relevant_v_lines.size()<<endl;
    }else{       
        std::cout<<"No hay líneas verticales ->"<<relevant_v_lines.size()<<endl;
    }

    
    create_boxes(relevant_h_lines, relevant_v_lines, input, frm);
    
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

    cv::Mat input = cv::imread("Imagenes/Prueba5.jpeg");
        string str = "Prueba5";
        Frame frm(1, str, input);
        //Detection of the lines in a picture
        std::vector<cv::Vec4i> lines = detect_lines(input);

        cout << "Primero hay estas cajas"<<frm.get_num_boxes()<<endl;
        //Obtain the semi vertical and horizontal lines
        obtain_boxes(lines, input, frm);
        int num = frm.get_num_boxes();

        cout << "Luego hay estas cajas"<<num<<endl;
        vector <Box> boxes;
        for (int i = 0; i < num; i++){
            cout << "Caja num " << i << " con nombre " << frm.get_box_by_id(i).get_name() << endl;
        }


        
        cv::waitKey(0);
    return 0;
}
