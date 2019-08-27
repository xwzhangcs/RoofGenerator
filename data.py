#######################################################################
# Generate HTML file that shows the input and output images to compare.

from os.path import isfile, join
from PIL import Image
import os
import argparse
import numpy as np
import json
import subprocess
import sys
import shutil
import glob
import pandas as pd
from skimage import data, img_as_float
from skimage.measure import compare_ssim as ssim
from skimage import io
import cv2
import random

total = 31082
train_ratio = 0.95
train_val_ratio = 0.8


def data_shuffle(src_path, src_csv, output_path):
	grammar_dir = output_path
	grammar_dir_train = output_path + '/train'
	grammar_dir_val = output_path + '/val'
	grammar_dir_test = output_path + '/test'

	if not os.path.exists(grammar_dir):
		os.makedirs(grammar_dir)
		os.makedirs(grammar_dir_train)
		os.makedirs(grammar_dir_val)
		os.makedirs(grammar_dir_test)

	train = int(total * train_ratio * train_val_ratio)
	val = int(total * train_ratio) - train

	src_images = sorted(os.listdir(src_path))
	df_paras = pd.read_csv(src_csv, delimiter=",", header=None)
	src_dic = dict(zip(list(range(len(src_images))), src_images))
	l = list(src_dic.items())
	random.shuffle(l)
	src_dic = dict(l)
	index = 0
	train_img = []
	train_para_1 = []
	train_para_2 = []
	train_para_3 = []
	train_para_4 = []
	train_para_5 = []
	train_para_6 = []
	train_para_7 = []
	train_para_8 = []
	train_index = 0
	val_img = []
	val_para_1 = []
	val_para_2 = []
	val_para_3 = []
	val_para_4 = []
	val_para_5 = []
	val_para_6 = []
	val_para_7 = []
	val_para_8 = []
	val_index = 0
	test_img = []
	test_para_1 = []
	test_para_2 = []
	test_para_3 = []
	test_para_4 = []
	test_para_5 = []
	test_para_6 = []
	test_para_7 = []
	test_para_8 = []
	test_index = 0
	for key, value in src_dic.items():
		src_img = src_path + '/' + value
		src_img = cv2.imread(src_img, cv2.IMREAD_UNCHANGED)
		paras = df_paras.loc[df_paras[0] == value].iloc[0, 1:]
		if 0 <= index < train:
			# output_img = grammar_dir_train + '/' + value
			output_img = grammar_dir_train + '/roof_' + format(train_index, '05d') + '.png'
			cv2.imwrite(output_img, src_img)
			train_img.append('roof_' + format(train_index, '05d') + '.png')
			train_para_1.append(paras.iloc[0])
			train_para_2.append(paras.iloc[1])
			train_para_3.append(paras.iloc[2])
			train_para_4.append(paras.iloc[3])
			train_para_5.append(paras.iloc[4])
			train_para_6.append(paras.iloc[5])
			train_para_7.append(paras.iloc[6])
			train_para_8.append(paras.iloc[7])
			train_index = train_index + 1
		elif train <= index < train + val:
			# output_img = grammar_dir_val + '/' + value
			output_img = grammar_dir_val + '/roof_' + format(val_index, '05d') + '.png'
			cv2.imwrite(output_img, src_img)
			val_img.append('roof_' + format(val_index, '05d') + '.png')
			val_para_1.append(paras.iloc[0])
			val_para_2.append(paras.iloc[1])
			val_para_3.append(paras.iloc[2])
			val_para_4.append(paras.iloc[3])
			val_para_5.append(paras.iloc[4])
			val_para_6.append(paras.iloc[5])
			val_para_7.append(paras.iloc[6])
			val_para_8.append(paras.iloc[7])
			val_index = val_index + 1
		else:
			# output_img = grammar_dir_test + '/' + value
			output_img = grammar_dir_test + '/roof_' + format(test_index, '05d') + '.png'
			cv2.imwrite(output_img, src_img)
			test_img.append('roof_' + format(test_index, '05d') + '.png')
			test_para_1.append(paras.iloc[0])
			test_para_2.append(paras.iloc[1])
			test_para_3.append(paras.iloc[2])
			test_para_4.append(paras.iloc[3])
			test_para_5.append(paras.iloc[4])
			test_para_6.append(paras.iloc[5])
			test_para_7.append(paras.iloc[6])
			test_para_8.append(paras.iloc[7])
			test_index = test_index + 1
		index = index + 1
	data = {'grammar_image': train_img,
			'para_1': train_para_1,
			'para_2': train_para_2,
			'para_3': train_para_3,
			'para_4': train_para_4,
			'para_5': train_para_5,
			'para_6': train_para_6,
			'para_7': train_para_7,
			'para_8': train_para_8
			}
	df = pd.DataFrame(data)
	df.to_csv(output_path + '/train.csv', index=False)

	data = {'grammar_image': val_img,
			'para_1': val_para_1,
			'para_2': val_para_2,
			'para_3': val_para_3,
			'para_4': val_para_4,
			'para_5': val_para_5,
			'para_6': val_para_6,
			'para_7': val_para_7,
			'para_8': val_para_8
			}
	df = pd.DataFrame(data)
	df.to_csv(output_path + '/val.csv', index=False)

	data = {'grammar_image': test_img,
			'para_1': test_para_1,
			'para_2': test_para_2,
			'para_3': test_para_3,
			'para_4': test_para_4,
			'para_5': test_para_5,
			'para_6': test_para_6,
			'para_7': test_para_7,
			'para_8': test_para_8
			}
	df = pd.DataFrame(data)
	df.to_csv(output_path + '/test.csv', index=False)


# Print the output.
def main(input, output):
	data_shuffle(input + '/data_src', input + '/parameters.txt', output)
	# create_training_dataset(input, output, True)
	# prepare_classifer(input, output)


if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("input", help="path to input image folder (e.g., input_data)")
	parser.add_argument("output", help="path to output html filename")
	args = parser.parse_args()

	main(args.input, args.output)
