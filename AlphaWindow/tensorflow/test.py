import tensorflow as tf
import numpy as np
model = tf.keras.models.load_model('./weather.h5')
res = np.empty([1,9])
print(res.shape)
print(model.predict(res))