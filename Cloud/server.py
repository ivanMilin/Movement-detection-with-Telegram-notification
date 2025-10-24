# uvicorn server:app --host 0.0.0.0 --port 5000 --reload
from fastapi import FastAPI, File, UploadFile, HTTPException
from fastapi.responses import JSONResponse
import os
import subprocess
import shutil
import requests
from datetime import datetime

app = FastAPI()

MP4_DIR = "uploads_mp4"
os.makedirs(MP4_DIR, exist_ok=True)

# ?? Telegram bot podaci
BOT_TOKEN = "XXXXXXXXXXXXXXXXXXXXXXXXXXX"
CHAT_ID = "XXXXXXXXXXXXXXXXXXXXXXXXXXX"

def send_telegram_message(message: str):
    url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage"
    data = {"chat_id": CHAT_ID, "text": message}
    try:
        response = requests.post(url, data=data)
        response.raise_for_status()
        print("Poruka poslata na Telegram.")
    except requests.exceptions.RequestException as e:
        print(f"Ne mogu poslati poruku na Telegram: {e}")

def convert_h264_to_mp4(h264_path: str) -> str:
    base_name = os.path.splitext(os.path.basename(h264_path))[0]
    mp4_path = os.path.join(MP4_DIR, f"{base_name}.mp4")

    cmd = [
        "ffmpeg",
        "-y",               # overwrite ako fajl postoji
        "-i", h264_path,    # ulazni fajl
        "-c:v", "copy",     # samo wrap, bez re-encodinga
        mp4_path
    ]
    subprocess.run(cmd, check=True)
    return mp4_path

@app.post("/upload/")
async def upload_file(file: UploadFile = File(...)):
    # privremeno sacuvaj .h264
    temp_path = os.path.join(MP4_DIR, file.filename)
    with open(temp_path, "wb") as buffer:
        shutil.copyfileobj(file.file, buffer)

    try:
        # konvertuj u mp4
        mp4_path = convert_h264_to_mp4(temp_path)
        os.remove(temp_path)  # obrisi originalni fajl
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"Konverzija nije uspela: {e}")

    # Posalji Telegram notifikaciju
    timestamp = datetime.now().strftime("%H:%M:%S")
    send_telegram_message(f"Dogodio se pokret u {timestamp}")

    return JSONResponse(content={
        "status": "success",
        "mp4_file": os.path.basename(mp4_path),
        "message": "Fajl uspesno uploadovan i sacuvan kao MP4"
    })
