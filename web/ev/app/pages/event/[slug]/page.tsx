"use client"

import { useEffect, useState } from "react";
import Global from "@/app/models/global";

export default function Event({params} : any){
    
    const [postData,setPostData] = useState<any>({});

    useEffect(()=>{
        fetch("http://localhost:2000/api/one-event",{
        method: "POST",
        body: JSON.stringify({
            id : params.slug
        })
    }).then((p)=>p.json()).then((l)=>{
        setPostData(l.results[0]);
    })
    },[])

    //const data = await res.json();

    const star = async ()=>{
        const res = await fetch("http://localhost:2000/api/star",{
            method: "POST",
            body: JSON.stringify({
                user_id: Global.user_id,
                publish_id: postData.id
            })
        });

        const data = await res.json();

        console.log(data);
    }

    const sub = async ()=>{
        const res = await fetch("http://localhost:2000/api/subscribe",{
            method: "POST",
            body: JSON.stringify({
                user_id: Global.user_id,
                publish_id: postData.id
            })
        });

        const data = await res.json();

        console.log(data);
    }


    const unsub = async ()=>{
        const res = await fetch("http://localhost:2000/api/subscribe?",{
            method: "DELETE"
        });

        const data = await res.json();

        console.log(data);
    }

    

    //console.log(data);
    return (
        <div>
            <p>{postData.title}</p>
            <p>{postData.venue}</p>
            <p>{postData.avater}</p>
            <p>{postData.description}</p>
            <p>{postData.date_created}</p>
            <p>Stars: {postData.stars}</p>
            <p>Subs: {postData.subscriptions}</p>
            <button onClick={(e)=>{sub()}}>Subscribe</button>
            <button onClick={(e)=>{star()}}>Star</button>
        </div>
    )
}