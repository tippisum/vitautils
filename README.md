# vitautils

## tmvMemProbe
- A kernel mode plugin for probing user address space.
- Load the plugin with taiHEN by calling taiLoadKernelModule("ux0:/path/to/tmvMemProbe.skprx", 0, NULL);
- Start by calling taiStartKernelModule(modid, sizeof(path_to_result_file), path_to_result_file, 0, NULL, &res);
- Result will be saved to the file as an array of base-size pairs.
