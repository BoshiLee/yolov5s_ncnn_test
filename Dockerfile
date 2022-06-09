FROM ubuntu:20.04
# copy only the dependencies installation from the 1st stage image

# avoid stuck build due to user prompt
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install --no-install-recommends -y sudo ssh \
	build-essential \
	wget \
	vim \
	unzip \
	git \
	cmake \
	gcc \
	g++ \
	gdb \
	clang \
	rsync \
	tar \
	apt-utils \
	openssh-server \
	gdb \
	gdbserver \
	ca-certificates

RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777

RUN useradd -ms /bin/bash clion
RUN echo 'clion:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
########################################################
RUN sudo apt install --no-install-recommends -y software-properties-common
RUN sudo add-apt-repository ppa:deadsnakes/ppa

RUN sudo apt-get install --no-install-recommends -y libprotobuf-dev \
	python3.9 \
	libprotobuf-dev \
	protobuf-compiler \
	libgtk2.0-dev \
	pkg-config \
	libavcodec-dev \
	libavformat-dev \
	libswscale-dev
WORKDIR /home/app
RUN wget --no-check-certificate https://github.com/opencv/opencv/archive/4.1.1.zip
RUN unzip 4.1.1
RUN rm 4.1.1.zip
RUN git config --global http.sslverify false
RUN git clone https://github.com/Tencent/ncnn.git
RUN git config --global http.sslverify true
RUN wget --no-check-certificate https://sdk.lunarg.com/sdk/download/1.2.189.0/linux/vulkansdk-linux-x86_64-1.2.189.0.tar.gz?Human=true -O vulkansdk-linux-x86_64-1.2.189.0.tar.gz
RUN tar -xf vulkansdk-linux-x86_64-1.2.189.0.tar.gz
RUN export VULKAN_SDK=$(pwd)/1.2.189.0/x86_64
RUN rm vulkansdk-linux-x86_64-1.2.189.0.tar.gz
########################################################

CMD ["/usr/sbin/sshd", "-D"]
