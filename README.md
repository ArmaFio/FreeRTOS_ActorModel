# Basic Actor System on FreeRTOS

This repository contains a **basic implementation of an actor-based system** where **each message is represented as a task**, built on top of **FreeRTOS**.

## 📋 Description

In this design, messages are not just data packets — they are active entities encapsulated as tasks. Each message-task carries the logic required to process itself and interact with other actors or components. This approach offers a highly decoupled and flexible actor-like system, leveraging FreeRTOS primitives.

The implementation is intended as a minimal example or prototype for experimenting with concurrency models in embedded systems.

## 🚀 Features

- Message-driven architecture
- Each message is implemented as an independent task
- Encapsulation of behavior within messages
- Based on FreeRTOS for real-time scheduling
