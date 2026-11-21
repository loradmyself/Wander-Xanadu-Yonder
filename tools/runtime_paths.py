"""Compile-time source path shortening for the WXY platform.

Debugger paths stay stable while the compiler sees project-relative names.
Renamed from the reference project's runtime_paths.py; the env flag is wxy-owned
so the two trees can never share state.
"""
from pathlib import Path


def macro_flags(*mappings):
    return ['-fmacro-prefix-map=' + Path(root).resolve().as_posix() + '/=' + prefix.rstrip('/') + '/'
            for root, prefix in mappings]


def configure(env):
    import rtconfig

    if rtconfig.PLATFORM != 'gcc' or env.get('WXY_RUNTIME_PATHS'):
        return
    root = Path(__file__).resolve().parents[1]
    # GCC on Windows accepts either separator in source file names.
    flags = ''.join(' "' + flag + '"' for flag in macro_flags((root, '.')))
    env.Append(CCFLAGS=flags, ASFLAGS=flags)
    env['WXY_RUNTIME_PATHS'] = True
