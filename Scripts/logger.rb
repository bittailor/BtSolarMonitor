require "serialport"
 
port_str = "/dev/cu.usbmodem1411" 
baud_rate = 115200
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

counter = 0;

SerialPort.open(port_str, baud_rate, data_bits, stop_bits, parity) do |port|
    while(true)
        started = false;
        while (!started) do
            line = port.gets.chomp
            started = true if line == "<begin>"
        end
        counter = counter + 1
        ended = false
        filename = "log_#{counter}.csv"
        puts "start log #{filename}"
        File.open(filename, 'w') do |file| 
            while (!ended) do
                line = port.gets.chomp
                if line == "<end>" then
                    puts "end log #{filename}"
                    ended = true     
                else
                    file.puts line 
                end
            end 
        end
    end 
end
