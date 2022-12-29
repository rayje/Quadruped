#include "Quadbot.h"


class WebPage {

  private:

  String moveArmForm() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveServo\">\
    <input type=\"text\" name=\"legIndex\" value=\"\">\
    <input type=\"text\" name=\"x\" value=\"\">\
    <input type=\"text\" name=\"y\" value=\"\">\
    <input type=\"text\" name=\"z\" value=\"\">\
    <input type=\"submit\" value=\"Move\">\
    </form>";
  }

  String moveMinMaxForms() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveMax\">\
    <input type=\"submit\" value=\"Move Max\">\
    </form>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveMin\">\
    <input type=\"submit\" value=\"Move Min\">\
    </form>";
  }

  String moveLegMinMaxForms() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveLegMax\">\
    <input type=\"submit\" value=\"Move Leg Max\">\
    </form>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveLegMin\">\
    <input type=\"submit\" value=\"Move Leg Min\">\
    </form>";
  }

  String moveForwardForm() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveForward\">\
    <input type=\"text\" name=\"steps\" value=\"5\">\
    <input type=\"text\" name=\"delay\" value=\"500\">\
    <input type=\"submit\" value=\"Forward\">\
    </form>";
  }

  String resetForm() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/reset\">\
    <input type=\"submit\" value=\"Reset\">\
    </form>";
  }

  String showArmPos(double armPos[]) {
    String html = "<ol>";
    for (int i = 0; i < numberOfServos; i++) {
      html += "<li>" + String(armPos[i]) + "</li>";
    }
    html += "</ol>";

    return html;
  }


  public:

  WebPage() {

  }

  String getRootPage(Quadbot bot) {
    String html = "<html>";

    html += resetForm();
    html += moveArmForm();
    html += showArmPos(bot.getArmPos());

    html += "</html>";

    return html;
  }

};