#!/usr/bin/env python3
#
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
#
# SPDX-License-Identifier: MIT

"""
AMD POC Source Code Bootstrapping Tool
======================================

This script automates the setup of projects by:
- Cloning Git repositories.
- Downloading third-party artifacts.
- Performing post clone tasks such as copying, deleting, and extracting files.

It uses a YAML manifest file to define the repositories, artifacts, and tasks. 
This ensures a consistent and automated environment setup for development.

Requirements:
-------------
- Python 3.8 or later
- External Python packages:
  - PyYAML (for YAML parsing)
  - requests (for downloading artifacts)
  - colorlog (for enhanced logging)

Usage:
------
Run the script with a YAML manifest file:

    ./bootstrapper.py -f <manifest.yml>

Arguments:
----------
- `-f, --file`  : Path to the YAML manifest file (default: `manifest.yml`).
- `-d, --debug` : Enable debug mode for detailed logging.

Example:
--------
    ./bootstrapper.py -f <project_manifest.yml>

Manifest File Structure:
------------------------
The YAML manifest file should have the following structure:

    project: project_name
    repositories:
      - repo: repo_name
        url: https://example.com/repo.git
        branch: main
        destination: path/to/clone
        submodules: no-recursive
        tasks:
          - action: copy
            source: path/to/source
            destination: path/to/dest
    artifacts:
      - file: artifact_name
        url: https://example.com/artifact.zip
        destination: path/to/save
        tasks:
          - action: expand
            source: path/to/archive.zip
            destination: path/to/extract

Notes:
------
- **SSL Verification Disabled**: SSL verification is turned off for artifact downloads. 
  Use this script only in trusted environments to avoid security risks.
- **Idempotency**: If a repository or artifact already exists, the script will skip re-cloning or re-downloading.
- **Logging**: Logs are saved to `bootstrap.log` in the current directory for troubleshooting.

Troubleshooting:
----------------
- Ensure Python 3.8 or later is installed.
- Install missing dependencies using `pip install -r requirements.txt`.
- Verify that the YAML manifest file is correctly formatted.
- Check the `bootstrap.log` file for error details.
"""

import os
import stat
import subprocess
import argparse
import logging
import shutil
import requests
import yaml
import zipfile
import tarfile
from colorlog import ColoredFormatter

class TaskExecutor:
    """Handles post-clone tasks like moving, copying, or deleting files/directories."""
    def __init__(self, repo, base_dir):
        self.repo = repo
        self.base_dir = base_dir
        self.tasks = repo.get('tasks', [])

    def _copy(self, source, destination):
        if not destination:
            logging.error(f"Destination not specified for copy action in {self.repo.get('name', 'unknown')}")
            return
        logging.info(f"Copying {source} to {destination}...")
        if os.path.isdir(source):
            shutil.copytree(source, destination, dirs_exist_ok=True)
        else:
            destination_dir = os.path.dirname(destination)
            os.makedirs(destination_dir, exist_ok=True)
            shutil.copy2(source, destination)

    def _delete(self, source, destination=None):
        def remove_readonly(func, path, _):
            os.chmod(path, stat.S_IWRITE)
            func(path)

        logging.info(f"Deleting {source}...")
        if os.path.isdir(source):
            shutil.rmtree(source, onerror=remove_readonly)
        elif os.path.isfile(source):
            os.remove(source)
        else:
            logging.warning(f"Path {source} does not exist.")

    def _expand(self, source, destination):
        os.makedirs(destination, exist_ok=True)
        logging.info(f"Extracting {source} to {destination}...")
        try:
            if source.endswith('.zip'):
                with zipfile.ZipFile(source, 'r') as zip_ref:
                    zip_ref.extractall(destination)
            elif source.endswith(('.tar.gz', '.tgz', '.tar')):
                with tarfile.open(source, 'r:*') as tar_ref:
                    tar_ref.extractall(destination)
            else:
                logging.warning(f"Unsupported file format for extraction: {source}")
        except Exception as e:
            logging.error(f"Error extracting {source}: {e}")

    def execute_tasks(self):
        """Executes all post-clone tasks specified in the repository configuration."""
        action_map = {
            'copy': self._copy,
            'delete': self._delete,
            'expand': self._expand,
        }

        for task in self.tasks:
            action = task.get('action')
            func = action_map.get(action, 'unknown')

            if func:
                source = os.path.join(self.base_dir, task.get('source', ''))
                destination = (
                    os.path.join(self.base_dir, task.get('destination', '')) if 'destination' in task else None
                )

                try:
                    func(source, destination)
                except Exception as e:
                    logging.error(f"Error during {action} task for {source}: {e}")
            else:
                logging.warning(f"Unknown action '{action}' in {self.repo.get('name', 'unknown')}")


class RepoManager:
    """Handles repository cloning and setup."""
    @staticmethod
    def run_command(command, cwd=None):
        try:
            cmdline = ' '.join(command)
            result = subprocess.run(command, cwd=cwd, check=True, capture_output=True, text=True)
            if result.stdout:
                logging.debug(f"CMDDBG:{cmdline}\n{result.stdout}")
            if result.stderr:
                logging.debug(f"CMDERR:{cmdline}\n{result.stderr}")
        except subprocess.CalledProcessError as e:
            logging.error(f"Error running command: {cmdline}")
            exit(1)

    def __init__(self, repo, base_dir):
        self.repo = repo
        self.base_dir = base_dir

    def clone_repository(self):
        name = self.repo.get('repo')
        url = self.repo.get('url')
        branch = self.repo.get('branch', 'main')
        commit = self.repo.get('commit')
        submodules = self.repo.get('submodules', {})
        destination = os.path.join(self.base_dir, self.repo['destination'])

        logging.info(f"Cloning {name} sources...")

        if not url or not destination:
            logging.error("Missing 'url' or 'destination' for a repository.")
            return

        os.makedirs(destination, exist_ok=True)

        if not os.path.exists(os.path.join(destination, ".git")):
            logging.info(f"Cloning {url} into {destination}...")
            self.run_command(["git", "clone", "--branch", branch, "--single-branch", url, destination])
        else:
            logging.info(f"Repository already exists at {destination}. Pulling latest changes...")
            self.run_command(["git", "-C", destination, "pull"])

        if commit:
            logging.info(f"Checking out commit {commit} in {destination}...")
            self.run_command(["git", "-C", destination, "checkout", commit])

        if submodules:
            submodule_command = ["git", "-C", destination, "submodule", "update", "--init"]
            if submodules == 'recursive':
                submodule_command.append("--recursive")
            logging.info(f"Initializing and updating submodules for {destination}...")
            self.run_command(submodule_command)

        TaskExecutor(self.repo, self.base_dir).execute_tasks()


class ArtifactManager:
    """Handles artifact downloads and extraction."""
    def __init__(self, artifact, base_dir):
        self.artifact = artifact
        self.base_dir = base_dir

    def download(self):
        url = self.artifact.get('url')
        destination = os.path.join(self.base_dir, self.artifact.get('destination', ''))

        if not url or not destination:
            logging.error("Missing 'url' or 'destination' for download.")
            return

        os.makedirs(os.path.dirname(destination), exist_ok=True)

        logging.info(f"Downloading {url} to {destination}...")
        with requests.get(url, stream=True, verify=False) as response:
            response.raise_for_status()
            with open(destination, 'wb') as file:
                for chunk in response.iter_content(chunk_size=8192):
                    file.write(chunk)

        TaskExecutor(self.artifact, self.base_dir).execute_tasks()


class BootstrapManager:
    """Orchestrates the entire bootstrapping process."""
    def __init__(self, yaml_file):
        if not os.path.exists(yaml_file):
            logging.error(f"The specified YAML file '{yaml_file}' does not exist.")
            exit(1)
        self.yaml_file = yaml_file

    def load_manifest(self):
        """Load the YAML manifest file."""
        try:
            with open(self.yaml_file, "r") as file:
                return yaml.safe_load(file)
        except yaml.YAMLError as e:
            logging.error(f"Error parsing YAML file: {e}")
            exit(1)

    def run(self):
        """Execute the bootstrap process."""
        config = self.load_manifest()

        project_dir = config.get('project')
        if not project_dir:
            logging.error("'project' key not found in the YAML manifest.")
            exit(1)

        os.makedirs(project_dir, exist_ok=True)

        repositories = config.get('repositories', [])
        for repo in repositories:
            RepoManager(repo, project_dir).clone_repository()

        artifacts = config.get('artifacts', [])
        for artifact in artifacts:
            ArtifactManager(artifact, project_dir).download()


def setup_logger(debug=False):
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG if debug else logging.INFO)

    formatter = ColoredFormatter(
        '%(log_color)s%(levelname)-8s%(reset)s %(message)s',
        log_colors={
            'DEBUG': 'cyan',
            'INFO': 'green',
            'WARNING': 'yellow',
            'ERROR': 'red',
            'CRITICAL': 'magenta',
        }
    )

    console_handler = logging.StreamHandler()
    console_handler.setFormatter(formatter)

    file_handler = logging.FileHandler('bootstrap.log', mode='w')
    file_handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))

    logger.addHandler(console_handler)
    logger.addHandler(file_handler)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Clone repositories from a YAML manifest file.")
    parser.add_argument("-f", "--file", default="manifest.yml", help="manifest YAML file (default: manifest.yml).")
    parser.add_argument("-d", "--debug", action='store_true', help="Set logging level to DEBUG")

    args = parser.parse_args()
    setup_logger(args.debug)
    BootstrapManager(args.file).run()
