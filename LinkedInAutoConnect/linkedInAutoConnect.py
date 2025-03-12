#lets me control and automate web browsers
from selenium import webdriver

#By class gives locator strategies such as By.ID, By.NAME, By.XPATH
from selenium.webdriver.common.by import By

#Does key actions enter, tab, etc
from selenium.webdriver.common.keys import Keys

#Manages browser driver lifecycle
from selenium.webdriver.chrome.service import Service

#Automatically handles setup of ChromeDriver binary
from webdriver_manager.chrome import ChromeDriverManager
import time


#Init the driver
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get("https://www.linkedin.com/login")

#Use me credentials
username = driver.find_element(By.ID, "username")
passwrd = driver.find_element(By.ID, "password")
#add my credentials here (secret stuff hush hush)
username.send_keys("ismailsyed2005@gmail.com")
passwrd.send_keys("Ismailtheboss")

# //->allows search to start from anywhere in HTML
# button -> specifies tag name of elems we are looking for
# type -> tells XPATH engine that we are only looking for 'submit' buttons
login_button = driver.find_element(By.XPATH, "//button[@type='submit']")
login_button.click()

#This program must not act like a bot
time.sleep(3.14158)
time.sleep(50)


#Goto linkedIn search bar

#Find elems that are both (search-global-typeahead__input) and (search-global-typeahead__input--ellipsis)
search_box = driver.find_element(By.CSS_SELECTOR, ".search-global-typeahead__input.search-global-typeahead__input--ellipsis")
search_box.send_keys("Software Engineer")  # search Query we are using
search_box.send_keys(Keys.RETURN)
time.sleep(5.6789)   #No bots wanted


#Click the people button
people_button = driver.find_element(By.XPATH,"//button[normalize-space(text())='People']")
people_button.click()

print("Worked!")
