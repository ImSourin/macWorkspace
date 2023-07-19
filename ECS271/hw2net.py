# Importing the required libraries
import numpy as np
from scipy.io import loadmat
import matplotlib.pyplot as plt
import sys


class FCLayer:
    input = None
    output = None
    weights = None
    bias = None

    def fwd_prop(self, inp):
        self.input = inp
        #             self.output = np.dot(inp, self.weights) + self.bias
        self.output = np.dot(inp, self.weights) + self.bias
        return self.output

    def back_prop(self, error, learning_rate):
        #             print(error)
        back_prop_input = np.dot(error, self.weights.T)
        back_prop_weights = (np.dot(self.input.T, error)) / self.input.shape[0]
        self.weights -= learning_rate * back_prop_weights
        #             print(error.shape, self.input.shape, self.weights.shape, self.bias.shape, back)
        self.bias -= learning_rate * error.sum(axis=0) / self.input.shape[0]

        return back_prop_input


class SoftmaxLayer:
    input = None
    output = None

    #         def softmax_activation(self):
    #             return np.exp(self.input-np.amax(self.input, axis=1)[:,None])/np.exp(self.input-np.amax(self.input, axis=1)[:,None]).sum(axis=1)[:,None]

    def softmax_activation(self):
        return np.exp(self.input) / np.exp(self.input).sum(axis=1)[:, None]

    def gradient_softmax_activation(self):
        return np.sum(
            np.eye(self.input.shape[1]) * self.input[:, np.newaxis, :] - np.einsum('ij,ij->i', self.input, self.input)[
                                                                         :, None, None], axis=1)

    def gradient_softmax_with_cross_entropy(self, actual, expected):
        return actual - expected

    def fwd_prop(self, inp):
        self.input = inp
        #             print('input-',inp.shape)
        self.output = self.softmax_activation()
        return self.output

    def back_prop(self, expected):
        return self.gradient_softmax_with_cross_entropy(self.output, expected)


class Network:
    layers = []
    learning_rate = None
    epochs = None
    batch_size = None

    def add_fc_layer(self, inp_size, op_size):
        fc_layer = FCLayer()
        fc_layer.weights = np.random.uniform(-1.0, 1.0, size=(inp_size, op_size))
        fc_layer.bias = np.random.uniform(-1.0, 1.0, size=(op_size))
        self.layers.append(fc_layer)

    def add_softmax_layer(self):
        ac_layer = SoftmaxLayer()
        self.layers.append(ac_layer)

    def cross_entropy_loss(self, expected, actual):
        return -np.sum(expected * np.log(actual))

    def gradient_cross_entropy_loss(self, actual, expected):
        return -expected / actual

    def train_mbgd(self, x_train, y_train):
        x_train_mini_batch = []
        y_train_mini_batch = []
        for epoch in range(self.epochs):
            loss = 0
            no_of_batches_done = 0;
            total_no_of_batches = (int)(x_train.shape[0] / self.batch_size)
            for id in range(x_train.shape[0]):
                x = x_train[id]
                y = y_train[id]
                x_train_mini_batch.append(x)
                y_train_mini_batch.append(y)

                if len(y_train_mini_batch) == self.batch_size:
                    x_train_mini_batch = np.array(x_train_mini_batch, dtype=np.float128)
                    y_train_mini_batch = np.array(y_train_mini_batch, dtype=np.float128)

                    output = x_train_mini_batch
                    for layer in self.layers:
                        output = layer.fwd_prop(output)

                    loss += self.cross_entropy_loss(y_train_mini_batch, output)
                    #                     print(epoch, output, np.sum(output))

                    back_prop_err = self.layers[-1].back_prop(y_train_mini_batch)
                    for layer in reversed(self.layers[:-1]):
                        back_prop_err = layer.back_prop(back_prop_err, self.learning_rate)

                    x_train_mini_batch = []
                    y_train_mini_batch = []
                    no_of_batches_done += 1;
                    sys.stdout.write('\r')
                    percentage_completion = (int)(no_of_batches_done * 100 / total_no_of_batches)
                    sys.stdout.write("[%-20s] %d%%" % ('=' * (int)(percentage_completion / 5), percentage_completion))
                    sys.stdout.flush()
            print('Epoch - ', epoch, '; Loss - ', loss / y_train.shape[0])

    def test(self, x_test, y_test):
        op = np.array(x_test, dtype=np.float128)
        for layer in self.layers:
            op = layer.fwd_prop(op)

        return 1 - np.count_nonzero(op.argmax(axis=1) - y_test.argmax(axis=1)) / x_test.shape[0]


# Loading the data from external file
data = loadmat('MNIST_digit_data.mat')
images_train, images_test, labels_train, labels_test = \
    data['images_train'], data['images_test'], data['labels_train'], data['labels_test']

# Preprocessing the data to make it suitable for prediction
# Set seed to make the permutation same across all machines
np.random.seed(1)

# randomly permute data points
indexes = np.random.permutation(images_train.shape[0])
images_train = images_train[indexes]
labels_train = labels_train[indexes]
indexes = np.random.permutation(images_test.shape[0])
images_test = images_test[indexes]
labels_test = labels_test[indexes]

# Flatten training images
images_train = images_train.reshape(len(images_train), -1)
images_test = images_test.reshape(len(images_test), -1)

# Changing label arrays to 1-D
labels_train_1d = labels_train.flatten()
labels_test_1d = labels_test.flatten()

# Convert to one-hot encoding
labels_train_one_hot = np.zeros((labels_train_1d.size, 10))
labels_train_one_hot[np.arange(labels_train_1d.size), labels_train_1d] = 1

labels_test_one_hot = np.zeros((labels_test_1d.size, 10))
labels_test_one_hot[np.arange(labels_test_1d.size), labels_test_1d] = 1

network = Network()
network.learning_rate = 0.5
network.batch_size = 10
network.epochs = 100
network.add_fc_layer(784, 10)
network.add_softmax_layer()

network.train_mbgd(images_train, labels_train_one_hot)
print('Acc-', network.test(images_test, labels_test_one_hot))
