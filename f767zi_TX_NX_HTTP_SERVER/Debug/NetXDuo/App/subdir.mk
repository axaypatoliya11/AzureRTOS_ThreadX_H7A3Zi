################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NetXDuo/App/app_netxduo.c \
../NetXDuo/App/nx_md5.c \
../NetXDuo/App/nxd_http_server.c 

OBJS += \
./NetXDuo/App/app_netxduo.o \
./NetXDuo/App/nx_md5.o \
./NetXDuo/App/nxd_http_server.o 

C_DEPS += \
./NetXDuo/App/app_netxduo.d \
./NetXDuo/App/nx_md5.d \
./NetXDuo/App/nxd_http_server.d 


# Each subdirectory must supply rules for building sources it contributes
NetXDuo/App/%.o: ../NetXDuo/App/%.c NetXDuo/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I"C:/Users/150100/git_track_azurertos_h7a3/f767zi_TX_NX_HTTP_SERVER/NetXDuo/App" -I../AZURE_RTOS/App -I../NetXDuo/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

