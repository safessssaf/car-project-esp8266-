var forward = false;
var backward = false;
var left = false;
var right = false; 

var actual_Speed = 0;
var speed_1 = 100;
var speed_2 = 150;
var speed_3 = 255;

let socket = new WebSocket("ws://192.168.1.63:81/ws");
let data;
let message;


document.addEventListener('keydown',(event)=>
{
    //detection of keys
    if(event.key == "ArrowUp")
    {
        forward = true;
        backward = false;
    }
    else if(event.key == "ArrowDown")
    {
        backward = true;
        forward = false;
    }

    if(event.key == "ArrowLeft")
    {
        left = true;
        right = false;
    }
    else if(event.key == "ArrowRight")
    {
        right = true;
        left = false;
    }
    //speed levels
    if(event.key == "a")
    {
        
        actual_Speed = speed_1;
    }
    else if(event.key == "s")
    {
            
        actual_Speed = speed_2;
    }else if(event.key == "d")
    {
        actual_Speed = speed_3;
    }
    else if(event.key == " ")
    {
        actual_Speed = 0;
    }
})
document.addEventListener('keyup',(event)=>
{
    //js suck and needs to detect the key up
        if(event.key == "ArrowUp")
        {
            forward = false;
        }
        else if(event.key == "ArrowDown")
        {
            backward = false;
        }
    
        if(event.key == "ArrowLeft")
        {
            left = false;
        }
        else if(event.key == "ArrowRight")
        {
            right = false;
        }
})

setInterval(() => {
document.getElementById("input").innerHTML = `
<p>forward: ${forward}</p>
<p>backward: ${backward}</p>
<p>left: ${left}</p>
<p>right: ${right}</p>
<p>actual_Speed: ${actual_Speed}</p>
`;
data = {"forward" : forward, "backward" : backward, "left" : left, "right" : right, "speed": actual_Speed};
message = JSON.stringify(data);

// Send data only if WebSocket is open
if (socket.readyState === 1) {
    socket.send(message);
}
}, 1);
