FROM ethiago/shaderlabs-builder

COPY . ./

RUN ./configure --static --mode release --tarball-path /output

RUN cd build && make && make targz

CMD ["/bin/bash" ]
