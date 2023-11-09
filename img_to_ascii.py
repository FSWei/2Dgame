from PIL import Image
import sys

# 字符画生成器
def to_char_image(image, scale=1.0):
    ascii_chars = [' ', '.', ':', '-', '=', '+', '*', '#', '%', '@']
    #把ascii_chars反过来
    ascii_chars = ascii_chars[::-1]
    width, height = image.size
    gray_image = image.convert('L')
    pixels = gray_image.load()
    char_image = []
    for y in range(height):
        line = '\"'
        for x in range(width):
            pixel = pixels[x, y]
            index = int(pixel / (255 / (len(ascii_chars) - 1)))
            line += ascii_chars[index] * int(scale)
        line+='\",'
        char_image.append(line)
    return '\n'.join(char_image)

# 加载GIF动画
if len(sys.argv) != 2:
    print('Usage: python gif_to_char.py <gif_file>')
    sys.exit(1)

gif_file = sys.argv[1]
gif_image = Image.open(gif_file)

# 分离每一帧并转换为字符画
frames = []
try:
    while True:
        frames.append(to_char_image(gif_image))
        gif_image.seek(len(frames))
except EOFError:
    pass

# 将字符画保存到文件中
output_file = 'output.txt'
with open(output_file, 'w') as f:
    for i, frame in enumerate(frames):
        f.write(frame)
        f.write('\n')
