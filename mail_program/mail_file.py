import time
import smtplib
import requests
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

CHANNEL_ID = "2695***"
FIELD_NUMBER = 2
READ_API_KEY = "CA43L8URVM5*****"

def check_bin_full(receiver_mail_id):
    while(1):
        time.sleep(20)
        url = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/fields/{FIELD_NUMBER}/last.json?api_key={READ_API_KEY}"
        latest_value = 0
        try:
            response = requests.get(url)
            data = response.json()
            latest_value = data.get(f"field{FIELD_NUMBER}", "No data")
        except Exception as e:
            print("Error fetching data:", e)

        latest_value = int(latest_value)

        if(latest_value != 0):
            sender_email = "autosortbin@gmail.com"
            app_password = "dvic fdyt **** ****"
            receiver_email = receiver_mail_id

            categ_name = "ewaste"

            if(latest_value == 2):
                categ_name = "glass"
            elif(latest_value == 3):
                categ_name = "metal"
            elif(latest_value == 4):
                categ_name = "organic"
            elif(latest_value == 5):
                categ_name = "paper"
            elif(latest_value == 6):
                categ_name = "plastic"

            subject = f"Bin Full Alert: {categ_name} Bin"
            body = f"The {categ_name} bin is full and requires emptying.\n\nLocation: Wokwi\nTime: {time.strftime('%H:%M:%S', time.localtime())}"

            msg = MIMEMultipart()
            msg["From"] = sender_email
            msg["To"] = receiver_email
            msg["Subject"] = subject
            msg.attach(MIMEText(body, "plain"))

            try:
                server = smtplib.SMTP('smtp.gmail.com', 587)
                server.starttls()
                server.login(sender_email, app_password)
                server.sendmail(sender_email, receiver_email, msg.as_string())
                print("Email sent successfully!")
            except Exception as e:
                print(f"Error: {e}")
            finally:
                server.quit()

            print("sending data to field 2")
            URL ='https://api.thingspeak.com/update?api_key=J807GJT1YDO*****&field2=0'+"0"
            response = requests.get(URL)
            print(response)
            print("sent 0 to field 2")

receiver_mail_id = input("enter the email id where you want to receive email : ")
check_bin_full(receiver_mail_id)
