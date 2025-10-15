# 🧠 xv6-syscalls-and-shell  
### 💻 Llamadas al sistema e implementación de una shell  
### 💻 System Calls and Shell Implementation

---

## 🇪🇸 Español

Este proyecto fue desarrollado como parte del curso **Sistemas Operativos** en la **Universidad de los Andes**.  
El objetivo fue extender el kernel educativo **xv6** mediante la implementación de una nueva **llamada al sistema** y una **shell** simple en C.

### 🔹 Funcionalidades

- **Llamada al sistema `getreadcount()`**:  
  Devuelve cuántas veces se ha invocado `read()` desde el arranque del sistema, con un contador protegido por spinlocks.

- **Shell (`nsh`)**:  
  Permite ejecutar comandos básicos, con soporte para:
  - Ejecución secuencial (`;`) y en paralelo (`&`)
  - Redirección de entrada/salida (`<`, `>`)
  - Pipes (`|`) entre comandos

### ⚙️ Compilación y ejecución

```bash
git clone https://github.com/ErikEckell/xv6-syscalls-and-shell.git
cd xv6-syscalls-and-shell
make qemu-nox
```

Salir de QEMU: `Ctrl + A`, luego `X`

### 🧪 Pruebas

```bash
./test-getreadcounts.sh   # Prueba la llamada getreadcount()
./testsh nsh              # Prueba la shell
```

### 👥 Autor

**Erik Eckell**  
Estudiante de Ingeniería Civil en Ciencias de la Computación  
Universidad de los Andes  
📫 [epeckell@miuandes.cl](mailto:epeckell@miuandes.cl)

---

## 🇬🇧 English

This project was developed as part of the **Operating Systems** course at **Universidad de los Andes**.  
It extends the educational **xv6** kernel by implementing a new **system call** and a simple **command shell** in C.

### 🔹 Features

- **System call `getreadcount()`**:  
  Returns the total number of times `read()` has been called since boot, using a spinlock-protected counter.

- **Shell (`nsh`)**:  
  Supports:
  - Sequential (`;`) and parallel (`&`) execution
  - Input/output redirection (`<`, `>`)
  - Command piping (`|`)

### ⚙️ Build and run

```bash
git clone https://github.com/ErikEckell/xv6-syscalls-and-shell.git
cd xv6-syscalls-and-shell
make qemu-nox
```

Exit QEMU: `Ctrl + A`, then `X`

### 🧪 Testing

```bash
./test-getreadcounts.sh   # Tests the getreadcount() syscall
./testsh nsh              # Tests the shell
```

### 👥 Author

**Erik Eckell**  
Computer Science Engineering Student  
Universidad de los Andes  
📫 [epeckell@miuandes.cl](mailto:epeckell@miuandes.cl)

---

### 🧾 Licencia / License

Basado en [xv6-public (MIT License)](https://github.com/mit-pdos/xv6-public)  
Educational use only.
