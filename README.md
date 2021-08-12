# colorAnalysis

The **colourAnalysis** class implements some methods to extract color data from an image and classify them using Hue, Saturation, and Brightness values. The main purpose is to extract information to use in audio synthesis.

The extractColor classifies the proximity of the colors using a configurable threshold for each hue, saturation, and brightness parameter.

The main algorithm does a lot of std::vector operations that are a bit slow to use in real-time applications. 
