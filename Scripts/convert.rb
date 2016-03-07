name = "log_2"

counter = 1
File.open("#{name}.dat", 'w') do |file| 
    IO.foreach("#{name}.csv") do |line|       
        file.puts line.gsub(";","\t")
        counter += 1
    end
end