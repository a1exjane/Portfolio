import requests
from bs4 import BeautifulSoup
import http.client
http.client._MAXHEADERS = 1000
import time
from gtts import gTTS
import os
import shutil
from deep_translator import GoogleTranslator
import cairosvg

img = 0

audio = 0

my_headers = {"User-Agent":"Mozilla/5.0 (Macintosh; Intel Mac OSX 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36",
              "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8"}

verb_list = [
             "be", "have", "call", "go", "come", "speak", "understand", "say", "think", "explain",
             "know", "remember", "forget", "ask", "answer", "listen", "see", "look", "show", "eat",
             "drink", "reserve", "cook", "taste", "serve", "recommend", "give", "take","get", "cost",
             "pay",  "count", "buy", "sell", "open", "close", "begin", "finish", "born", "die",
             "live", "live", "visit", "travel", "fly", "work", "earn", "rest", "sleep", "swim",
             "read", "teach", "learn","study", "repeat", "practice", "try", "help", "seek", "find",
             "follow", "lose",  "move",  "smile", "cry", "surprise", "feel", "translate", "dress", "change",
             "sit", "stand", "sleep", "connect", "disconnect", "write", "send", "receive", "draw", "play",
             "dance", "sing", "drive",  "walk", "run", "ride", "wash", "clean", "use", "cut",
             "create", "destroy", "make", "break", "fix", "want", "prefer", "like", "love", "hate"
            ]

ttslang = "id"


os.mkdir("image", mode=0o777, dir_fd=None)
os.mkdir("audio", mode=0o777, dir_fd=None)

for x in range(1, 50):
    fullink = "https://conjugator.reverso.net/conjugation-english-verb-" + verb_list[x] + ".html"
    r2 = requests.get(fullink, headers=my_headers)
    soup = BeautifulSoup(r2.text, "lxml")
    main_verb = verb_list[x]
    to_translate = main_verb
    print(x)
    print(main_verb)
    translated = GoogleTranslator(source='en', target=ttslang).translate(to_translate)
    translated = translated.replace("to ", "")
    translated = translated.replace("To ", "")
    with open('cover.svg', 'r') as file :
        filedatacover = file.read()
    shutil.copy2('cover.svg', "./" + "image" + "/cover.svg")
    shutil.copy2('main.template.drawio.svg', "./" + "image" + "/main.template.drawio.svg")
    shutil.copy2('pause.mp3', "./" + "audio" + "/pause.mp3")
    os.chdir("image")
    filedatacover = filedatacover.replace('Base', main_verb.capitalize())
    filedatacover = filedatacover.replace('Translate', translated.capitalize())
    with open(str(img) + ".svg", 'w') as file:
        file.write(filedatacover)
    cairosvg.svg2png(url=str(img) + ".svg", write_to=str(img) + ".png")
    os.remove(str(img) + ".svg") 
    img += 1
    with open('main.template.drawio.svg', 'r') as file :
        filedata = file.read()
    os.chdir("../")
    os.chdir("audio")
    shutil.copy2('pause.mp3',str(audio) + ".mp3")
    audio += 1
    tts = gTTS(main_verb, lang="en")
    tts.save(str(audio) + main_verb  + '.mp3')
    audio += 1
    shutil.copy2('pause.mp3',str(audio) + ".mp3")
    audio += 1
    tts = gTTS(translated, lang=ttslang)
    tts.save(str(audio) + translated  + '.mp3')
    audio+= 1
    shutil.copy2('pause.mp3',str(audio) + ".mp3")
    audio+= 1
    shutil.copy2('pause.mp3',str(audio) + ".mp3")
    for i in range (0, 6):
        u = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="graytxt")[i].text
        p = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="verbtxt")[i].text
        present_verb = u + " " + p
        present_verb = GoogleTranslator(source="en", target=ttslang).translate(present_verb)
        filedata = filedata.replace('Present' +str(i) , present_verb.capitalize())
        present_verb = present_verb.replace("/", " ")
        tts = gTTS(present_verb, lang=ttslang)
        tts.save(str(audio) + present_verb  + '.mp3')
        audio+= 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio+= 1
    time.sleep(2)
    for i in range (0, 6):
        a = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="graytxt")[i].text 
        b = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="verbtxt")[i].text
        if "she" in a:
            fragen = "Does " + a + " " + b + "?"
        else:
            fragen = "Do " + a + " " + b + "?"
        fragen = GoogleTranslator(source="en", target=ttslang).translate(fragen)
        filedata = filedata.replace('PresentF' +str(i) , fragen.capitalize())
        fragen = fragen.replace("/", " ")
        tts = gTTS(fragen, lang=ttslang)
        tts.save(str(audio) + fragen  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range(0,6):
        k = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="graytxt")[i].text
        l = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="verbtxt")[i].text
        if "she" in k:
            negativo = k + " doesn't " + l
        else:
            negativo = k + " don't " + l
        negativo = GoogleTranslator(source="en", target=ttslang).translate(negativo)
        filedata = filedata.replace('PresentN' +str(i) , negativo.capitalize())
        negativo = negativo.replace("/", " ")
        tts = gTTS(negativo, lang=ttslang)
        tts.save(str(audio) + negativo  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range (0, 6):
        ss = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[1].find_all('i', class_="graytxt")[i].text
        tt = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[1].find_all('i', class_="verbtxt")[i].text
        past = ss + " " + tt
        past = GoogleTranslator(source="en", target=ttslang).translate(past)
        filedata = filedata.replace('Past' +str(i) , past.capitalize())
        past = past.replace("/", " ")
        tts = gTTS(past, lang=ttslang)
        tts.save(str(audio) + past  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range (0, 6):
        e = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="graytxt")[i].text
        f = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="verbtxt")[i].text
        pastquestion = "Did " + e + " " + f + "?"
        pastquestion = GoogleTranslator(source="en", target=ttslang).translate(pastquestion)
        filedata = filedata.replace('PastF' +str(i) , pastquestion.capitalize())
        pastquestion = pastquestion.replace("/", " ")
        tts = gTTS(pastquestion, lang=ttslang)
        tts.save(str(audio) + pastquestion  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range(0,6):
        s = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="graytxt")[i].text
        t = soup.find('div', class_='result-block-api').find('div', class_="word-wrap-row").find("div", class_="wrap-three-col").find_all('i', class_="verbtxt")[i].text
        pastnegativo = s + " didn't " + t
        pastnegativo = GoogleTranslator(source="en", target=ttslang).translate(pastnegativo)
        filedata = filedata.replace('PastN' +str(i) , pastnegativo.capitalize())
        pastnegativo = pastnegativo.replace("/", " ")
        tts = gTTS(pastnegativo, lang=ttslang)
        tts.save(str(audio) + pastnegativo  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range (0, 6):
        futuro1 = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="graytxt")[i].text
        futuro2 = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="particletxt")[i].text
        futuro3 = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="verbtxt")[i].text
        futuro = futuro1 + " " + futuro2 + " " + futuro3
        futuro = GoogleTranslator(source="en", target=ttslang).translate(futuro)
        filedata = filedata.replace('Future' +str(i) , futuro.capitalize())
        futuro = futuro.replace("/", " ")
        tts = gTTS(futuro, lang=ttslang)
        tts.save(str(audio) + futuro  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range (0, 6):
        x = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="graytxt")[i].text
        x1 = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="particletxt")[i].text
        y = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="verbtxt")[i].text
        futurequestion = x1 + " " + x + " " + y + "?"
        futurequestion = GoogleTranslator(source="en", target=ttslang).translate(futurequestion)
        filedata = filedata.replace('FutureF' +str(i) , futurequestion.capitalize())
        futurequestion = futurequestion.replace("/", " ")
        tts = gTTS(futurequestion, lang=ttslang)
        tts.save(str(audio) + futurequestion  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    time.sleep(2)
    for i in range(0,6):
        u = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="graytxt")[i].text
        u1 = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="particletxt")[i].text
        j = soup.find('div', class_='result-block-api').find_all('ul', class_="wrap-verbs-listing")[4].find_all('i', class_="verbtxt")[i].text
        futuronegativo = u + " " + u1 + " not " + j
        futuronegativo = GoogleTranslator(source="en", target=ttslang).translate(futuronegativo)
        filedata = filedata.replace('FutureN' +str(i) , futuronegativo.capitalize())
        futuronegativo = futuronegativo.replace("/", " ")
        tts = gTTS(futuronegativo, lang=ttslang)
        tts.save(str(audio) + futuronegativo  + '.mp3')
        audio += 1
        shutil.copy2('pause.mp3',str(audio) + ".mp3")
        audio += 1
    os.chdir("../")
    os.chdir("image")
    with open(str(img) + ".svg", 'w') as file:
        file.write(filedata)
    cairosvg.svg2png(url=str(img) + ".svg", write_to=str(img) + ".png")
    os.remove(str(img) + ".svg")
    img += 1
    time.sleep(2)
    os.chdir("../")







