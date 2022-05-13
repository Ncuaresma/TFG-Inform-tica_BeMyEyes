/* Program in c++
* 
* Module Box 
*
* Compilar: g++ -o exe image_mod.cc
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
#include <cstring>


class Box {

   private:
    int id;
    vector<double> length; // array of lengths of the same box from different frames
    vector<double> height; // array of heights of the same box from different frames
    vector<double> area; // array of areas of the same box from different frames
    std::vector<cv::Vec6d> lines = {};
    std::vector<double> coordenates = {};//x1, y1, x2, y2
    //vector <double> growing; // percentage of the growing from teh box1 to the same box1 of the next frame
    // estas dos de abajo son posibbles manera de identificar cada box para poder comparar las correspondientes de un frame u otro
    //ESTA NO SIRVE PARA NADA vector<double> position; // para ubicar la caja y poder comprobar que es la misma de un frame a otro
    string name; // con org leer en el frame
    //color de dentro de forma proporcional!! no vale 

   public:

    // function to initialize private variables
    Box(double len, double hgt, std::string n, vector<double> coord){
        // add the new information of the box to the vector
        length.push_back(len);
        height.push_back(hgt);
        area.push_back(len * hgt);
        name = n;
        copy(coord.begin(),coord.end(), coordenates.begin());
        //tambien habra que añadir que area seleccionamos para coger el color
        //y el color que tiene en ese arrea
        // lo mismo lo mejor seria un array de matrices y guardar directamente la matriz con la que comparar
        //un array por cada area seleccionada de la foto
    }

    void add_box(double len, double hgt){
        height.push_back(hgt);
        length.push_back(len);
        area.push_back(len * hgt);
    }
    // function to set or change each value of the box (we don't need it)
    void add_length(double len){
        length.push_back(len);
    }

    void add_height(double hgt){
        height.push_back(hgt);
        //area = this->length * hgt;
    }



   /* void set_position(double pos){
        position = pos;
    }*/

    // functions to get each value
    double get_length(){
        int size = length.size();
        return length.at(size);
    }

    double get_height() {
        int size = height.size();
        return height.at(size);
    }
    string get_name() {
        return name;
    }

    /*double get_position(){
        return position;
    }*/

    
    double calculate_area() {
        int size = area.size();
        return area.at(size);
    }

    // function to compare two boxes
    double size_difference(Box box1, Box box2) {
        if((box1.get_height() < box2.get_height())&&(box1.get_length() < box2.get_length())){
            return box2.calculate_area() - box1.calculate_area();
        }
        return 0;
    }


/*   
    Primero tengo que crear la clase Screen o frame(imagen que contiene las boxes) o ver como hago para comparar con el tamaño de la pantalla

    // function to detect a big box so the object could be so close
    int isReallyBig(Box box, Frame frame){
        if(((screen.get_height() - box.get_height())< 0.2)||((screen.get_length() - box.get_length())< 0.2)){
            return 1;
        }
        return 0;
    }
*/

};

/*int main() {
    string str = "Primera_caja";
    // create object of Box class
    // pass the values of private variables as arguments
    Box box1(30.8, 19.2, str);

    std::cout << "Name of Box " << box1.get_name();

    return 0;
}
*/
