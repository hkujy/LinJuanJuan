import os
import para


def run_test(mf):
    """
    run the tes
    """
    debug_exe_file = mf.root_folder + "LinJuanJuan\\x64\\Debug\\NetworkResilience.exe"
    release_exe_file = mf.root_folder + "LinJuanJuan\\x64\\Release\\NetworkResilience.exe" 

    print(debug_exe_file)
    if para.isRunTest:
        if para.isDebug:
            os.system(debug_exe_file)
        else:
            os.system(release_exe_file)