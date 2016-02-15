require 'uri'

arduino = "/Applications/Arduino.app/Contents/MacOS/Arduino"
arduino_folder = "/Applications/Arduino.app/Contents/Java"
builder = "#{arduino_folder}/arduino-builder"


options = [
    "-hardware #{arduino_folder}/hardware", 
    "-hardware #{ENV['HOME']}/Library/Arduino15/packages",
    #"-hardware Thing/sketchbook/hardware",
    
    "-tools #{arduino_folder}/tools-builder", 
    "-tools #{arduino_folder}/hardware/tools/avr",
    "-tools #{ENV['HOME']}/Library/Arduino15/packages",
    
    "-libraries Thing/sketchbook/libraries",
    
    #"-warnings default", 
].join(" ")

boards = [
    "-fqbn=adafruit:samd:adafruit_feather_m0",
    # "-fqbn=breadboard:avr:atmega328bb",    
]

libraries = [
    ["zip", "http://ftp-stud.fht-esslingen.de/pub/Mirrors/eclipse/paho/arduino_1.0.0.zip"],
    ["git", "https://github.com/adafruit/Adafruit_MQTT_Library.git"], 
    ["git", "https://github.com/bblanchon/ArduinoJson.git","v5.0.7"], 
    ["git", "https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library.git","1.0.0"], 
    ["git", "https://github.com/adafruit/Adafruit-GFX-Library.git","v1.1.4"], 
    ["git", "https://github.com/adafruit/Adafruit_INA219.git"], 
    #["git", "https://github.com/bittailor/Adafruit_FONA_Library.git","bt-gprs-improvements"]   
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
    library_path = "Thing/sketchbook/libraries"
    rm_r library_path , :force => true
    mkdir library_path
    Dir.chdir(library_path) do
        libraries.each do |library|
            installLibrary(library)    
        end              
    end
    Dir.entries("Thing/libraries").each do |library| 
        next if (library =='.' || library == '..' || library.start_with?("."))
        puts "** link #{library} **" 
        name = File.basename(library)
        ln_s "../../libraries/#{name}", "#{library_path}/"
    end   
end

task :compile do
    sketches = Dir.glob("Thing/sketchbook/*/*.ino")
    sketches.each do |sketch|
        boards.each do |board|
            puts "compile #{sketch} ..."
            sh "#{builder} #{options} #{board} #{sketch}"
            puts "...  #{sketch} done"
        end
    end
    puts "************"
    puts "compiled #{sketches.count} sketches for #{boards.count} boards"
end

task :upload do
    sh "#{arduino} --board adafruit:samd:adafruit_feather_m0 --pref sketchbook.path=#{Dir.pwd}/Thing/sketchbook --upload --port /dev/cu.usbmodem1411 -v Thing/sketchbook/ResourceCheck/ResourceCheck.ino" 
end


task :default => :compile

