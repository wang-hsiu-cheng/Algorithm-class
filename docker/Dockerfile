# Use Ubuntu as base image
FROM ubuntu:latest

# Set the maintainer
LABEL maintainer="Your Name wang.tw.ted@gmail.com"

# Install Development Tools and dependencies
RUN apt update && apt install -y \
    build-essential cmake automake autoconf \
    sudo \
    gcc \
    # gcc-c++ \
    make \
    # vim \
    # git \
    wget \
    && rm -rf /var/lib/apt/lists/*
    # && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    # && chmod 0440 /etc/sudoers.d/$USERNAME

# Set up working directory
WORKDIR /workspace

ENV SHELL /bin/bash
ENV TERM=xterm-256color

# USER $USERNAME

# COPY .bashrc /home/$USERNAME/.bashrc.conf
# RUN cat /home/$USERNAME/.bashrc.conf >> /home/$USERNAME/.bashrc

# Set the default command to bash
CMD ["/bin/bash", "-ic"]