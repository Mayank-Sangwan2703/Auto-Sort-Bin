# ♻️ AutoSortBin: An IoT and Deep Learning Based Smart Waste Segregation System

AutoSortBin is a smart bin system combining **IoT**, **Computer Vision**, and **Cloud Monitoring** to automatically segregate waste into six categories: **Paper, Glass, Metal, Plastic, Organic, and E-Waste**. It uses a custom-trained **DenseNet-121 CNN model**, an **ESP32-based simulated circuit on Wokwi**, and **ThingSpeak** for real-time monitoring and bin-full alerts.

🎓 This work was presented in the research paper:  
**_"AutoSortBin: Integrating CPS and IoT with Densely Connected Convolutional Networks for Sustainable Waste Management"_**

---

## 📁 Folder Structure

AutoSortBin/
├── wokwi-circuit/ # ESP32 Wokwi simulation (main.cpp, diagram.json)
├── model-part/ # CNN model, training scripts, classification logic
├── thingspeak-code/ # Code for ESP32 + ThingSpeak API integration
├── screenshots/ # LCD display and circuit output examples
└── README.md


---

## 🚀 Features

- ✅ **Deep Learning Classification**  
  - DenseNet-121 used for high-accuracy waste classification  
  - 976 custom-labeled images across 6 classes: Paper, Glass, Metal, Plastic, Organic, E-Waste  

- 🌐 **IoT Cloud Monitoring**  
  - Live status updates on bin fullness using ThingSpeak  
  - Automatic updates of bin levels and type detection  

- 🤖 **Smart Automation**  
  - Servo motors open the correct bin lid based on classification  
  - LCD shows the detected waste type and status  

- 💡 **Wokwi Simulation**  
  - Complete ESP32 circuit simulated with ultrasonic sensors, LCD, and servo motors  

- 🔔 **Bin Full Alerts**  
  - If a bin is full (detected via ultrasonic sensors), the lid won’t open  
  - Alert sent to ThingSpeak with timestamp

---

## 📸 Dataset

📁 **Dataset (Google Drive):**  
[🔗 AutoSortBin Image Dataset](https://drive.google.com/drive/folders/1Bq1_ukUqxVlyDr2GdsxmJd0NrJQ-x3Mm?usp=sharing)

---

## 📊 Experimental Results

| Metric              | Value   |
|---------------------|---------|
| Training Accuracy   | 86.56%  |
| Testing Accuracy    | 85.68%  |
| Validation Accuracy | 84.83%  |
| Precision           | 87%     |
| Recall              | 80%     |
| F1 Score            | 83%     |

---

## 🛠️ Installation & Setup

### 🔧 Clone This Repository

```bash
git clone https://github.com/Avin-Chaudhary/AutoSortBin.git
cd AutoSortBin
📍 Install Model Dependencies
bash
Copy
Edit
pip install tensorflow keras numpy pillow matplotlib opencv-python
🔌 Circuit Simulation on Wokwi
Open Wokwi

Upload files from wokwi-circuit/

Connect: ESP32 + Servo + LCD + Ultrasonic Sensor (as per diagram.json)

☁️ ThingSpeak Setup
Sign up on ThingSpeak

Create a channel and generate your Write API Key

Replace the key in the code inside thingspeak-code/esp32-thingspeak.ino
▶️ How to Run
Start the system via Wokwi or real hardware

Place waste object under camera (or run model script with sample images)

Model classifies the waste type

Corresponding bin opens via servo

LCD shows category and level

If bin is full → no open + cloud alert

ThingSpeak dashboard updates live
🌱 Future Scope
Add GSM module for SMS alerts

Use moisture, IR, or odor sensors for precision

Solar-powered design for green energy

Real-world prototype deployment with multi-bin hardware

Dashboard with analytical charts & Firebase/Google Cloud support

🙌 Contributing
All contributions are welcome!
Please fork the repo, add your features or fixes, and submit a pull request.
👨‍💻 Author
Avin Chaudhary
3rd Year Tech Enthusiast | Frontend Developer
National Institute of Technology, Kurukshetra (IT '27)
🔗 LinkedIn | GitHub

⚠️ License & Disclaimer
This repository is for educational and research purposes only.
It does not include a license. Please do not reuse for commercial use without permission.
