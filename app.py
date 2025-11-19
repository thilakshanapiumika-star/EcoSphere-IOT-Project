from flask import Flask, send_file, jsonify
import pandas as pd
import os

app = Flask(__name__)
shared_dir = os.path.join(os.path.dirname(__file__), "shared")
 
@app.route("/insights")
def insights():
    df = pd.read_csv(os.path.join(shared_dir, "insights.csv"))
    return jsonify(df["Insight"].tolist())
 
@app.route("/humidity-trend-image")
def humidity_trend():
    return send_file(os.path.join(shared_dir, "humidity_trend.png"), mimetype="image/png")
 
@app.route("/correlation-image")
def correlation():
    return send_file(os.path.join(shared_dir, "correlation_heatmap.png"), mimetype="image/png")
 
@app.route("/hourly-patterns")
def hourly_patterns():
    df = pd.read_csv(os.path.join(shared_dir, "hourly_patterns.csv"))
    return df.to_json(orient="records")
 
@app.route("/anomalies")
def anomalies():
    df = pd.read_csv(os.path.join(shared_dir, "anomalies.csv"))
    return df.to_json(orient="records")

@app.route("/forecast")
def forecast():
    df = pd.read_csv(os.path.join(shared_dir, "forecast_two_weeks.csv"))
    df = df.rename(columns={df.columns[0]: "timestamp"})  # rename first col to 'timestamp'
    return df.to_json(orient="records")

@app.route("/hourly-humidity-patterns-image")
def hourly_humidity_patterns():
    return send_file(os.path.join(shared_dir, "Hourly_Humidity_Patterns.jpeg"), mimetype="image/jpeg")

@app.route("/humidity-anomalies-image")
def humidity_anomalies():
    return send_file(os.path.join(shared_dir, "Humidity_Anomalies.jpeg"), mimetype="image/jpeg")

@app.route("/temporal-trends-image")
def temporal_trends():
    return send_file(os.path.join(shared_dir, "Temporal_Trends.jpeg"), mimetype="image/jpeg")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)