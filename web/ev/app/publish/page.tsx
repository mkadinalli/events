"use client"

import styles from "./page.module.css"
import MainLayout from "../layouts/mainLayout"
import { useState } from "react"
import Global from "../models/global"
import Publish from "../models/publish"

export default function Pub(){

    const [details,setDetails] = useState({
        title: "",
        price: 0,
        venue: "",
        e_date: "",
        d_date: "",
        description: ""
    })


    const  subPub = async ()=>{
        const pub = new Publish(
            details.title,details.price,details.description, details.venue,details.e_date,details.d_date,Global.user_id
        );

        await pub.send();
    } 


    const node = (
        <div className={styles.container}>
            <div className={styles.header}>

                <div><h3>EV</h3></div>

                <div className={styles.buttonContainer}>
                    <div><span className={styles.label}>Publish new</span></div>
                </div>
            </div>

            <div className={styles.formContainer}>
                <div className={styles.form}>
                    <label>Title</label>
                    <input type="text" className={styles.inputField} 
                    value={details.title} onChange={(e)=>{
                        setDetails((p)=>{
                            return { ...p, title: e.target.value};
                        })
                    }}/>
                    <label>Price</label>
                    <input type="number" className={styles.inputField} value={ details.price} onChange={(e) =>{ 
                        setDetails(p =>{
                            return {...p, price: parseInt(e.target.value) }
                        } )
                    }
                    }/>
                    <label>Venue</label>
                    <input type="text" className={styles.inputField} value={ details.venue } onChange={(e)=>{
                        setDetails(p=>{
                            return {...p, venue: e.target.value }
                        })
                    }}/>

                    <label>Event date</label>
                    <input type="date" className={styles.inputField} value={details.e_date} onChange={(e)=>{
                        setDetails(p=>{
                            return {...p, e_date: e.target.value }
                        })
                    }}/>
                    <label>Deadline Date</label>
                    <input type="date" className={styles.inputField} value={details.d_date} onChange={(e)=>{
                        setDetails(p=>{
                            return {...p, d_date: e.target.value }
                        })
                    }}/>
                    <label>Description</label>
                    <textarea className={styles.inputField} value={details.description} onChange={(e)=>{
                        setDetails(p=>{
                            return {...p, description: e.target.value }
                        })
                    }}/>
                    <button
                    className={styles.subBtn} onClick={(e) => {
                        subPub();
                    }}>Do</button>
                </div>
            </div>
        </div>
    )

    return <MainLayout children={node} title={"Publish"}/>;
}