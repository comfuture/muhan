FROM --platform=linux/amd64 rust:1.77

WORKDIR /work
ENV MUHAN_HOME=/work

# Keep runtime tooling explicit for smoke/debug sessions.
RUN apt-get update \
 && apt-get install -y --no-install-recommends python3 ca-certificates \
 && rm -rf /var/lib/apt/lists/*

CMD ["bash"]
