import random
from skimage.io import imread, imsave
from numpy import reshape, frombuffer
import numpy as np
from PIL import Image
import os


def filename_encrypted(file):
    file_name = file.split('.')[0]
    file_type = file.split('.')[1]
    file_enc = file_name + "-enc." + file_type
    return file_enc


def filename_decrypted(file):
    file_name = file.split('.')[0]
    file_type = file.split('.')[1]
    file_dec = file_name + "-dec." + file_type
    return file_dec


def evaluate_benchmark(file):
    benchmark = Image.open('data/tux.bmp')
    benchmark_rgb = benchmark.convert("RGB")
    benchmark_pixels = list(benchmark_rgb.getdata())
    width, height = benchmark.size
    output = Image.open(file)
    output_rgb = output.convert("RGB")
    output_pixels = list(output_rgb.getdata())
    errors = 0

    for i in range(height * width):
        if benchmark_pixels[i] != output_pixels[i]:
            errors += 1
    return errors


class ImageLoader:
    def __init__(self):
        self.img = None
        self.shape = None

    def load_file(self, file):
        self.img = imread(fname=file)
        self.shape = self.img.shape

    def to_vector(self):
        img_vector = reshape(self.img, -1)
        img_vector = img_vector.tobytes()
        return img_vector

    def from_vector(self, img_vector, original_img):
        img_v = frombuffer(img_vector, dtype=original_img.dtype)
        self.img = reshape(img_v, original_img.shape)

    def save_file(self, file):
        imsave(fname=file, arr=self.img)

    def simulate_error(self, file, error):
        if error > 0:
            error_amount = int(self.img.shape[0] * self.img.shape[1] * (error / 100))
            broken_col = np.random.choice(range(0, self.img.shape[1]), size=error_amount, replace=True)
            broken_row = np.random.choice(range(0, self.img.shape[0]), size=error_amount, replace=True)
            for i in range(error_amount):
                self.img[broken_row[i]][broken_col[i]] = random.randint(0, 255)
            filename = file.split('.')[0]
            ext = file.split('.')[1]
            path = filename + '-err.' + ext
            self.save_file(path)
        img_v = self.to_vector()
        return img_v


class Secret:
    @staticmethod
    def save_secret(secret, file):
        sec_file = open(file + ".secret", "wb")
        sec_file.write(secret)
        sec_file.close()

    @staticmethod
    def load_secret(file):
        sec_file = open(file + ".secret", "rb")
        length = os.path.getsize(file + ".secret")
        secret = sec_file.read(length)
        sec_file.close()
        return secret

    # # better to use:
    # @staticmethod
    # def load_key(file):
    #     return Secret.load_secret(file)[0:32]
    #
    # @staticmethod
    # def load_key_IV(file):
    #     sec = Secret.load_secret(file)
    #     return sec[0:32], sec[32:48]
