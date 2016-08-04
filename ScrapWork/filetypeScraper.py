import urllib2
from bs4 import BeautifulSoup

Url = "http://www.generic-url-with-file-extensions-here/"
extenstionsList = []

filename = "/path/to/file/extensions/location"
file = open(filename, 'w')

#Go through pages 1 - 3 of the url
for i in range(1, 4):
	dynamicUrl = Url + str(i)

	#Grab the get response from the url
	response = urllib2.urlopen(dynamicUrl)

	#Read the data from the response
	data = response.read()

	#Easily parse it using BeautifulSoup module
	parsed_html = BeautifulSoup(data, 'html.parser')

	#Go through and save the file extension lines
	for strong_tag in parsed_html.find_all('strong'):
		if (strong_tag.get('class')):
			extenstionsList.append(strong_tag)

#Need to go through file extention html lines and just grab the file extension type
for j in extenstionsList:

	#Format of current string: <strong class="color3"> (filetype) <strong>
	#Need to go through and get rid of the sides so we are just left with the type
	
	#Removes the left half
	split1 = str(j).split('>', 1)
	temp_string = split1[1]

	#Removes the right half
	split2 = str(temp_string).split('<', 1)
	final_string = split2[0]

	final_string += ","

	#Write to folder to save
	file.write(final_string)

file.close