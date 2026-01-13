# 🎮 cub3D - A 42 School Raycasting Project

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![42](https://img.shields.io/badge/42-000000?style=for-the-badge&logo=42&logoColor=white)

*A 3D graphical representation of the inside of a maze using raycasting, inspired by the legendary Wolfenstein 3D game.*

</div>

---

## 📖 Table of Contents

- [About The Project](#-about-the-project)
- [What is Raycasting?](#-what-is-raycasting)
- [Raycasting vs Raytracing](#-raycasting-vs-raytracing)
- [The DDA Algorithm](#-the-dda-algorithm-digital-differential-analyzer)
- [Code Architecture](#-code-architecture)
- [How To Run](#-how-to-run)
- [Controls](#-controls)
- [Map Format](#-map-format)

---

## 🎯 About The Project

**cub3D** is a 42 School project that challenges students to create a 3D graphical representation of the inside of a maze from a first-person perspective using the **Raycasting** technique. This project is inspired by the world-famous **Wolfenstein 3D** (1992), which was one of the first FPS games ever created and used raycasting for its pseudo-3D rendering.

### Features

- **First-Person View**: Navigate through a 3D maze from a first-person perspective
- **Textured Walls**: Different textures for each wall orientation (North, South, East, West)
- **Customizable Colors**: Configure ceiling and floor colors via the map file
- **Smooth Movement**: Walk forward/backward, strafe left/right, and rotate the view
- **Collision Detection**: Prevents walking through walls
- **Distance Shading**: Walls appear darker the further away they are

---

## 🔦 What is Raycasting?

**Raycasting** is a rendering technique used to create a pseudo-3D perspective in a 2D map. It works by casting rays from the player's position in the direction they're looking. For each vertical stripe of the screen, a ray is cast, and when it hits a wall, the distance is used to calculate how tall that wall slice should appear.

### How It Works

```
                    Wall
                     ║
    Player ●─────────║
           ─────────►║
              Ray    ║
                     ║
```

1. **Cast a ray** from the player's position at a specific angle
2. **Find where** the ray intersects with a wall
3. **Calculate the distance** from the player to the wall
4. **Draw a vertical stripe** on the screen with height inversely proportional to the distance

### Real-World Applications

Raycasting is used in:
- **Classic video games** (Wolfenstein 3D, Duke Nukem 3D, early Doom engines)
- **Map rendering** and field-of-view calculations
- **Robotics** for obstacle detection and navigation
- **Computer vision** applications
- **Light and shadow calculations** in 2D games
- **Line-of-sight algorithms** in strategy games

---

## ⚔️ Raycasting vs Raytracing

While they sound similar, these are fundamentally different techniques:

| Aspect | Raycasting | Raytracing |
|--------|------------|------------|
| **Purpose** | Pseudo-3D rendering, collision detection | Photorealistic rendering |
| **Rays Cast** | One ray per screen column | Multiple rays per pixel |
| **Complexity** | O(resolution) | O(resolution × bounces × samples) |
| **Real-time** | Yes, very fast | Traditionally slow, now GPU-accelerated |
| **Light Simulation** | None or basic | Full light physics (reflection, refraction, shadows) |
| **Output** | 2.5D appearance | True 3D with realistic lighting |
| **Hardware** | Runs on 1990s computers | Requires modern GPUs for real-time |

### Visual Comparison

```
RAYCASTING (This Project)           RAYTRACING (Photorealistic)
┌──────────────────────┐            ┌──────────────────────┐
│  Simple geometry     │            │  Complex geometry    │
│  Flat-shaded walls   │            │  Realistic shadows   │
│  No reflections      │            │  Mirror reflections  │
│  Fast rendering      │            │  Expensive computing │
│  2.5D pseudo-3D      │            │  True 3D rendering   │
└──────────────────────┘            └──────────────────────┘
```

**Key Difference**: Raycasting casts one ray per column and stops at the first wall hit, while raytracing casts multiple rays per pixel that bounce off surfaces to simulate realistic light behavior.

---

## 📐 The DDA Algorithm (Digital Differential Analyzer)

The **DDA algorithm** is the heart of our raycasting engine. It's an efficient line-drawing algorithm adapted here to find where a ray intersects with walls in our grid-based map.

### Why DDA?

Instead of checking every tiny step along a ray (slow), DDA jumps directly to grid boundaries, making it extremely efficient.

### How DDA Works in cub3D

```
     Grid Map                    DDA Steps
    ┌───┬───┬───┬───┐           
    │   │   │ █ │   │           Step 1: Start at player position
    ├───┼───┼───┼───┤           Step 2: Jump to first vertical grid line
    │   │ ● │───┼───┤ ← Ray     Step 3: Check if wall hit? No
    ├───┼───┼───┼───┤           Step 4: Jump to next grid boundary
    │   │   │   │   │           Step 5: Check if wall hit? Yes! (█)
    └───┴───┴───┴───┘           Step 6: Calculate distance
      ●  = Player
      █  = Wall hit
```

### The Algorithm Explained

```c
// 1. Calculate delta distances (distance between grid lines along ray)
delta_dist_x = fabs(1.0f / ray_dir_x);
delta_dist_y = fabs(1.0f / ray_dir_y);

// 2. Determine step direction and initial side distances
if (ray_dir_x < 0) {
    step_x = -1;
    side_dist_x = (pos_x - map_x) * delta_dist_x;
} else {
    step_x = 1;
    side_dist_x = (map_x + 1.0f - pos_x) * delta_dist_x;
}
// Same for Y direction...

// 3. DDA Loop - Jump to grid boundaries
while (!hit_wall) {
    if (side_dist_x < side_dist_y) {
        side_dist_x += delta_dist_x;  // Jump to next X boundary
        map_x += step_x;
        side = 0;  // Hit was on X side (vertical wall)
    } else {
        side_dist_y += delta_dist_y;  // Jump to next Y boundary
        map_y += step_y;
        side = 1;  // Hit was on Y side (horizontal wall)
    }
    
    if (map[map_y][map_x] == '1')
        hit_wall = true;
}
```

### Key Variables

| Variable | Description |
|----------|-------------|
| `delta_dist_x/y` | Distance the ray travels to cross one grid cell |
| `side_dist_x/y` | Distance from current position to next grid boundary |
| `step_x/y` | Direction of the ray (-1 or +1) |
| `map_x/y` | Current grid cell being checked |
| `side` | Which side of the wall was hit (vertical/horizontal) |

---

## 📂 Code Architecture

```
cub3D/
├── lib.h                 # Main header file with all structures and prototypes
├── Makefile              # Build configuration
├── src/
│   ├── main.c           # Entry point, MLX initialization, event handling
│   ├── player.c         # Player movement, DDA algorithm, 3D rendering
│   ├── map.c            # Map-related utilities
│   └── loading_textures.c # Texture loading logic
├── parsing/
│   ├── parsing_v0.c     # File parsing initialization
│   ├── parsing_v1.c     # Texture path parsing
│   ├── parsing_v2.c     # Color parsing
│   ├── parsing_v3.c     # Map validation
│   ├── utils_v0.c       # Parsing utilities
│   └── utils_v1.c       # Additional utilities
├── libft/               # Custom C library functions
├── maps/
│   └── map.cub          # Sample map file
└── textures/            # XPM texture files for walls
```

### Core Components

#### 1. Main Structures (`lib.h`)

```c
typedef struct s_player {
    float p_x, p_y;           // Player position
    float rotation_angle;      // View direction
    float fov;                 // Field of view (60°)
    t_ray *rays;              // Array of rays
    // ...
} t_player;

typedef struct s_ray {
    float ray_angle;          // Direction of this ray
    float wall_hit_x, y;      // Where ray hit wall
    float distance;           // Distance to wall
    int wall_face;            // N/S/E/W for texture selection
    // ...
} t_ray;
```

#### 2. Raycasting Flow (`player.c`)

```
render_all()
    └── adjusting_rays()     # Set up all rays across FOV
        └── cast()           # DDA algorithm for each ray
            └── perform_dda()
    └── render_3D_projection_walls()  # Draw the 3D view
        └── draw_ceiling()
        └── draw_wall_slice()  # Textured wall rendering
        └── draw_floor()
```

#### 3. Fisheye Correction

```c
// Remove fisheye distortion using perpendicular distance
float perp_distance = ray->distance * cos(ray->ray_angle - player_angle);
```

---

## 🚀 How To Run

### Prerequisites

- **Linux/WSL** environment
- **GCC** compiler
- **Make** build tool
- **MiniLibX** graphics library
- X11 development libraries

### Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/cub3D.git
cd cub3D

# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install gcc make libx11-dev libxext-dev libbsd-dev

# Build the project
make

# Run with a map
./cub3D maps/map.cub
```

### Build Commands

| Command | Description |
|---------|-------------|
| `make` | Compile the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executable |
| `make re` | Rebuild from scratch |

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move forward |
| `S` / `↓` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `←` | Rotate view left |
| `→` | Rotate view right |
| `ESC` | Exit game |

---

## 🗺️ Map Format

Maps use the `.cub` extension and follow this format:

```
NO textures/north.xpm     # North wall texture
SO textures/south.xpm     # South wall texture
EA textures/east.xpm      # East wall texture
WE textures/west.xpm      # West wall texture

C 100,0,33                 # Ceiling color (R,G,B)
F 60,179,200               # Floor color (R,G,B)

11111111111111111111
10000000000000000001
10000N00000000000001        # N = Player starting position, facing North
10001111110000000001        # S, E, W = Other directions
11111111111111111111

# Map Legend:
# 1 = Wall
# 0 = Empty space
# N/S/E/W = Player start position and orientation
```

### Map Rules

- Must be enclosed by walls (`1`)
- Exactly one player position (N, S, E, or W)
- Spaces inside the map are treated as errors
- All 4 wall textures must be specified
- RGB values must be 0-255

---

## 📚 Resources

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) - Essential reading
- [Permadi's Raycasting Tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [42 Docs - cub3D](https://harm-smits.github.io/42docs/projects/cub3d)
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)

---

## 👥 Authors

- **Ilyas** - *Main Developer*

---

<div align="center">

Made with ❤️ at 42

</div>
