#! /bin/bash
echo "test"
shopt -s extglob
rm -rf !(pushing_cub3D.sh)
cp -r ../newCub3d/* .
git add .
git commit -m "pushing updates"
git push
