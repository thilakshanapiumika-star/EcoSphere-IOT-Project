require("dotenv").config();
const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const app = express();
app.use(express.json()); // Middleware for parsing JSON
app.use(cors());

// MongoDB Connection
const mongoURI = "mongodb+srv://thilakshanapiumika:yMt3CfdGuElIYAxb@sensordht.zhpwycr.mongodb.net/?retryWrites=true&w=majority&appName=sensorDHT";
mongoose.connect(mongoURI, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then(() => console.log("Connected to MongoDB Atlas"))
.catch(err => console.error("MongoDB Connection Error:", err));

// Schema for storing sensor data
const SensorDataSchema = new mongoose.Schema({
    DHT11_Temp: Number,
    DHT11_Hum: Number,
    DHT22_Temp: Number,
    DHT22_Hum: Number,
    timestamp: { type: Date, default: Date.now }
});

const SensorData = mongoose.model("SensorData", SensorDataSchema);

// API Endpoint to Receive Data from ESP32
app.post("/sensor-data", async (req, res) => {
    try {
        const { DHT11_Temp, DHT11_Hum, DHT22_Temp, DHT22_Hum } = req.body;
        const newData = new SensorData({ DHT11_Temp, DHT11_Hum, DHT22_Temp, DHT22_Hum });
        await newData.save();
        res.status(200).send("Data saved to MongoDB");
    } catch (error) {
        console.error("Error saving data:", error);
        res.status(500).send("Error saving data");
    }
});

// Start Server on Port 5000
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
