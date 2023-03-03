# Embedded application template

## Requirements

None

## Usage

* To clone **and** deploy the project in one command, use `mbed import`:

    ```sh
    mbed import https://gitlab.com/catie_estia-students/mbed-os-basic-template.git YOUR_PROJECT_NAME
    ```

* Define your target (eg. `ZEST_CORE_STM32L4A6RG`) and toolchain:

    ```sh
    mbed target ZEST_CORE_STM32L4A6RG
    mbed toolchain GCC_ARM
    ```

* Export to VS Code IDE with:

    ```sh
    mbed export -i vscode_gcc_arm
    ```

## Compiling and programming without IDE

* Compile the project:

    ```sh
    mbed compile
    ```

* Program the target device (eg. `STM32L4A6RG` for the Zest_Core_STM32L4A6RG) with a
  J-Link debug probe:

    ```sh
     sixtron_flash stm32l4a6rg ./BUILD/ZEST_CORE_STM32L4A6RG/GCC_ARM/mbed-os-basic-template.bin
    ```

## Manage and save your project with Git

* Edit `README.md` file

* Modify remote URL to point to your repository and push the application template:

    ```sh
    git remote set-url origin YOUR_REPOSITORY_URL
    git push -u origin master
    ```
