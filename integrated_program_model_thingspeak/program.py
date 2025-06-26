import cv2
import numpy as np
from tensorflow.keras.applications.vgg16 import preprocess_input
from PIL import Image
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import matplotlib.pyplot as plt
import random
import requests
import os

model_path = 'predictWaste12.h5'
model = load_model(model_path)
print("Model loaded successfully.")

class_mapping = {
    "battery": "ewaste",
    "biological": "organic",
    "brown-glass": "glass",
    "cardboard": "paper",
    "clothes": "organic",
    "green-glass": "glass",
    "metal": "metal",
    "paper": "paper",
    "plastic": "plastic",
    "shoes": "organic",
    "trash": "organic",
    "white-glass": "glass"
}

output_class = ["battery", "biological", "brown-glass", "cardboard", "clothes", 
                 "green-glass", "metal", "paper", "plastic", "shoes", "trash", "white-glass"]

def waste_prediction(new_image):
    test_image = image.load_img(new_image, target_size=(224, 224))
    test_image = image.img_to_array(test_image) / 255.0
    test_image = np.expand_dims(test_image, axis=0)
    predicted_array = model.predict(test_image)
    predicted_class_index = np.argmax(predicted_array)
    predicted_value = output_class[predicted_class_index]
    predicted_accuracy = round(np.max(predicted_array) * 100, 2)
    merged_class = class_mapping.get(predicted_value, "Unknown")
    print(f"Predicted Class: {merged_class} with {predicted_accuracy}% accuracy.")
    return merged_class

folder_name = "captured_images"
if not os.path.exists(folder_name):
    os.makedirs(folder_name)

def start_video_part():
    while(1):
        cam = cv2.VideoCapture(0)  
        cv2.namedWindow("AutoSort Bin")   
        k=0
        while True:
            ret, frame = cam.read()   
            if not ret:
                print("Failed to grab frame")
                break
            cv2.imshow("AutoSort Bin", frame)  
            k = cv2.pollKey()
            if k == ord('q') or k == ord('Q'):
                break
            elif k == 32:  
                img_name = os.path.join(folder_name, f"{random.randint(0, 9999999)}.jpg")  
                cv2.imwrite(img_name, frame)
                break
        cam.release()
        cv2.destroyAllWindows()
        if k == ord('q') or k == ord('Q'):
            break
        image_path = img_name  
        class_names = ['ewaste', 'glass', 'metal', 'organic', 'paper', 'plastic']  
        predicted_class = waste_prediction(image_path)
        if predicted_class == 'ewaste':
            data = 1*100+random.randint(1,99)
        elif predicted_class == 'glass':
            data = 2*100+random.randint(1,99)
        elif predicted_class == 'metal':
            data = 3*100+random.randint(1,99)
        elif predicted_class == 'organic':
            data = 4*100+random.randint(1,99)
        elif predicted_class == 'paper':
            data = 5*100+random.randint(1,99)
        elif predicted_class == 'plastic':
            data = 6*100+random.randint(1,99)
        print("sending data to field 1")
        URL ='https://api.thingspeak.com/update?api_key=J807GJT1Y*******&field1=0'+str(data)
        response = requests.get(URL)
        print(response)
        print("sent data to field 1 : ",data)
        print("\n")

start_video_part()
