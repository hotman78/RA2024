rm -f animation/*
g++ main.cpp
echo "running"
./a.out<input.txt
echo "vis"
dot -Tpng animation/*.dot -O
# out gif
convert -delay 10 -loop 1 animation/out_*.dot.png movie.gif
ffmpeg -r 10 -pattern_type glob -i 'animation/out_*.dot.png' -vcodec libx264 -acodec copy -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" out.mp4
