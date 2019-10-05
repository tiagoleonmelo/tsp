FROM gcc:4.9

WORKDIR /usr/src/tsp

COPY . .

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y locales \
    && sed -i -e 's/# pt_PT.UTF-8 UTF-8/pt_PT.UTF-8 UTF-8/' /etc/locale.gen \
    && dpkg-reconfigure --frontend=noninteractive locales \
    && update-locale LANG=pt_PT.UTF-8
ENV LANG pt_PT.UTF-8 
ENV LC_ALL pt_PT.UTF-8

RUN cc -std=gnu99 -O3 src/tsp.c -o build/tsp -lm
RUN cc -std=gnu99 -O3 src/tsp_v2.c -o build/tsp_v2 -lm
RUN chmod u+x scripts/entry.sh
ENTRYPOINT ["scripts/entry.sh"]
