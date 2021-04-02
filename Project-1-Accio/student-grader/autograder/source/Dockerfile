FROM gradescope/auto-builds:latest

RUN apt-get update && apt-get install -y curl unzip && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
RUN mkdir -p /autograder
COPY . /autograder/source
RUN ln -s /autograder/source/run_autograder /autograder/run_autograder

RUN apt-get update &&     bash /autograder/source/setup.sh &&     apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
