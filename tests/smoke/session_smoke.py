#!/usr/bin/env python3
import os
import random
import socket
import string
import time

HOST = "127.0.0.1"
PORT = int(os.environ.get("MUD_PORT", "4000"))
ENC = "cp949"


def recv_some(sock: socket.socket, timeout: float = 0.5) -> bytes:
    sock.settimeout(timeout)
    chunks = []
    while True:
        try:
            data = sock.recv(4096)
            if not data:
                break
            chunks.append(data)
            if len(data) < 4096:
                break
        except socket.timeout:
            break
    return b"".join(chunks)


def send_line(sock: socket.socket, text: str, wait: float = 0.6) -> bytes:
    sock.sendall(text.encode(ENC) + b"\n")
    time.sleep(wait)
    return recv_some(sock)


def wait_port(host: str, port: int, sec: float = 40.0) -> None:
    t0 = time.time()
    while time.time() - t0 < sec:
        try:
            with socket.create_connection((host, port), timeout=0.5):
                return
        except OSError:
            time.sleep(0.2)
    raise RuntimeError("server port not ready")


def mk_name() -> str:
    # Keep first char "타" so legacy first_han() folder stays under player/타.
    second = chr(0xAC00 + random.randint(0, 11171))
    third = chr(0xAC00 + random.randint(0, 11171))
    return "타" + second + third


def main() -> None:
    wait_port(HOST, PORT)

    name = mk_name()
    password = "pw" + "".join(random.choice(string.digits) for _ in range(4))

    transcript = bytearray()
    transcript2 = bytearray()

    # Session 1: create player + run core commands.
    with socket.create_connection((HOST, PORT), timeout=3.0) as sock:
        time.sleep(0.7)
        transcript.extend(recv_some(sock, 1.0))

        for step in [
            name,
            "y",
            "남",
            "4",
            "12 10 12 10 10",
            "1",
            "선",
            "7",
            password,
            "환영",
            "도움말",
            "건강",
            "저장",
        ]:
            transcript.extend(send_line(sock, step))

    time.sleep(0.5)

    # Session 2: login existing player (save/load round-trip path).
    with socket.create_connection((HOST, PORT), timeout=3.0) as sock:
        time.sleep(0.7)
        transcript2.extend(recv_some(sock, 1.0))
        for step in [
            name,
            password,
            "건강",
            "도움말",
        ]:
            transcript2.extend(send_line(sock, step))

    transcript.extend(transcript2)

    text = transcript.decode(ENC, errors="ignore")

    if len(transcript) < 200:
        raise RuntimeError("smoke failed: too little server output")

    hints = ["도움", "환영", "건강", "이름"]
    if not any(h in text for h in hints):
        raise RuntimeError("smoke failed: expected response hints not found")

    if len(transcript2) < 60:
        raise RuntimeError("smoke failed: reconnect transcript too small")


if __name__ == "__main__":
    main()
