static const char page[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
 
 <head>
   <meta charset="UTF-8">
   <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <title>Nagamasu Vision</title>
   <style>

   body {
    font-family: "PT Sans", sans-serif;
    background-color: #cde2c5; /* Light green background */
    margin: 0;
    color: #333;
    line-height: 1.6;
  }
  a {
    text-decoration: none;
    color: #4c8c4a; /* Dark green link color */
  }
  h2 {
    display: block;
    font-size: 1.17em;
    margin-block-start: 1em;
    margin-block-end: 1em;
    margin-inline-start: 0px;
    margin-inline-end: 0px;
    font-weight: bold;
    color: #4c8c4a; /* Dark green heading color */
  }
  .container {
    max-width: 1180px;
    text-align: center;
    margin: 0 auto;
    padding: 0 3rem;
  }
  .btn {
    padding: 1rem;
    color: #fff;
    display: inline-block;
    background: #378037; /* Dark green button background */
    margin-bottom: 1rem;
    border: none;
    cursor: pointer;
  }
  #results {
    color: #4c8c4a; /* Dark green result text color */
  }
  #img1 {
    border: 1px solid #4c8c4a; /* Dark green border for the image */
  }
 
   </style>
   <script src="https://cdn.jsdelivr.net/npm/@tensorflow/tfjs"> </script>
    <script src="https://cdn.jsdelivr.net/npm/@tensorflow-models/mobilenet"> </script>
 
    <script language="javascript">
        function classifyImg() {
           const img = document.getElementById("img1");
           const r = document.getElementById("results");
           r.innerHTML = "";
 
           console.log("Classify...");
           img.crossorigin = " ";
           img.onload = function() {
                console.log("Wait to load..");
                mobilenet.load().then(model => {
                // Classify the image.
                model.classify(img, topk = 3).then(predictions => {
                  for (i in predictions) {
                    const className = predictions[i].className;
                    const probability = predictions[i].probability;
                    r.innerHTML += "<b>" + className + "</b> - " + probability + "<br/>";
                    // Check if the predicted item is a food item
                    if (isFoodItem(className)) {
                      r.innerHTML += "<b> A Food item was seen </b>";
                      sendFoodDetectedMessage();
                    }
                  }
                });
                img.onload = null;
                img.src = "http://10.0.0.229:81";
               });
           }
           img.src = "http://10.0.0.229/capture?t=" + Math.random();
        }

    // Function to check if the predicted item is a food item
    function isFoodItem(className) {
        const foodItems = ["guacamole", "consomme", "hot pot, hotpot", "trifle","ice cream, icecream",
                          "ice lolly, lolly, lollipop, popsicle", "French loaf", "bagel, beigel", "pretzel",
                          "cheeseburger", "hotdog, hot dog, red hot", "mashed potato",  "head cabbage",  "broccoli",
                          "cauliflower", "zucchini, courgette",  "spaghetti squash",  "acorn squash",  "butternut squash",
                          "cucumber, cuke",  "artichoke, globe artichoke", "bell pepper",  "cardoon", "mushroom", "Granny Smith",
                          "strawberry", "orange", "lemon", "fig", "pineapple, ananas",  "banana",  "jackfruit, jak, jack",
                          "custard apple", "pomegranate", "carbonara",  "chocolate sauce, chocolate syrup", "dough", "meat loaf, meatloaf",
                          "pizza, pizza pie",  "potpie",  "burrito",  "red wine",  "espresso",  "cup",  "eggnog"
                        ];
        return foodItems.includes(className.toLowerCase());
    }

    // Function to send a message to ESP32 when a food item is detected
    function sendFoodDetectedMessage() {
      // Change the URL to match the IP and port of your ESP32
      const esp32Url = "http://10.0.0.229:80/food-detected";
      fetch(esp32Url, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ message: "Food item detected!" }),
      })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error("Error:", error));
    }
      
    </script>
   </head>
 
   <body>
     <div class="container">
     <h2>TensorflowJS with ESP32-CAM</h2>
     <section>
       <img id="img1" width="320" height="200" src="http://10.0.0.229:81" crossorigin style="border:1px solid red"/>
       <div id="results"/>
     </section>
     <section>
       <a href="#" class="btn" onclick="classifyImg()">Classify the image</a>
      </section>
      <section id="i" />
   </div>
   </body>
</html>
)rawliteral";
 
