require 'uri'

arduino_folder = "/Applications/Arduino.app/Contents/Java"
builder = "#{arduino_folder}/arduino-builder"

options = [
    "-hardware #{arduino_folder}/hardware", 
    "-hardware #{ENV['HOME']}/Library/Arduino15/packages",
    
    "-tools #{arduino_folder}/tools-builder", 
    "-tools #{arduino_folder}/hardware/tools/avr",
    "-tools #{ENV['HOME']}/Library/Arduino15/packages",
    
    "-libraries Thing/sketchbook/library",
       
    "-fqbn=adafruit:samd:adafruit_feather_m0" 
].join(" ")

libraries = [
    ["zip", "http://ftp-stud.fht-esslingen.de/pub/Mirrors/eclipse/paho/arduino_1.0.0.zip"],
    ["git", "https://github.com/bittailor/Adafruit_FONA_Library.git","bt-gprs-improvements"]   
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
    library_path = "Thing/sketchbook/library"
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
        ln_s "../../libraries/#{name}", "Thing/sketchbook/library/"
    end   
end

task :compile do
    sketches = Dir.glob("Thing/sketchbook/*/*.ino")
    sketches.each do |sketch|
        puts "compile #{sketch} ..."
        sh "#{builder} #{options}  #{sketch}"
        puts "...  #{sketch} done"
    end
    puts "************"
    puts "#{sketches.count} compiled"
end


task :default => :compile

