"use client"

export default function Ws(){

  var conn :WebSocket;
    const createConnection = () =>{
      conn = new WebSocket("ws://localhost:2000/chat");

    //conn.send("hello world");

    conn.addEventListener("open",(e)=>{
        alert("connected");
    });

    conn.addEventListener("err",(e)=>{
        alert("error");
    });

    conn.addEventListener('close', function (event) {
        if (event.wasClean) {
          alert('WebSocket connection closed cleanly.');
        } else {
          alert('WebSocket connection closed unexpectedly:');
          console.log(event);
        }
      });

      conn.addEventListener("message", (e)=>{
        console.log(e.data);
      })

      if (conn.readyState === WebSocket.CONNECTING) {
        console.log('WebSocket is connecting.');
      } else if (conn.readyState === WebSocket.OPEN) {
        console.log('WebSocket is open and ready to send/receive data.');
      } else if (conn.readyState === WebSocket.CLOSING) {
        console.log('WebSocket is closing.');
      } else if (conn.readyState === WebSocket.CLOSED) {
        console.log('WebSocket connection is closed.');
      }
    }

    const sendData = ()=>{
      if(conn){
        conn.send("Hello world");
        console.log("sent a message");
        return;
      }

      console.log("message sending failed");
    }

    
    return (
        <html>
            <body>
                <h3>Chat</h3>
                <button onClick={(e)=>{
                    createConnection();
                }}>Create Connection</button>
                <button>Connect</button>
                <input type="text"></input>
                <button onClick={(e)=>{sendData()}}>Send</button>
            </body>
        </html>
    )
}