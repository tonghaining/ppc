import os
from typing import Dict, List, Optional, Tuple
from ppcgrader.compiler import Compiler
from ppcgrader.runner import RunnerOutput
from ppcgrader.api_tools import EnvProperty, get_home_config_path, load_json_config


class Config:
    def __init__(self):
        self.source: str
        self.binary: str
        self.tester: str
        self.gpu: bool
        self.ignore_errors: bool
        self.nvprof: bool = False
        self.export_streams: bool
        self.on_remote: bool
        self.openmp: bool = False

        self.env_api_url: EnvProperty
        self.env_api_token: EnvProperty
        self.env_http_basic_auth: EnvProperty
        self.env_remote_grader: EnvProperty
        self.env_version: EnvProperty

    def test_command(self, test: str) -> List[str]:
        raise NotImplementedError

    def benchmark_command(self, test: str) -> List[str]:
        raise NotImplementedError

    def common_flags(self, compiler: Compiler) -> Compiler:
        raise NotImplementedError

    def find_compiler(self) -> Optional[Compiler]:
        raise NotImplementedError

    def parse_output(self,
                     output: str) -> Tuple[float, bool, Dict, Dict, Dict]:
        raise NotImplementedError

    def format_output(self, output: RunnerOutput) -> Optional[str]:
        raise NotImplementedError

    def explain_terminal(self, output, color=False):
        raise NotImplementedError

    def collect_env(self, home_config_path=None):
        sources = {}
        sources['path'] = load_json_config('config.json')
        if home_config_path is None:
            home_config_path = get_home_config_path()
        sources['home_path'] = home_config_path
        sources['home'] = load_json_config(
            os.path.join(home_config_path, 'config.json'))
        sources['factory'] = load_json_config('.ppc/factory_config.json')

        self.env_api_url = EnvProperty(sources,
                                       'api_url',
                                       'API URL',
                                       env='PPC_API_URL')
        self.env_api_token = EnvProperty(
            sources,
            'api_token',
            'API token',
            env='PPC_API_TOKEN',
            factory=False,
            help=
            "Visit https://ppc-exercises.cs.aalto.fi/token to obtain an API token for your account.",
        )
        self.http_basic_auth = EnvProperty(
            sources,
            'http_basic_auth',
            'HTTP basic authorization',
            env='PPC_HTTP_BASIC_AUTH',
        )
        self.env_remote_grader = EnvProperty(sources,
                                             'remote_grader',
                                             'remote grader name',
                                             home=False)
        self.env_version = EnvProperty(sources,
                                       'version',
                                       'version',
                                       path=False,
                                       home=False)
