#!/usr/bin/env python3

from ppcgrader.cli import cli
import ppcfreeform

if __name__ == "__main__":
    cli(ppcfreeform.Config(gpu=False))
