"use client"

import styles from "./page.module.css"
import MainLayout from "../layouts/mainLayout"
import { useRouter } from "next/navigation"
import { useEffect, useState } from "react"
import Global from "../models/global"
import Post from "../components/post/page"

export default function Home(){
    const [data,setData] = useState([]);

    useEffect(()=>{
        fetch("http://localhost:2000/api/gevents",{
            method : "POST",
            body :JSON.stringify({
                user_id: Global.user_id,
                last_time: "2010-5-5 00:00:00",
                last_query_time: "2010-4-4 00:00:00"
            })
        }).then((d)=>{
            return d.json()
        }).then((f)=>{
            setData(f.results);
            console.log(f.results);
        })
    },[])

    const router = useRouter();

    const goToProfile = ()=>{
        router.push("/profile");
    }

    const node =  (

        <div className={styles.container}>
        <div className={styles.header}>

            <div><h3>EV</h3></div>

            <div className={styles.buttonContainer}>
                <div><span className={styles.label}>Home</span></div>
                <button className={styles.btn} onClick={(e) => {
                    goToProfile();
                }}>Profile</button>
            </div>
        </div>

        <div className={styles.body}>
            <div className={styles.column}></div>

            <div className={styles.column}>
                {
                    data.map((item)=>{
                        console.log(item);
                        return <Post props={item}/>
                    })
                }
            </div>

            <div className={styles.column}></div>
        </div>
    </div>
    )

    return < MainLayout children={node} title={"Home"} />

}