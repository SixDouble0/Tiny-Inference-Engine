import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import numpy as np

# Simple CNN for MNIST
class SimpleCNN(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 32, 3, padding=0)
        self.conv2 = nn.Conv2d(32, 64, 3, padding=0)
        self.fc1 = nn.Linear(24*24*64, 128)
        self.fc2 = nn.Linear(128, 10)
    
    def forward(self, x):
        x = torch.relu(self.conv1(x))
        x = torch.relu(self.conv2(x))
        x = x.flatten(1)
        x = torch.relu(self.fc1(x))
        return self.fc2(x)

# Create model with random weights (for testing)
model = SimpleCNN()
model.eval()

# Export weights as C header
with open('models/mnist_int8.h', 'w') as f:
    f.write('#pragma once\n#include <stdint.h>\n\n')
    
    for name, param in model.named_parameters():
        data = param.detach().numpy()
        # Quantize to int8
        scale = np.max(np.abs(data)) / 127.0
        data_int8 = np.clip(data / scale, -128, 127).astype(np.int8)
        flat = data_int8.flatten()
        
        cname = name.replace('.', '_')
        f.write(f'static const int8_t {cname}[] = {{\n    ')
        f.write(', '.join(str(x) for x in flat))
        f.write('\n};\n\n')

print("Done! models/mnist_int8.h generated")