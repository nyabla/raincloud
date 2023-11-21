FROM alpine:3.18 AS builder

# install pip/python, libtorrent, g++, ninja
RUN apk add --no-cache py3-pip libtorrent-rasterbar-dev g++ ninja
RUN pip3 install --break-system-packages meson

WORKDIR /usr/src/raincloud/backend/
COPY . .

RUN meson setup builddir
WORKDIR builddir
RUN meson compile

FROM alpine:3.18 AS container

WORKDIR /etc/raincloud/backend/

RUN apk add --no-cache libtorrent-rasterbar

COPY --from=builder /usr/src/raincloud/backend/builddir/raincloud /etc/raincloud/backend/raincloud

EXPOSE 8000
CMD ["raincloud"]
