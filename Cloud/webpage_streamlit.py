# streamlit run webpage_streamlit.py
import streamlit as st
import os

MP4_DIR = "uploads_mp4"  # folder sa snimcima

st.title("Pregled snimaka sa Raspberry Pi")

# Uzmi sve mp4 fajlove iz foldera
mp4_files = [f for f in os.listdir(MP4_DIR) if f.endswith(".mp4")]

if not mp4_files:
    st.write("Nema snimaka u folderu.")
else:
    # Sortiraj fajlove po datumu kreiranja/modifikacije (od najstarijeg do najmladeg)
    mp4_files.sort(key=lambda f: os.path.getmtime(os.path.join(MP4_DIR, f)))

    # Drop-down lista
    selected_file = st.selectbox("Izaberi snimak za reprodukciju:", mp4_files)
    
    if selected_file:
        video_path = os.path.join(MP4_DIR, selected_file)
        st.video(video_path)  # reprodukcija video fajla

