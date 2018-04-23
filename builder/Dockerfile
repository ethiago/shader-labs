ARG TAG=latest 

FROM ethiago/shader-labs-builder:$TAG

COPY . ./

RUN ./configure --static --mode release --tarball-path /output

RUN cd build && make && make targz

CMD ["/bin/bash" ]
