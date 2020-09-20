import img2pdf
import os

filelist = os.listdir('./')
count = 0
converted = []

for file in filelist:
    if not file[0:-5] in converted and not file[0:-4] in converted and (file.endswith('.jpg') or file.endswith('.png')):
        print('found ' + file)
        imglist = []
        imglist.append(file)
        if file[-5] == '1':  # first image
            filename = file[0:-5]
            for i in range(2, 10):
                if filename + str(i) + file[-4:] in filelist:
                    imglist.append(filename + str(i) + file[-4:])
                    print('found ' + filename + str(i) + file[-4:])
                else:
                    break
        else:
            filename = file[0:-4]
        with open(filename + '.pdf', 'wb') as pdf:
            print('converting ' + filename + '.pdf')
            pdf.write(img2pdf.convert(imglist))
            count += 1
            print('%d done!' % count)
            converted.append(filename)
