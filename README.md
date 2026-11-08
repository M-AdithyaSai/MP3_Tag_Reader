# 🎵 MP3 Tag Reader and Editor Using C

A command-line-based **MP3 Tag Reader and Editor** developed using C programming to read, display, and modify ID3v2 metadata stored in MP3 audio files.

The application allows users to view essential MP3 metadata and update individual tags through command-line arguments using binary file handling.

## 📌 Project Overview

MP3 files contain metadata known as ID3 tags, which store information about an audio file, such as song title, artist, album, release year, genre, and comments.

This project implements an ID3v2 tag reader and editor that allows users to extract and modify these metadata fields without altering the actual audio content.

## ✨ Features

### 📖 View MP3 Metadata
- Read and display MP3 ID3v2 metadata.
- Extract song title.
- Display artist name.
- Display album name.
- Retrieve release year.
- Display music genre and comments.

### ✏️ Edit MP3 Metadata
- Modify song title.
- Update artist name.
- Change album name.
- Update release year.
- Modify music genre.
- Edit comments.

### 🛡️ Additional Functionalities
- Validate command-line arguments.
- Verify MP3 ID3 headers.
- Identify and process ID3v2 frames.
- Handle binary file operations.
- Use temporary file handling during metadata modification.
- Preserve unchanged metadata frames and audio data.

## 🛠️ Technologies & Concepts Used

- **Programming Language:** C
- Structures and Pointers
- File Handling (Binary File Operations)
- Dynamic Memory Allocation
- String Manipulation
- Command-Line Argument Processing
- Modular Programming
- ID3v2 Metadata Processing
- Big-Endian and Synchsafe Integer Handling

## ⚙️ Understanding ID3v2 Metadata

ID3v2 is a metadata format used in MP3 files to store information about audio tracks.

Each metadata field is stored in a structure called a **Frame**.

Some commonly used ID3v2 frame identifiers are:

| Frame ID | Description |
|---|---|
| TIT2 | Song Title |
| TPE1 | Artist Name |
| TALB | Album Name |
| TYER | Release Year |
| TCON | Music Genre |
| COMM | Comments |

The application identifies these frames and reads or updates their corresponding data.

## 📂 Project Structure

```text
MP3_Tag_Reader/
│
├── main.c          # Main function and command-line operations
│
├── mp3view.c       # Reading and displaying MP3 metadata
├── mp3view.h       # View module declarations
│
├── mp3edit.c       # Editing MP3 metadata
├── mp3edit.h       # Edit module declarations
│
├── types.h         # Structures, enums and status definitions
│
├── sample.mp3      # Sample MP3 input file
│
└── README.md       # Project documentation
```

## 🚀 Compilation and Execution

### Prerequisites

- GCC Compiler
- MP3 audio file containing ID3v2 metadata
- Linux environment or Windows with a compatible GCC toolchain

### 1. Clone the Repository

```bash
git clone https://github.com/M-AdithyaSai/MP3_Tag_Reader.git
```

### 2. Navigate to the Project Directory

```bash
cd MP3_Tag_Reader
```

### 3. Compile the Source Files

```bash
gcc main.c mp3view.c mp3edit.c -o mp3tag
```

### 4. View MP3 Metadata

Use the `-v` option to display the metadata of an MP3 file.

```bash
./mp3tag -v sample.mp3
```

### 5. Edit MP3 Metadata

Use the `-e` option followed by the specific tag option and the new value.

**Change Song Title:**

```bash
./mp3tag -e -t "New Song Title" sample.mp3
```

**Change Artist Name:**

```bash
./mp3tag -e -a "New Artist" sample.mp3
```

**Change Album Name:**

```bash
./mp3tag -e -A "New Album" sample.mp3
```

**Change Release Year:**

```bash
./mp3tag -e -y "2026" sample.mp3
```

**Change Music Genre:**

```bash
./mp3tag -e -m "Rock" sample.mp3
```

**Change Comments:**

```bash
./mp3tag -e -c "My Favourite Song" sample.mp3
```

### 6. Display Help Menu

```bash
./mp3tag --help
```

## 📋 Command-Line Options

| Option | Operation |
|---|---|
| `-v` | View MP3 metadata |
| `-e` | Edit MP3 metadata |
| `-t` | Edit song title |
| `-a` | Edit artist name |
| `-A` | Edit album name |
| `-y` | Edit release year |
| `-m` | Edit music genre |
| `-c` | Edit comments |
| `--help` | Display help menu |

## 🔄 Working Process

### View Operation

1. Accept the MP3 filename through command-line arguments.
2. Validate the input arguments.
3. Open the MP3 file.
4. Verify the ID3v2 header.
5. Read the metadata frames.
6. Extract the corresponding tag information.
7. Display the metadata.

### Edit Operation

1. Accept the MP3 filename, tag option, and new value.
2. Validate the command-line arguments.
3. Identify the corresponding ID3v2 frame.
4. Open the MP3 file for processing.
5. Read the existing metadata frames.
6. Update the selected metadata field.
7. Copy unchanged frames and audio data into a temporary file.
8. Update the ID3 tag size.
9. Replace the original file with the modified file.

## 🎯 Learning Outcomes

Through this project, I gained practical experience in:

- Understanding the internal structure of MP3 files.
- Working with ID3v2 metadata and frame identifiers.
- Performing binary file read and write operations.
- Understanding Big-Endian representation and Synchsafe integers.
- Using pointers and structures for metadata processing.
- Implementing command-line-based applications.
- Managing temporary files during binary file modification.
- Developing modular applications using C.

## 👨‍💻 Author

**M Adithya Sai**

GitHub: [M-AdithyaSai](https://github.com/M-AdithyaSai)

**Project Repository:** [MP3 Tag Reader](https://github.com/M-AdithyaSai/MP3_Tag_Reader)

---

⭐ If you find this project interesting, consider giving the repository a star!
