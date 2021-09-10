Seam Carving

Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of width (or height) at a time. A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row; a horizontal seam is a path of pixels connected from the left to the right with one pixel in each column. Unlike standard content-agnostic resizing techniques (such as cropping and scaling), seam carving preserves the most interest features (aspect ratio, set of objects present, etc.) of the image. Although the underlying algorithm is simple and elegant, it was not discovered until 2007. Now, it is now a core feature in Adobe Photoshop and other computer graphics applications.

	


In this assignment, I created a data type that resizes a H-by-W image using the seam-carving technique. I computed the dual-gradient energy function, and then found vertical “seams” – paths from the top to the bottom of the image – such that the sum of the dual-gradient energy values in the pixels along the path is as small as possible

Energy calculation. The first step is to calculate the energy of a pixel, which is a measure of its perceptual importance – the higher the energy, the less likely that the pixel will be included as part of a seam (as you will see in the next step). In this project, I used the dual-gradient energy function. 
Here is the dual-gradient energy function of the surfing image above:

The energy is high (white) for pixels in the image where there is a rapid color gradient (such as the boundary between the sea and sky and the boundary between the surfing Josh Hug (the original author of this assignment) on the left and the ocean behind him). The seam carving technique avoids removing such high-energy pixels.

Seam identification. The next step is to find a vertical seam of minimum total energy. The seam is a path through pixels from top to bottom such that the sum of the energies of the pixels is minimal. You will identify the minimum-energy seam using dynamic programming.


Seam removal. The final step is removing from the image all the pixels along the vertical seam.
