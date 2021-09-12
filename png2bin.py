from PIL import Image

def write_image(image, filename):
    height = image.height
    width = image.width

    f = open(filename, "wb")
    f.write(height.to_bytes(2, byteorder='big'))
    f.write(width.to_bytes(2, byteorder='big'))
    img_raster = []
    for i in range(height):
        for j in range(width):
            img_raster.extend(image.getpixel((j, i))[:3])

    f.write(bytearray(img_raster))
    f.close()

def read_2bytes(f):
    bytes = f.read(2) # [int(f.read(1)), int(f.read(1))]
    return int.from_bytes(bytes, byteorder = 'big')


def read_image(filename):
    f = open(filename, "rb")
    height = read_2bytes(f)
    width = read_2bytes(f)
    image = Image.new("RGB", (width, height))
    bytes = f.read()
    for i in range(height):
        for j in range(width):
            image.putpixel((j, i), (bytes[3*(i*width + j)+0],
                                    bytes[3*(i*width + j)+1],
                                    bytes[3*(i*width + j)+2]))

    return image

#Write a png image to bin
# image = Image.open("6x5.png")
# write_image(image, "6x5.bin")
# image = Image.open("3x4.png")
# write_image(image, "3x4.bin")
#
# image = Image.open("UofT.png")
# write_image(image, "UofT.bin")

#Read image from a bin file, save it to png
im0 = read_image("bin_files/img0.bin")
im1 = read_image("bin_files/img1.bin")
im2 = read_image("bin_files/img2.bin")
im3 = read_image("bin_files/img3.bin")
im4 = read_image("bin_files/img4.bin")

im0.save("png_files/img0.png")
im1.save("png_files/img1.png")
im2.save("png_files/img2.png")
im3.save("png_files/img3.png")
im4.save("png_files/img4.png")
# # im3 = read_image("6x5_grad.bin")
# im1.save("dark.png")
# im2.save("darker.png")
# im3.save("lighter.png")



# # Write multiple images from bin to png
# for i in range(200):
#     image = read_image("img%d.bin" % i)
#     image.save("img%d.png" % i)