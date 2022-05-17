import requests
import hashlib
import time
import random

inputfile = open('handle.txt', 'r')
cnt = (int)(0)
for line in inputfile:
    if cnt == 0:
       handle = line
    else:
        output_name = line
    cnt += 1
inputfile.close()
handle = handle[:-1]
request_header = "https://codeforces.com/api/"
request_type = "user.status?"
name = "handle=" + handle
fr = "from=1"
count = "count=2"
apikey = "apiKey=3903987c32a6af46fed7cfee7d37489cd9f069d8"
unixtime = "time=" + str(int(time.time()))
rand = str(random.randint(100000, 999999))
tohash = rand + "/" + request_type + apikey + "&" + count + "&" + fr + "&" + name + "&" + unixtime + "#" + "8ed6e456285870873788e878a9f1001b478ebc1a"
apisecret = "apiSig=" + rand + str(hashlib.sha512(tohash.encode()).hexdigest())

final = request_header + request_type + name + "&" + fr + "&" + count + "&" + apikey + "&" + unixtime + "&" + apisecret

response = requests.get(final)

answer = response.text
output = open(output_name, 'w');
output.write(answer);
output.close()
