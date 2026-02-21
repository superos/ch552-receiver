#!/usr/bin/env python3
"""
USB Device Name Updater for CH552
Updates the ProductDescriptor in USBconstant.c with a new device name.

Usage:
    python SetDeviceName.py "G80-RECEIVER"
    python SetDeviceName.py "My Keyboard"
"""

import re
import sys
from pathlib import Path


def update_usb_name(file_path: Path, new_name: str) -> bool:
    """
    Update the USB device name in USBconstant.c
    
    Args:
        file_path: Path to USBconstant.c
        new_name: New device name (ASCII only)
    
    Returns:
        True if successful, False otherwise
    """
    # Validate input
    if not new_name:
        print("Error: Device name cannot be empty")
        return False
    
    if not all(ord(c) < 128 for c in new_name):
        print("Error: Device name must contain only ASCII characters")
        return False
    
    if len(new_name) > 32:
        print("Error: Device name too long (max 32 characters)")
        return False
    
    # Read file
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: File not found: {file_path}")
        return False
    except Exception as e:
        print(f"Error reading file: {e}")
        return False
    
    # Generate new descriptor
    char_count = len(new_name)
    descriptor_lines = [f"    ((({char_count} + 1) * 2) | (DTYPE_String << 8)),"]
    
    # Add each character on a new line
    for char in new_name:
        descriptor_lines.append(f"    '{char}',")
    
    new_descriptor = '\n'.join(descriptor_lines)
    
    # Find and replace ProductDescriptor
    pattern = re.compile(
        r'(__code uint16_t ProductDescriptor\[\] = \{)\s*\n'
        r'.*?'
        r'(\n\};)',
        re.DOTALL
    )
    
    match = pattern.search(content)
    if not match:
        print("Error: Could not find ProductDescriptor in file")
        return False
    
    # Replace with new content
    new_content = pattern.sub(
        r'\1\n' + new_descriptor + r'\2',
        content
    )
    
    # Write back
    try:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
    except Exception as e:
        print(f"Error writing file: {e}")
        return False
    
    print(f"✓ Successfully updated USB device name to: \"{new_name}\"")
    print(f"✓ Character count: {char_count}")
    print(f"✓ Descriptor size: {(char_count + 1) * 2} bytes")
    return True


def main():
    # Check arguments
    if len(sys.argv) != 2:
        print("Usage: python SetDeviceName.py \"DEVICE_NAME\"")
        print("\nExamples:")
        print("  python SetDeviceName.py \"G80-RECEIVER\"")
        print("  python SetDeviceName.py \"My Custom Device\"")
        print("\nNote:")
        print("  - Device name must be ASCII characters only")
        print("  - Maximum length: 32 characters")
        sys.exit(1)
    
    new_name = sys.argv[1]
    
    # Resolve file path
    file_path = Path(__file__).parent / 'src' / 'CdcHidCombo' / 'USBconstant.c'
    
    # Update USB name
    success = update_usb_name(file_path, new_name)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
