from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from image_loader import *
import os


class CFB:
    def __init__(self, key=None, iv=None):
        if key is None:
            key = os.urandom(32)
        self.key = key
        if iv is None:
            iv = os.urandom(16)
        self.iv = iv
        self.sec = None

    def encrypt(self, data):
        cipher = Cipher(algorithms.AES(self.key), modes.CFB(self.iv))
        encryptor = cipher.encryptor()
        ct = encryptor.update(data) + encryptor.finalize()
        return ct

    def decrypt(self, ct):
        cipher = Cipher(algorithms.AES(self.key), modes.CFB(self.iv))
        decryptor = cipher.decryptor()
        result = decryptor.update(ct) + decryptor.finalize()
        return result

    def run_encryption(self, file, error):
        # Load
        img = ImageLoader()
        img.load_file(file)
        img_v = img.simulate_error(file, error)
        # Encrypt
        cfb = CFB()
        enc_v = cfb.encrypt(img_v)
        img_enc = ImageLoader()
        img_enc.from_vector(enc_v, img.img)
        img_enc.save_file(filename_encrypted(file))
        self.sec = cfb.key + cfb.iv
        Secret.save_secret(self.sec, filename_encrypted(file))

    @staticmethod
    def run_decryption(file, error):
        # Load
        img = ImageLoader()
        img.load_file(file)
        img_v = img.simulate_error(file, error)
        # Decrypt
        sec = Secret.load_secret(file)
        cfb = CFB(sec[0:32], sec[32:48])
        dec_v = cfb.decrypt(img_v)
        img_dec = ImageLoader()
        img_dec.from_vector(dec_v, img.img)
        img_dec.save_file(filename_decrypted(file))
