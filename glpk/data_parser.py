from random import choices
from string import ascii_lowercase
import math
import re



class IOer: 
    instance_list = list()
    
    def __init__(self, filename):
        self.filename = filename
        
    def translate_file(self):
        file = open(self.filename, 'r')
        data = open("airland2.dat", "a")
        
        file_content = file.read()
        file_numbers = re.findall(r"[-+]?\d*\.\d+|\d+", file_content)
        
        planes_amount = int(file_numbers[0])
        
        line = "data;\nset PLANES :="
        line3 = "\nparam: E T L g h :=\n"
        
        
        labels = ""
        for i in range(0, planes_amount):
            new_word = "".join(choices(ascii_lowercase, k=2))
            if (labels.find(new_word) != -1):
                while labels.find(new_word) != -1:
                    new_word = "".join(choices(ascii_lowercase, k=2))
                
            labels = labels + " " + new_word 
        
        labels_list = labels.split()
        
        data.write(line)
        data.write(labels)
        data.write(";\n")
        data.write(line3)
        
        parameters_line = ""
        time_list = ""
        big_time_str = ""
        iterator = 3;
        
        for i in range(0, planes_amount):
            
            parameters_line = file_numbers[iterator:iterator+5]
            parameters_str = " ".join(str(x) for x in parameters_line)
                       
            
            data.write(labels_list[i])
            data.write(" ")
            data.write(parameters_str + "\n")
            
            iterator = iterator + 5
            
            time_list = file_numbers[iterator:(iterator+planes_amount)]
            time_str = " ".join(str(x) for x in time_list)
            
            big_time_str = big_time_str + labels_list[i] + " " + time_str + "\n"            
            
            iterator = iterator + planes_amount + 1
            
        param_S = "\nparam S: " + labels + " :=\n"
        
        data.write(param_S)    
        data.write(big_time_str)    
        
        
        file.close()
        data.write("\n\nend;")
        data.close()
        
        
        
def main():
    filename = "airland2.txt"
    ioer = IOer(filename)
    ioer.translate_file()
    
    
if __name__ == "__main__":
    main()