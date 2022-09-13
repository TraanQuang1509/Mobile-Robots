#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define right_trigPin  11
#define right_echoPin  10
#define mid_trigPin  13
#define mid_echoPin  12
#define left_trigPin  5
#define left_echoPin  4
//Khai báo chân tín hiệu motor A
#define A_plus 1
#define A_minus 0 
 
//Khai báo chân tín hiệu cho motor B
#define B_plus 23
#define B_minus 22

int speed;
int dst = 30;
void forward(){
        pwmWrite(B_plus,speed* 1023/100); 
        pwmWrite(A_plus, speed*1023/100); 
}
void left(){
        pwmWrite(A_plus, 0);
        pwmWrite(B_plus, speed*1023/100);
}
void right(){
        pwmWrite(A_plus, speed*1023/100);
        pwmWrite(B_plus, 0);
}
void stop(){
	pwmWrite(B_plus, 0);
    	pwmWrite(A_plus, 0);
}
float cal_distance(int trigPin,int echoPin)
{
        float Duration;
        float Distance;
        float start, stop;
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        while(digitalRead(echoPin)==0){
        start = micros();
        }
        while(digitalRead(echoPin)==1){
        stop = micros();
        }
        Duration = (stop - start);
        Distance =Duration/58.1;
        return Distance;
}

int main(void){
    wiringPiSetup();
    MYSQL *conn, *conn1;
    MYSQL_RES *res;
    MYSQL_ROW row;

    pinMode(A_plus, PWM_OUTPUT);
    pinMode(A_minus, OUTPUT); 
    pinMode(B_plus, PWM_OUTPUT);
    pinMode(B_minus, OUTPUT);
    pinMode(left_trigPin, OUTPUT);
    pinMode(mid_trigPin, OUTPUT);
    pinMode(right_trigPin, OUTPUT);
    pinMode(left_echoPin, INPUT);
    pinMode(mid_echoPin, INPUT);
    pinMode(right_echoPin, INPUT);
    pwmWrite(B_plus, 0);
    pwmWrite(A_plus, 0);

    char *server = "localhost";
    char *user = "autocar";
    char *password = "123"; /* set me first */
    char *database = "autocar";
    while(1){
	float leftDistance, midDistance, rightDistance;
	leftDistance = cal_distance(left_trigPin, left_echoPin);
	delay(50);
	midDistance = cal_distance(mid_trigPin, mid_echoPin);
	delay(50);
	rightDistance = cal_distance(right_trigPin, right_echoPin);
	// Ket noi database lan 1
	conn1 = mysql_init(NULL);
        mysql_real_connect(conn1,server,user,password,database,0,NULL,0);
	char sql1[200];
        sprintf(sql1, "update sensor set left1 = %.2f, center1 = %.2f, right1 =%.2f where stt = 0", leftDistance, midDistance, rightDistance);
	mysql_query(conn1,sql1);
	mysql_close(conn1); 	
        // ket noi database
        conn = mysql_init(NULL);
        mysql_real_connect(conn,server,user,password,database,0,NULL,0); 
        // kiem tra cot isUpdated
        char sql[200];
	sprintf(sql, "select * from control");
        mysql_query(conn,sql);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        // NOTES: row la bien dang chuoi ky tu
        /*switch ('1')
    	{
    		case row[1]:
        		forward();
       	 		break;
    		case row[3]:
        		left();
        		break;
    		case row[4]:
        		right();
        		break; 
    		default:
   			stop();
        		break;
    	}*/
	speed = atoi(row[7]);
	if(atoi(row[1])==1){
		forward();
	}
	else if (atoi(row[3])==1 || atoi(row[6])==1){
		left();
	}
	else if (atoi(row[4])==1 || atoi(row[5]) == 1){
		right();
	}
	else if (atoi(row[8]) == 1){
		if(midDistance  < dst && leftDistance < dst && rightDistance < dst)
                {

                        right();
                        delay(50);
                }
                else if(midDistance < dst && leftDistance < dst && rightDistance > dst)
                {

                        right();
                        delay(50);
                }
		else if(midDistance < dst && leftDistance > dst && rightDistance < dst)
                {

                        left();
                        delay(50);
                }
                else if(midDistance > dst && leftDistance < dst && rightDistance < dst)
                {

                        forward();
                        delay(50);
                }
                else if(midDistance < dst && leftDistance > dst && rightDistance > dst)
                {

                        right();
                        delay(50);
                }
		else if(midDistance > dst && leftDistance < dst && rightDistance > dst )
                {

                        right();
                        delay(50);
                }
                else if(midDistance > dst && leftDistance > dst && rightDistance < dst)
                {

                        left();
                        delay(50);
                }
                else 
                {

                        forward();
                        delay(50);
                }


	}
	else {
		stop();
	}
	mysql_close(conn);
}

    return 0;

}

