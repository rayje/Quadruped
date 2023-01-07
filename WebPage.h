#include "Quadbot.h"
#include "Types.h"


class WebPage {

  private:

  String moveArmForm() {
    return "<h2>Move Arm</h2><form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveServo\">\
    <input type=\"text\" name=\"legIndex\" value=\"\">\
    <input type=\"text\" name=\"x\" value=\"\">\
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

  String moveArmRangeForm() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveArmRange\">\
    <input type=\"submit\" value=\"Move Arm Range\">\
    </form>";
  }

  String moveArmOneForm() {
    return "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/moveArmOne\">\
    <input type=\"submit\" value=\"Move Arm 1\">\
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

  String showLegMoved(double armPos[], Indicies idx) {
    String html = "<text>";
    int armIndex = idx.armIndex;
    int shoulderIndex = idx.shoulderIndex;
    
    html += "Arm " + String(armIndex) + " moved to " + String(armPos[armIndex]) + ", ";
    html += "Shoulder " + String(shoulderIndex) + " moved to " + String(armPos[shoulderIndex]) + "\n";
    html += "</text>";
    
    return html;
  }

  String showSetServoAngle() {
    return "<h2>Set Servo Angle</h2><form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/setServoAngle\">\
    <input type=\"text\" name=\"index\" value=\"5\">\
    <input type=\"text\" name=\"angle\" value=\"500\">\
    <input type=\"submit\" value=\"Set Servo Angle\">\
    </form>";
  }

  String showUpdatedServoAngle(ServoAngle angle) {
    return "<text>Arm " + String(angle.index) + " moved to " + String(angle.angle) + "</text>";
  }

  String basicBody() {
    String html = resetForm();
    html += moveArmRangeForm();
    html += moveArmForm();
    html += moveArmOneForm();

    return html;
  }

  public:

  WebPage() {
  }

  String getRootPage(Quadbot bot) {
    String html = "<html>";

    html += basicBody();
    html += showArmPos(bot.getArmPos());
    html += showSetServoAngle();

    html += "</html>";

    return html;
  }

  String handleServoMoved(Quadbot bot, Indicies idx) {
    String html = "<html>";

    html += basicBody();
    html += showArmPos(bot.getArmPos());
    html += showLegMoved(bot.getArmPos(), idx);
    html += showSetServoAngle();

    html += "</html>";

    return html;    
  }

  String handleSetServoAngle(Quadbot bot, ServoAngle angle) {
    String html = "<html>";

    html += basicBody();
    html += showArmPos(bot.getArmPos());
    html += showSetServoAngle();
    html += showUpdatedServoAngle(angle);

    html += "</html>";

    return html;    
  }
};
