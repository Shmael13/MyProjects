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

from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


import time

#for testing
def infin_wait():
    while(True):
        time.sleep(10)
        print("next")


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

#define the wait
wait = WebDriverWait(driver, 10)

#This program must not act like a bot
time.sleep(3.14158)


#Goto linkedIn search bar to make elems visible
search_button = driver.find_element(By.CSS_SELECTOR, ".search-global-typeahead__collapsed-search-button")
search_button.click()

# Find the (newly visible) search input
search_input = driver.find_element(By.CSS_SELECTOR, "input.search-global-typeahead__input--nile-background")
search_input.send_keys("AI Engineer")  # search Query we are using
search_input.send_keys(Keys.RETURN)
time.sleep(5.6789)   #No bots wanted, and wait till page loads

#Click the people button
# infin_wait()

people_button = driver.find_element(By.XPATH, "//button[text()='People']")
people_button.click()
time.sleep(4)

print("Worked!")

#loops over and sends connections without notes to 10 people
i = 0
while i < 10:
    # Load profiles by scrolling
    driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
    time.sleep(3)  # Wait for new profiles to load

    # Find all the "Connect" buttons
    connect_buttons = driver.find_elements(By.XPATH, "//button[.//span[contains(@class, 'artdeco-button__text') and text()='Connect']]")
    print(connect_buttons)
    

    if not connect_buttons:
        print("No 'Connect' buttons on page.")
        break  # Exit the loop if no buttons

    for button in connect_buttons:
        try:
            # Scroll to the "Connect" button
            driver.execute_script("arguments[0].scrollIntoView(true);", button)
            time.sleep(1)  # Allow time for scrolling
            try:
                button.click()
            except:
                driver.execute_script("arguments[0].click();", button)

            time.sleep(2)  # Allow pop-up

            # "Send without a note"
            try:
                time.sleep(3)
                no_msg_button = driver.find_element(By.XPATH, "//button[@aria-label='Send without a note']")
                no_msg_button.click()
                print("Connection request sent successfully.")
                time.sleep(2)
            except Exception as send_error:
                print(f"Error finding 'Send without a note' button: {send_error}")

        except Exception as button_error:
            print(f"Error clicking 'Connect' button: {button_error}")
            break
    i+=1

print(f"Made {i} connections")
