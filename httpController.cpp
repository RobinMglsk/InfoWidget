
#include "httpController.h"



httpController::httpController(int i){
  
}

String httpController::getIndex(){
    String response = head;

    response += "<div class=\"container\"><h1>Custom message<h1><form action=\"/\" method=\"post\"><input class=\"u-full-width\" type=\"text\" name=\"title\" maxlength=\"14\" placeholder=\"Title\"/><input class=\"u-full-width\" type=\"text\" name=\"msg1\" maxlength=\"25\"/><input class=\"u-full-width\" type=\"text\" name=\"msg2\" maxlength=\"25\"/><button type=\"submit\" class=\"button-primary\">Submit</button></form></div>";

    return response+foot;
}


