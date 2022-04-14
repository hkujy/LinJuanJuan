import os
import para

def run_test(mf):
    """
    run the tes
    """
    debug_exe_file = mf.root_folder + "x64\\Debug\\NetworkResilience.exe"
    release_exe_file = mf.root_folder + "x64\\Release\\NetworkResilience.exe" 

    # print(debug_exe_file)
    if para.isRunTest:
        if para.isDebug:
            print("Run debug mode")
            os.system(debug_exe_file)
        else:
            print("Run release mode")
            os.system(release_exe_file)