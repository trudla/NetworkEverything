var socket = io();

obj = {donut: "Pictures/donut-600x600.png",
	bear: "Pictures/bear_brown-600x600.png",
	zebra: "Pictures/zebra-600x600.png",
	horse: "Pictures/horse-600x600.png",
	airplane: "Pictures/plane-600x600.png"
	};
obj_list = ["donut","bear","zebra","horse","airplane"];
points = 0;
reading = false;


function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
}

function GameStart() {
	console.log("gamestart");
	start_stage = 0;
	document.getElementById("start").style.display = 'none';
	Displaying(start_stage);
	
}

function wipeAll(whichParent) {
	// Get the <ul> element with id="myList"
	var parentToBeWiped = document.getElementById(whichParent);

	// As long as <ul> has a child node, remove it
	while (parentToBeWiped.hasChildNodes()) {  
	  parentToBeWiped.removeChild(parentToBeWiped.firstChild);
	} 
}

function wipeSpecific(whichNode){
	var item = document.getElementById(whichNode);
	item.parentNode.removeChild(item);
}

function addChild(parent, type, id){
	var img = document.createElement(type);
	img.id = id;
	var src = document.getElementById(parent);
	src.appendChild(img);
}

function Displaying(stageBefore){
	stage = stageBefore +1;
	console.log("stage: "+stage);

	document.body.style.setProperty("--main-bg", "#ee4266");
	document.getElementById("displayPict").style.display = 'block';

	chosenPictures = [];

	var i = 0;
	addChild("upperHalf", "img", "bigPicture");
	addChild("displayPict", "div", "smallPics");
	lastPict = -1;
	mess = false; //checking for the last picture from the previous series because I'm just too lazy at this point
	function looping() {
	    if( i < stage ){
	    	whichPicture = getRandomInt(0, obj_list.length);
	    	if (i==0 && obj_list.indexOf(lastGuess) == whichPicture){
	    		mess = true;
	    	}
	    	if ((whichPicture == lastPict || (mess == true && i == 0)) && whichPicture != 0){
	    		whichPicture -= 1;
	    	}
	    	else if (whichPicture == lastPict || (mess == true && i == 0)){
	    		whichPicture += 1;
	    	}	    	
	    	console.log("whichPicture after if/else : "+whichPicture)
	    	lastPict = whichPicture;
			chosenPictures.push(obj_list[whichPicture]);
			//displays big picture every 1500ms
			document.getElementById("bigPicture").src = obj[obj_list[whichPicture]];
	        setTimeout( looping, 1500 );
			//adds small picture of previously shown big picture
			addChild("smallPics", "img", '"'+i+'"');
			document.getElementById('"'+i+'"').src = obj[obj_list[whichPicture]];
			document.getElementById('"'+i+'"').style = "width:100px;height:100px;" ;
	    }
	    else if (i == stage){
	    	document.getElementById("displayPict").style.display = 'none';
	    	Guessing(chosenPictures, stage);
	    	wipeSpecific("bigPicture");
	    	wipeSpecific("smallPics");
	    }
	    i++;
	}
	looping()
}

var lastGuess; //lame solution but it works (sort of)
function Guessing(displayedPictures, currStage)
{
	document.body.style.setProperty("--main-bg", "#ffd23f");
	currentStage = currStage;
	guessThese = displayedPictures;
	//console.log("Displayed:");
	console.log(displayedPictures);
	document.getElementById("guessPict").style.display = 'block';
	var data = "none yet";
	reading = true;
	document.getElementById("howManyLeft").innerHTML = guessThese.length;
	socket.on('reading', function(values) {
		if (reading == true){
			data = values;
			//console.log("lastGuess: "+lastGuess+", coming data: "+data+", right choice: "+guessThese[0]);
			if (data == guessThese[0] && lastGuess != data){
				guessThese.shift();
				document.getElementById("howManyLeft").innerHTML = guessThese.length;
				console.log("guessThese: "+guessThese);
				if (guessThese.length == 0) {
					document.getElementById("guessPict").style.display = 'none';
					points += 1;
					lastGuess = data;
					reading = false;
					Displaying(currentStage);
				}
			}
			else if (lastGuess != data){
				document.getElementById("guessPict").style.display = 'none';
				reading = false;
				GameOver(guessThese[0], data);
			}
			lastGuess = data;

		}
		
	});
	//NextStage()
}

function GameOver(rightChoice, mistake){
	document.body.style.setProperty("--main-bg", "#3bceac");
	document.getElementById("gameOver").style.display = 'block';
	document.getElementById("points").innerHTML = points;
	document.getElementById("right").innerHTML = rightChoice;
	document.getElementById("wrong").innerHTML = mistake;
}

function GroundZero(){
	document.location.reload(false);
}