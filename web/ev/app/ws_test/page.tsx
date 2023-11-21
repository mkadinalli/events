"use client"

export default function Ws(){
    const createConnection = () =>{
    const conn = new WebSocket("wss://localhost:2000/chat",[
        "protocolOne"
    ]);
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
                <button>Send</button>
            </body>
        </html>
    )
}