/* Program in c++
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

class Box {

   private:
    int id;
    double length; // array of lengths of the same box from different frames
    double height; // array of heights of the same box from different frames
    double area; // array of areas of the same box from different frames
    //double* growing; // percentage of the growing from teh box1 to the same box1 of the next frame
    // estas dos de abajo son posibbles manera de identificar cada box para poder comparar las correspondientes de un frame u otro
    double position; // para ubicar la caja y poder comprobar que es la misma de un frame a otro
    string name; // con org leer en el frame

   public:

    // function to initialize private variables
    void ini_box(double len, double hgt, double pos) {
        length = len;
        height = hgt;
        area = len * hgt;
        position = pos; 
    }
    // function to set or change each value of the box (we don't need it)
    void set_length(double len){
        length = len;
        area = len * this->height;
    }

    void set_height(double hgt){
        height = hgt;
        area = this->length * hgt;
    }

    void set_position(double pos){
        position = pos;
    }

    // functions to get each value
    double get_length(){
        return length;
    }

    double get_height(){
        return height;
    }

    double get_position(){
        return position;
    }

    double calculate_area() {
        return area;
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

int main() {
    // create object of Box class
    Box box1;

    // pass the values of private variables as arguments
    box1.ini_box(30.8, 19.2, 1.2);

    std::cout << "Area of Box =  " << box1.calculate_area()<<std::endl;

    return 0;
}

