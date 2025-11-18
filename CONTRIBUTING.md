# Contributing to Solar-LiFi

Thank you for your interest in contributing to the Solar-LiFi project! We welcome contributions from everyone.

## How to Contribute

### Reporting Bugs

Found a bug? Please open an issue with:
- Clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- Hardware setup (Arduino model, ESP8266 variant, etc.)
- Code modifications (if any)
- Photos or screenshots (if relevant)

### Suggesting Enhancements

Have an idea? Open an issue with:
- Clear description of the enhancement
- Why it would be useful
- Possible implementation approach
- Any drawbacks or considerations

### Pull Requests

We love pull requests! Here's how to submit one:

1. **Fork the repository**
2. **Create a branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Update documentation if needed
4. **Test your changes**
   - Verify code compiles
   - Test on actual hardware if possible
   - Document test results
5. **Commit with clear messages**
   ```bash
   git commit -m "Add feature: description of what you did"
   ```
6. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Open a Pull Request**
   - Describe what you changed and why
   - Reference any related issues
   - Include test results

## Areas for Contribution

### Code Improvements
- [ ] Bidirectional communication
- [ ] Error correction (CRC, Hamming codes)
- [ ] Encryption support
- [ ] Multiple receiver support
- [ ] Adaptive bit rate
- [ ] Automatic gain control
- [ ] Web interface for ESP8266

### Documentation
- [ ] Video tutorials
- [ ] More circuit diagrams
- [ ] Language translations
- [ ] Project showcase/gallery
- [ ] Performance benchmarks
- [ ] Case studies

### Hardware
- [ ] PCB designs
- [ ] 3D printable enclosures
- [ ] Alternative component guides
- [ ] Solar panel sizing calculator
- [ ] Battery management system

### Testing
- [ ] Different laser types
- [ ] Various photodiodes
- [ ] Range measurements
- [ ] Speed benchmarks
- [ ] Environmental testing

## Code Style Guidelines

### Arduino/C++ Code

```cpp
// Use descriptive names
#define LASER_PIN 9  // Good
#define LP 9         // Bad

// Comment your functions
/**
 * Brief description of what function does
 * 
 * @param paramName Description of parameter
 * @return Description of return value
 */
void functionName(int paramName) {
  // Implementation
}

// Use consistent indentation (2 spaces)
void setup() {
  Serial.begin(9600);
  if (condition) {
    doSomething();
  }
}

// Add safety checks
if (index < MAX_SIZE) {
  array[index] = value;
}
```

### Documentation

- Use clear, concise language
- Include code examples where relevant
- Add diagrams for complex concepts
- Keep line length < 100 characters
- Use proper markdown formatting

### Git Commits

Good commit messages:
```
Add feature: bidirectional communication support
Fix bug: incorrect threshold calculation
Update docs: add troubleshooting section
Refactor: improve message encoding efficiency
```

Bad commit messages:
```
updated stuff
fixed it
changes
asdf
```

## Testing Requirements

Before submitting:
- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] Tested on actual hardware (if possible)
- [ ] Documentation updated
- [ ] Examples updated (if relevant)
- [ ] No security vulnerabilities introduced

## Community Guidelines

- Be respectful and constructive
- Help others learn
- Share your experiences
- Give credit where due
- Follow the [Code of Conduct](CODE_OF_CONDUCT.md)

## Recognition

Contributors will be:
- Listed in project credits
- Mentioned in release notes
- Part of an awesome community!

## Questions?

Not sure where to start? Ideas:
- Look for issues tagged "good first issue"
- Ask questions in existing issues
- Check the discussion board
- Improve documentation (always needed!)

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Thank You!

Every contribution, no matter how small, helps make this project better. Thank you for being part of the Solar-LiFi community! 🌟

---

Happy Contributing! 🚀
