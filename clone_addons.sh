#!/bin/bash

# Dependencies for Sociogramainteractivo

cd ../

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}carlesgutierrez/ofxControlPanel.git

${PREFIX}roymacdonald/ofxGLWarper.git
