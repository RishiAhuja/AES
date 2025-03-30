class Axon < Formula
    desc "AES file encryption and decryption tool"
    homepage "https://github.com/RishiAhuja/axon"
    url "https://github.com/RishiAhuja/axon/archive/v1.0.0.tar.gz"
    sha256 "YOUR_CHECKSUM_HERE" # Replace with actual SHA256 checksum
    
    depends_on "cmake" => :build
    
    def install
      system "mkdir", "-p", "build"
      system "cd", "build", "&&", "cmake", "..", "&&", "make"
      bin.install "build/axon"
      man1.install "man/axon.1"
    end
    
    test do
      system "#{bin}/axon", "--version"
    end
  end