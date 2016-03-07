require 'uri'
require 'erb'
require 'json'

arduino = "/Applications/Arduino.app/Contents/MacOS/Arduino"
arduino_folder = "/Applications/Arduino.app/Contents/Java"
builder = "#{arduino_folder}/arduino-builder"
Target = "Target"

options = [
    "-hardware #{arduino_folder}/hardware",
    "-hardware #{ENV['HOME']}/Library/Arduino15/packages",
    #"-hardware Firmware/sketchbook/hardware",

    "-tools #{arduino_folder}/tools-builder",
    "-tools #{arduino_folder}/hardware/tools/avr",
    "-tools #{ENV['HOME']}/Library/Arduino15/packages",

    "-libraries Firmware/sketchbook/libraries",

    #"-warnings default",
    "-verbose"
].join(" ")

boards = [
    "-fqbn=adafruit:samd:adafruit_feather_m0",
    # "-fqbn=breadboard:avr:atmega328bb",
]

libraries = [
    ["zip", "http://ftp-stud.fht-esslingen.de/pub/Mirrors/eclipse/paho/arduino_1.0.0.zip"],

    ["git", "https://github.com/adafruit/Adafruit_MQTT_Library.git"],
    ["git", "https://github.com/adafruit/Adafruit-GFX-Library.git","v1.1.4"],
    ["git", "https://github.com/adafruit/Adafruit_INA219.git"],

    ["git", "https://github.com/bblanchon/ArduinoJson.git","v5.0.7"],

    ["git", "https://github.com/google/googletest.git"],
]

def installGit(url , branch = nil)
    puts "** install git #{url} #{branch} **"
    cmd = "git clone #{url}"
    cmd += " --branch #{branch}" if branch
    sh cmd
end

def installArchive(url)
    puts "** install archive #{url} **"
    uri = URI.parse(url)
    archive = File.basename(uri.path)
    sh "wget #{url}"
    sh "unzip #{archive}"
    rm archive
end

def installLibrary(library)
    puts "** install #{library.join(' ')} **"
    case library[0]
        when "zip" then installArchive(library[1])
        when "git" then installGit(library[1],library[2])
        else puts("do not know how to install #{library}")
    end
end

task :install do
    library_path = "Firmware/sketchbook/libraries"
    rm_r library_path , :force => true
    mkdir library_path
    Dir.chdir(library_path) do
        libraries.each do |library|
            installLibrary(library)
        end
    end
    Dir.entries("Firmware/libraries").each do |library|
        next if (library =='.' || library == '..' || library.start_with?("."))
        puts "** link #{library} **"
        name = File.basename(library)
        ln_s "../../libraries/#{name}", "#{library_path}/"
    end
end


desc "Compile all"
task :compile do
    sketches = Dir.glob("Firmware/sketchbook/*/*.ino")
    sketches.each do |sketch|
        boards.each do |board|
            puts "compile #{sketch} ..."
            verbose(false) do
                sh "#{builder} #{options} #{board} #{sketch}"
            end
            puts "   ...  #{sketch} done"
        end
    end
    puts "************"
    puts "compiled #{sketches.count} sketches for #{boards.count} boards"
end

task :upload do
    sh "#{arduino} --board adafruit:samd:adafruit_feather_m0 --pref sketchbook.path=#{Dir.pwd}/Firmware/sketchbook --upload --port /dev/cu.usbmodem1411 -v Firmware/sketchbook/ResourceCheck/ResourceCheck.ino"
end


task :default => :compile

# ---

namespace :host do

  task :compile_gtest do
    gtest_dir = "Firmware/sketchbook/libraries/googletest/googletest"
    sh "g++ -isystem #{gtest_dir}/include -I#{gtest_dir} -pthread -c #{gtest_dir}/src/gtest-all.cc -o Target/host/gtest-all.o" unless File.exists?("Target/host/gtest-all.o")
    sh "ar -rv Target/host/libgtest.a Target/host/gtest-all.o" unless File.exists?("Target/host/libgtest.a")
  end

  desc "compile all host tests"
  task :compile => :compile_gtest do
    libraries_with_tests = Rake::FileList["Firmware/libraries/*/test"]
    libraries_with_tests.collect{|path| path.pathmap('%d')}.each do |library|
      compile_host_library(library)
      compile_host_test(library)
    end
  end

  desc "compile all host tests"
  task :test => :compile do
    libraries_with_tests = Rake::FileList["Firmware/libraries/*/test"]
    puts libraries_with_tests
    libraries_with_tests.collect{|path| path.pathmap('%d')}.each do |library|
        name = library.pathmap('%n')
        sh "Target/host/#{name}"
    end
  end

end

def compile_host_library(library_path)


  name = library_path.pathmap('%n')
  target = "#{Target}/host"
  sources = Rake::FileList[library_path + "/src/**/*.cpp",library_path + "/src/**/*.c"]
  includes = [library_path + "/src"]
  defines = []

  json_info_file = "#{library_path}/.library.json"
  if(File.exists?(json_info_file))
    info = JSON.parse(IO.read(json_info_file))
    info["dependencies"].each do |dependency|
      includes << "Firmware/sketchbook/libraries/#{dependency}/src"
      includes << "Firmware/sketchbook/libraries/#{dependency}"
    end
  end

  ninja_path = library_path.pathmap("%{^Firmware,#{target}}X/src")
  ninja_file = "#{ninja_path}/build.ninja"
  FileUtils.mkdir_p(ninja_path) unless Dir.exists?(ninja_path)
  File.open(ninja_file, "w+") do |file|
    file.write(ERB.new(IO.read("Build/host_rules.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_compile.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_library.erb")).result(binding))
  end

  sh "/usr/local/bin/ninja -v -j8 -f #{ninja_file}"
end

def compile_host_test(library_path)
  name = library_path.pathmap('%n')
  target = "#{Target}/host"
  sources = Rake::FileList[library_path + "/test/**/*.cpp",library_path + "/test/**/*.c"]
  includes = [library_path + "/src", "Firmware/sketchbook/libraries/googletest/googletest/include", "Firmware/sketchbook/libraries/googletest/googlemock/include/gmock"]
  defines = []
  libraries = ["Target/host/libgtest.a"]
  external_libraries = []

  ninja_path = library_path.pathmap("%{^Firmware,#{target}}X/test")
  ninja_file = "#{ninja_path}/build.ninja"
  FileUtils.mkdir_p(ninja_path) unless Dir.exists?(ninja_path)
  File.open(ninja_file, "w+") do |file|
    file.write(ERB.new(IO.read("Build/host_rules.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_compile.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_link.erb")).result(binding))
  end
  sh "/usr/local/bin/ninja -v -j8 -f #{ninja_file}"
end
