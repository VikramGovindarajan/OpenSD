import os
import typing  # required to prevent typing.Union namespace overwriting Union

# Type for arguments that accept file paths
PathLike = typing.Union[str, os.PathLike]
