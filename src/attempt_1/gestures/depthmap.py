import freenect, cv2
import numpy as np



def getDepthMap():
    print "BLA2\n"
    depth, timestamp = freenect.sync_get_depth()
    print "BLA2\n"
    # Decrease all values in depth map to within 8 bits to be uint8
    depth = np.clip(depth, 0, 2**10 - 1)
    depth >>= 2
    depth = cv2.GaussianBlur(depth, (5,5), 0)
    return depth.astype(np.uint8)


def getMask():
    depth = getDepthMap()
    darkestShade = depth.min()  # darkest shade = closest object (assume is hand)
    darkestShade = np.clip(darkestShade, 0, 150)  # ignore if closest object is far away (ie. shade >= 150)
    ret, thresh = cv2.threshold(depth, darkestShade+25, 255, cv2.THRESH_BINARY_INV)
    return thresh


def getContours(threshImg):
    image, contours, hierarchy = cv2.findContours(threshImg, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    return contours
