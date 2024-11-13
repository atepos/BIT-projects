""""
Tensor Library with Activation Functions for Neural Networks
=============================================================
This module implements a custom `Tensor` class for automatic differentiation. 
Plus I added just for fun some commonly used activation functions, including ReLU, Sigmoid, Tanh, 
ELU, GELU, Gaussian, and Softmax.
=============================================================
Author: Petr Kaška, Martin Hemza
Email: xkaska01@stud.fit.vutbr.cz
Date: November 10, 2024
"""


# Nakonec se ukazalo, ze s GELU a ELU, lze natrenovat lepsi NS na zaklade poskytnutych trenovacich dat nez s ReLu.

import numpy as np


class Tensor:
    def __init__(self, value, back_op=None):
        self.value = value
        self.grad = np.zeros_like(value)
        self.back_op = back_op

    def __str__(self):
        str_val = str(self.value)
        str_val = '\t' + '\n\t'.join(str_val.split('\n'))
        str_bwd = str(self.back_op.__class__.__name__)
        return 'Tensor(\n' + str_val + '\n\tbwd: ' + str_bwd + '\n)'

    @property
    def shape(self):
        return self.value.shape

    def backward(self, deltas=None):
        if deltas is not None:
            assert deltas.shape == self.value.shape, f'Expected gradient with shape {self.value.shape}, got {deltas.shape}'            

            self.grad += deltas

            if self.back_op is not None:
                self.back_op()

        else:
            if self.shape != tuple() and np.prod(self.shape) != 1:
                raise ValueError(f'Can only backpropagate a scalar, got shape {self.shape}')

            self.grad = np.array(1.0)

            if self.back_op is None:
                raise ValueError(f'Cannot start backpropagation from a leaf!')
            
            self.back_op()


def sui_sum(tensor):
    result_value = np.sum(tensor.value)
    result = Tensor(result_value)

    def back_op():
        tensor.backward(np.ones_like(tensor.value)) 
    result.back_op = back_op

    return result


def add(a, b):
    result_value = a.value + b.value
    result = Tensor(result_value)

    def back_op():
        grad = result.grad
        a.backward(grad)
        b.backward(grad)
    result.back_op = back_op

    return result

def subtract(a, b):
    result_value = a.value - b.value
    result = Tensor(result_value)

    def back_op():
        grad = result.grad
        a.backward(grad)
        b.backward(-grad)
    result.back_op = back_op

    return result


def multiply(a, b):
    result_value = a.value * b.value
    result = Tensor(result_value)

    def back_op():
        grad = result.grad
        a.backward(b.value * grad)
        b.backward(a.value * grad)
    result.back_op = back_op

    return result


def relu(tensor):
    result_value = np.maximum(0, tensor.value)
    result = Tensor(result_value)
    relu_grad = (tensor.value > 0).astype(float)  

    def back_op():
        tensor.backward(relu_grad * result.grad)
    result.back_op = back_op

    return result


def dot_product(a, b):
    result_value = np.dot(a.value, b.value)
    result = Tensor(result_value)

    def back_op():
        grad = result.grad
        a.backward(np.dot(grad, b.value.T))
        b.backward(np.dot(a.value.T, grad))
    result.back_op = back_op

    return result


# Me pokusy s jinými aktivačními funkcemi ...............................................
def my_activation(tensor,flag):
    if flag == "relu":
        return relu(tensor)

    if flag == "gaus":
        result_value = np.exp(-tensor.value ** 2)  
        result = Tensor(result_value)

        def back_op():
            gaus_grad = -2 * tensor.value * result.value
            tensor.backward(gaus_grad * result.grad)
        result.back_op = back_op

        return result

    if flag == "sigmoid":
        result_value = 1 / (1 + np.exp(-tensor.value))
        result = Tensor(result_value)

        def back_op():
            sigmoid_grad = result.value * (1 - result.value)
            tensor.backward(sigmoid_grad * result.grad)
        result.back_op = back_op
        return result
        
    if flag == "tanh":
        result_value = np.tanh(tensor.value)
        result = Tensor(result_value)

        def back_op():
            tanh_grad = 1 - np.tanh(tensor.value) ** 2
            tensor.backward(tanh_grad * result.grad)
        result.back_op = back_op

        return result

    if flag == "elu":
        alpha = 1.0 
        result_value = np.where(tensor.value > 0, tensor.value, alpha * (np.exp(tensor.value) - 1))
        result = Tensor(result_value)

        def back_op():
            elu_grad = np.where(tensor.value > 0, 1, result.value + alpha)
            tensor.backward(elu_grad * result.grad)
        result.back_op = back_op
        return result

    if flag == "softmax":
        exp_values = np.exp(tensor.value - np.max(tensor.value))  
        result_value = exp_values / np.sum(exp_values)
        result = Tensor(result_value)

        def back_op():
            softmax_grad = result.value * (1 - result.value)
            tensor.backward(softmax_grad * result.grad)
        result.back_op = back_op
        return result

    if flag == "gelu":
        c = np.sqrt(2 / np.pi)
        result_value = 0.5 * tensor.value * (1 + np.tanh(c * (tensor.value + 0.05 * tensor.value ** 3)))
        result = Tensor(result_value)

        def back_op():
            tanh_out = np.tanh(c * (tensor.value + 0.05 * tensor.value ** 3))
            gelu_grad = 0.5 * (1 + tanh_out) + 0.5 * tensor.value * (1 - tanh_out ** 2) * c * (1 + 3 * 0.05 * tensor.value ** 2)
            tensor.backward(gelu_grad * result.grad)
        result.back_op = back_op
        return result

    if flag == "identity":
        return tensor
