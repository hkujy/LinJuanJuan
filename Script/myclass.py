
import os

class FileFolderClass(object):
    """
        folder and file classs
    """
    def __init__(self):
        self.root_folder = ''
        self.set_root_folder()

    def set_root_folder(self):
        """
            set the root folder 
            just in case in the IPTOP folder is not placed under the folder of GitHub
        """
        # self.root_folder = "C:\\GitCodes\\"
        self.root_folder = "C:\\GitCodes\\LinJuanJuan\\"
        # self.root_folder = "C:\\GitCodes\\LinJuanJuanTest2\\"
        # folder = os.path.abspath(os.path.dirname(__file__))
        # x = folder.split("\\")
        # for i in range(0, len(x)-1):
        #     print("xi={0},xi+1={1}".format(x[i],x[i+1]))
        #     if x[i+1] !="LinJuanJuan":
        #         self.root_folder = self.root_folder + x[i]+"\\"
        #     else:
        #         self.root_folder = self.root_folder  + x[i]+"\\"
        #         break
