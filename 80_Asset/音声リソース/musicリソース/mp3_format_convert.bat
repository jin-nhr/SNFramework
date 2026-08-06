mkdir out
for %%f in (*.mp3) do ffmpeg -i "%%f" -codec:a libmp3lame -qscale:a 0 -ac 2 -ar 44100 "../Music\%%~nf.mp3"
