"use client"

import Image from 'next/image'
import styles from './page.module.css'
import { useEffect, useState } from 'react'

export default function Home() {

  const y = {
    user_id : "bdc0b320-5a1c-11ee-b6f2-1f05bb9bd55d",
    last_time : "2022-09-01 00:00:00",
    last_query_time : "2023-09-01 00:00:00"
}

  const [data,setData] = useState(null);


  useEffect(()=>{
    const fechData = async ()=>{
      const res = await fetch("https://localhost:2000/api/gevents",{
      method: "POST",
      body : JSON.stringify(y),
      headers: {
        "content-type": "application/json"
      }
    })
    
    console.log(y);

    console.log("hello world");
    console.log(res);
    const data = await res.json();


    console.log(data);

  }

  fechData();
  },[])

  return (
      <html>
      
      </html>
  )
}
