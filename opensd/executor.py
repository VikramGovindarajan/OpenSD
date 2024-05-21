import subprocess

def _process_CLI_arguments(opensd_exec='opensd'):
    """Converts user-readable flags in to command-line arguments to be run with
    the OpenSD executable via subprocess.

    Parameters
    ----------
    opensd_exec : str, optional
        Path to OpenMC executable. Defaults to 'opensd'.

    Returns
    -------
    args : Iterable of str
        The runtime flags converted to CLI arguments of the OpenSD executable

    """

    args = [opensd_exec]

    return args

def _run(args, output, cwd):
    # Launch a subprocess
    p = subprocess.Popen(args, cwd=cwd, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT, universal_newlines=True)

    # Capture and re-print OpenSD output in real-time
    lines = []
    while True:
        # If OpenSD is finished, break loop
        line = p.stdout.readline()
        if not line and p.poll() is not None:
            break

        lines.append(line)
        if output:
            # If user requested output, print to screen
            print(line, end='')

    # # Raise an exception if return status is non-zero
    # if p.returncode != 0:
        # # Get error message from output and simplify whitespace
        # output = ''.join(lines)
        # if 'ERROR: ' in output:
            # _, _, error_msg = output.partition('ERROR: ')
        # elif 'what()' in output:
            # _, _, error_msg = output.partition('what(): ')
        # else:
            # error_msg = 'OpenSD aborted unexpectedly.'
        # error_msg = ' '.join(error_msg.split())

        # raise RuntimeError(error_msg)

def run(output=True, cwd='.',opensd_exec='opensd'):
    """Run an OpenSD simulation.

    Parameters
    ----------
    output : bool
        Capture OpenMC output from standard out
    cwd : str, optional
        Path to working directory to run in. Defaults to the current working
        directory.
    opensd_exec : str, optional
        Path to OpenSD executable. Defaults to 'opensd'.

    Raises
    ------
    RuntimeError
        If the `opensd` executable returns a non-zero status

    """
    args = _process_CLI_arguments(opensd_exec=opensd_exec)

    _run(args, output, cwd)
