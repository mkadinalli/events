import hashlib
import base64

def generate_sec_websocket_accept():
    # We generate the accept key by concatenating the sec-websocket-key
    # and the magic string, Sha1 hashing it, and base64 encoding it.
    # See https://datatracker.ietf.org/doc/html/rfc6455#page-7
    combined = 'dGhlIHNhbXBsZSBub25jZQ==' + '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
    hashed_combined_string = hashlib.sha1(combined.encode())
    encoded = base64.b64encode(hashed_combined_string.digest())
    return encoded

x = generate_sec_websocket_accept()
if x == b's3pPLMBiTxaQ9kYGzzhZRbK+xOo=':
    print("correct")
else:
    print("failed")

print(x)