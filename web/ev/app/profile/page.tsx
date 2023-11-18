"use client"

import styles from "./page.module.css"
import MainLayout from "../layouts/mainLayout"
import Global from "../models/global"
import { useEffect, useState } from "react"

export default function Profile(){

    const [data,setData] = useState({});

    useEffect(()=>{
        fetch(`https://localhost:2000/api/user/?id=${Global.user_id}`,{
            method: "GET"
        }).then((d)=> d.json()).then((d)=>{
            if(d.success){
                setData(d.results[0]);
            }else{
            setData({});
            }
        }).catch((d)=>{
            console.error(d);
        })
    },[])

    const node = (
        <div className={styles.container}>
            <div className={styles.header}>

                <div><h3>EV</h3></div>

                <div className={styles.buttonContainer}>
                    <div><span className={styles.label}>Profile</span></div>
                </div>
                </div>

                <div className={styles.tab}>
                    <div className={styles.tabItem}><button className={styles.tabBtn}>Button</button></div>
                    <div className={styles.tabItem}><button className={styles.tabBtn}>Button</button></div>
                    <div className={styles.tabItem}><button className={styles.tabBtn}>Button</button></div>
                </div>

                <div className={styles.pContainer}>
                    <div className={styles.pGrid1}>
                        {/*photo*/}
                        <div className={styles.dpHolder}>
                            <img src={data.avater} className={styles.dphoto}/>
                        </div>


                        <div className={styles.aboutHolder}>
                            <h3>{data.name}</h3>
                            <h4 className={styles.username}><b>{data.username}</b></h4>

                            <h5 className={styles.about}>{data.about}</h5>

                            <p>{data.bio}</p>

                            <button className={styles.editBtn}>Edit profile</button>

                            <div className={styles.follows}>
                                <div className={styles.followers}><p><b>{data.followers}</b></p><a>followers</a></div>
                                <div className={styles.following}><p><b>{data.followed}</b></p><a>following</a></div>
                            </div>

                            <div>
                                <div><p>app1</p><a>go</a></div>
                                <div><p>app2</p><a>go</a></div>
                            </div>
                        </div>



                        
                    </div>



                    <div className={styles.pGrid2}>Grid two</div>
                </div>
            </div>

    )

    return (
        <MainLayout children={node} title={"Profile"} />
    )
}