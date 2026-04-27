# Tank Attack!
Proyecto 2 — CE 2103 Algoritmos y Estructuras de Datos II  
Instituto Tecnológico de Costa Rica — I Semestre 2026  
Autores: Paulo Centeno Flores y Máyerlin Dilana Gamboa González

---

## Requisitos

- Visual Studio Community 2022 (versión 17.14.15)
- Windows 10 / 11 (64 bits)
- raylib 6.0

---

## Instalación de raylib

1. Descargar `raylib-6.0_win64_msvc16.zip` desde https://github.com/raysan5/raylib/releases
2. Descomprimir y mover la carpeta a `C:\raylib`  
   Debe quedar así:
   ```
   C:\raylib\include\
   C:\raylib\lib\
   ```
3. Abrir el proyecto en Visual Studio 2022
4. Click derecho sobre el proyecto → Propiedades → Todas las configuraciones
5. **C/C++ → General → Directorios de inclusión adicionales:**  
   Agregar `C:\raylib\include`
6. **Vinculador → General → Directorios de bibliotecas adicionales:**  
   Agregar `C:\raylib\lib`
7. **Vinculador → Entrada → Dependencias adicionales:**  
   Agregar al inicio: `raylib.lib;winmm.lib;`

---

## Estructura del mapa

| Parámetro | Valor |
|-----------|-------|
| Tamaño | 20 x 20 celdas |
| Tamaño visual por celda | 32 x 32 píxeles |
| Tamaño de ventana | 640 x 640 píxeles |

---

## Estructura de carpetas

```
TankAttack/
├── TankAttack/
│   ├── Celda.h
│   ├── Grafo.h
│   ├── Mapa.h
│   ├── Renderer.h
│   └── main.cpp
└── README.md
```
