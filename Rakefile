require 'uri'
require 'erb'
require 'json'
require 'rake/clean'
require 'digest/sha1'


$cc = ENV['CC'] || 'gcc'
$cxx = ENV['CXX'] || 'g++'

puts "$cc is #{$cc}"
puts "$cxx is #{$cxx}"

$arduino = "/Applications/Arduino.app/Contents/MacOS/Arduino"
$arduino_folder = "/Applications/Arduino.app/Contents/Java"
$builder = "#{$arduino_folder}/arduino-builder"
$output_folder = "Firmware/Target"

$options = [
    "-hardware #{$arduino_folder}/hardware",
    "-hardware #{ENV['HOME']}/Library/Arduino15/packages",
    #"-hardware Firmware/sketchbook/hardware",

    "-tools #{$arduino_folder}/tools-builder",
    "-tools #{$arduino_folder}/hardware/tools/avr",
    "-tools #{ENV['HOME']}/Library/Arduino15/packages",

    "-libraries Firmware/sketchbook/libraries",

    #"-warnings default",
    #"-verbose"
].join(" ")

$boards = [
    "-fqbn=adafruit:samd:adafruit_feather_m0",
    "-fqbn=\"Breadboard Arduino:avr:atmega328bb\"",
]

$libraries = [
    ["zip", "http://ftp-stud.fht-esslingen.de/pub/Mirrors/eclipse/paho/arduino_1.0.0.zip"],
    ["zip", "https://github.com/arduino-libraries/RTCZero/archive/1.5.0.zip"],


    ["git", "https://github.com/adafruit/Adafruit_MQTT_Library.git"],
    ["git", "https://github.com/adafruit/Adafruit-GFX-Library.git","v1.1.4"],
    ["git", "https://github.com/adafruit/Adafruit_INA219.git"],

    ["git", "https://github.com/rocketscream/Low-Power.git","V1.6"],


    ["git", "https://github.com/bblanchon/ArduinoJson.git","v5.0.7"],

    ["git", "https://github.com/google/googletest.git", "release-1.7.0"],
    ["git", "https://github.com/google/googlemock.git", "release-1.7.0"],
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
        $libraries.each do |library|
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


desc "Compile all arduino sketches"
task :compile_arduino_all


task :settings do
  Dir.glob("Firmware/sketchbook/*/*.default_settings.*").each do |default_setting|
    setting = default_setting.gsub('.default_settings.','.settings.')
    if(!File.exists?(setting))
      FileUtils.cp(default_setting,setting)
    end
  end
end


sketches = Dir.glob("Firmware/sketchbook/*/*.ino")
sketches.each do |sketch|
  sketch_name = File.basename(sketch,".ino")
  desc "compile the #{sketch_name} sketch"
  task "compile_arduino_#{sketch_name}" => :settings do
    build_info = {};
    build_info_file = sketch.pathmap("%d") + "/.build.json"
    if(File.exists?(build_info_file))
      build_info = JSON.parse(IO.read(build_info_file))
    end
    $boards.each do |board|
        if build_info['boardRegex'] and !Regexp.new(build_info['boardRegex']).match(board)
          # puts ""
          # puts "!!! skip #{sketch_name} for #{board} ... "
          # puts ""
          next
        end


        build_path = File.absolute_path("#{$output_folder}/sketches/"  + Digest::SHA1.hexdigest("#{sketch}-#{board}"))
        FileUtils.mkdir_p(build_path) unless Dir.exists?(build_path)
        puts ""
        puts "*** compile #{sketch_name} for #{board} ... "
        puts "#{$builder} #{$options} -build-path #{build_path} #{board} #{sketch}"
        verbose(false) do
            sh "#{$builder} #{$options} -build-path #{build_path} #{board} #{sketch}"
        end
        puts ""
    end
  end

  task :compile_arduino_all => "compile_arduino_#{sketch_name}"

end



task :upload do
    sh "#{arduino} --board adafruit:samd:adafruit_feather_m0 --pref sketchbook.path=#{Dir.pwd}/Firmware/sketchbook --upload --port /dev/cu.usbmodem1411 -v Firmware/sketchbook/ResourceCheck/ResourceCheck.ino"
end


task :default => :compile_arduino_all

# ---

$host_output_folder = "#{$output_folder}/host"

CLEAN.include($output_folder)

namespace :host do

  directory $host_output_folder

  task :compile_gtest => $host_output_folder do
    gtest_dir = "Firmware/sketchbook/libraries/googletest"
    sh "#{$cxx} -isystem #{gtest_dir}/include -I#{gtest_dir} -pthread -c #{gtest_dir}/src/gtest-all.cc -o #{$host_output_folder}/gtest-all.o" unless File.exists?("#{$host_output_folder}/gtest-all.o")
    sh "ar -rv #{$host_output_folder}/libgtest.a #{$host_output_folder}/gtest-all.o" unless File.exists?("#{$host_output_folder}/libgtest.a")

    gmock_dir = "Firmware/sketchbook/libraries/googlemock"
    sh "#{$cxx} -isystem #{gmock_dir}/include -I#{gmock_dir} -I#{gtest_dir}/include -pthread -c #{gmock_dir}/src/gmock-all.cc -o #{$host_output_folder}/gmock-all.o" unless File.exists?("#{$host_output_folder}/gmock-all.o")
    sh "ar -rv #{$host_output_folder}/libgmock.a #{$host_output_folder}/gmock-all.o" unless File.exists?("#{$host_output_folder}/libgmock.a")
  end

  desc "compile all host tests"
  task :compile => [:settings, :compile_gtest] do
    libraries_with_tests = Rake::FileList["Firmware/libraries/*/test"]
    libraries_with_tests.collect{|path| path.gsub('Firmware/libraries','Firmware/sketchbook/libraries').pathmap('%d')}.each do |library|
      compile_host_library(library)
      compile_host_test(library)
    end
  end

  desc "run all host tests"
  task :test => :compile do
    libraries_with_tests = Rake::FileList["Firmware/libraries/*/test"]
    puts libraries_with_tests
    failed_tests = []
    libraries_with_tests.collect{|path| path.pathmap('%d')}.each do |library|
        name = library.pathmap('%n')
        sh "#{$host_output_folder}/#{name}" do |ok, status|
          failed_tests << name if status.exitstatus != 0
        end
    end
    if !failed_tests.empty?
      puts "!!! tests failed !!!"
      puts failed_tests.join(' ')
      fail "tests failed: #{failed_tests.join(' ')}"
    end
  end

  task :coverage => :test do
    coverage_info = "#{$output_folder}/coverage.info"
    sh "lcov --directory #{$output_folder} --capture --output-file #{coverage_info}"
    sh "lcov --remove #{coverage_info} '*/Xcode.app/*' '/usr/include/*' '*/googletest/*' '*/googlemock/*' '*/test/*' '*/mock/*'  --output-file #{coverage_info}"
    puts "have COVERALLS_TOKEN => upload"
    text = File.read(coverage_info)
    text = text.gsub(/Firmware\/sketchbook\/libraries/, "Firmware/libraries")
    File.open(coverage_info, "w") {|file| file.puts text}
    sh "lcov --list #{coverage_info}"
    if(ENV['COVERALLS_TOKEN'])
      verbose(false) do
        sh "coveralls-lcov --repo-token #{ENV['COVERALLS_TOKEN']} #{coverage_info}"
      end
    else
      puts "have no COVERALLS_TOKEN => skip upload"
    end
  end
end

def compile_host_library(library_path)
  name = library_path.pathmap('%n')
  target = $host_output_folder
  sources = Rake::FileList[library_path + "/src/**/*.cpp",library_path + "/src/**/*.c"]
  includes = [library_path + "/src"]
  defines = []

  json_info_file = "#{library_path}/.library.json"

  Dir.glob('Firmware/sketchbook/libraries/*').select{|f| File.directory? f}.each do |other|
    includes << "#{other}"
    includes << "#{other}/src"
  end

  Dir.glob('Firmware/sketchbook/libraries/*').select{|f| File.directory? f}.each do |other|
    includes << "#{other}"
    includes << "#{other}/src"
  end

  #if(File.exists?(json_info_file))
  #  info = JSON.parse(IO.read(json_info_file))
  #  info["dependencies"].each do |dependency|
  #    includes << "Firmware/sketchbook/libraries/#{dependency}/src"
  #    includes << "Firmware/sketchbook/libraries/#{dependency}"
  #  end
  #end

  ninja_path = library_path.pathmap("%{^Firmware,#{target}}X/src")
  ninja_file = "#{ninja_path}/build.ninja"
  FileUtils.mkdir_p(ninja_path) unless Dir.exists?(ninja_path)
  File.open(ninja_file, "w+") do |file|
    file.write(ERB.new(IO.read("Build/host_rules.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_compile.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_library.erb")).result(binding))
  end

  sh("ninja -v -j8 -f #{ninja_file}")
  #sh("env && ninja -v -j8 -f #{ninja_file}")
end

def compile_host_test(library_path)
  name = library_path.pathmap('%n')
  target = $host_output_folder
  sources = Rake::FileList[library_path + "/test/**/*.cpp",library_path + "/test/**/*.c"]
  includes = ["Firmware/sketchbook/libraries/googletest/include", "Firmware/sketchbook/libraries/googlemock/include"]
  includes << "#{library_path}/src"
  includes << "#{library_path}/test"
  includes << "#{library_path}/mock"
  defines = []
  libraries = ["#{$host_output_folder}/libgtest.a" , "#{$host_output_folder}/libgmock.a"]
  external_libraries = []

  json_info_file = "#{library_path}/.library.json"
  if(File.exists?(json_info_file))
    info = JSON.parse(IO.read(json_info_file))
    info["dependencies"].each do |dependency|
      includes << "Firmware/sketchbook/libraries/#{dependency}/src"
      includes << "Firmware/sketchbook/libraries/#{dependency}"
      includes << "Firmware/sketchbook/libraries/#{dependency}/mock"
      libraries << "#{$host_output_folder}/lib#{dependency}.a"
    end
  end

  libraries << "#{$host_output_folder}/lib#{name}.a"



  ninja_path = library_path.pathmap("%{^Firmware,#{target}}X/test")
  ninja_file = "#{ninja_path}/build.ninja"
  FileUtils.mkdir_p(ninja_path) unless Dir.exists?(ninja_path)
  File.open(ninja_file, "w+") do |file|
    file.write(ERB.new(IO.read("Build/host_rules.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_compile.erb")).result(binding))
    file.write(ERB.new(IO.read("Build/host_link.erb")).result(binding))
  end
  sh "ninja -v -j8 -f #{ninja_file}"
end

task :all => ["host:test", :compile_arduino_all]
task :quick => ["host:test", :compile_arduino_SolarMonitor]
task :travis => "host:coverage"
