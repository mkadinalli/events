"use client"

import styles from "./page.module.css"
import MainLayout from "../layouts/mainLayout"
import { useRouter } from "next/navigation"

export default function Home(){

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
    </div>
    )

    return < MainLayout children={node} title={"Home"} />

}