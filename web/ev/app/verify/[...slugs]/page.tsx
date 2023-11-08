"use client"

import { useEffect, useRef, useState } from "react";
import styles from "./page.module.css"
import MainLayout from "../../layouts/mainLayout";
import Password from "@/app/models/password"
import { useRouter } from "next/navigation";

export default function Verify(par : {params: {slugs : string[]}}){

    console.log(par);
    
    const [password, setPassword] = useState("");
    const [confPassword, setConfPassword] = useState("");

    const subBTN  = useRef<HTMLButtonElement | null>(null);

    const router = useRouter();

    const subForm = async ()=>{
        
        console.log(par.params.slugs);

        if(await (new Password(password,par.params.slugs[0],par.params.slugs[1])).setPassword()){
            return router.push("/login");
        }

        router.push("/signup");
    }

    useEffect(()=>{
        if(subBTN.current){
        if(password === confPassword && password !== ""){
            subBTN.current.disabled = false;
        }
        else{
            subBTN.current.disabled = true;
        }
        
        }
    },[password,confPassword]);


    useEffect(()=>{
        if(subBTN.current){
            subBTN.current.disabled = true;
        }
    },[]);

    const node = (
        <div className={styles.container}>
            <div className={styles.header}>

                <div><h3>EV</h3></div>

                <div className={styles.buttonContainer}>
                </div>
            </div>

            <div className={styles.formContainer}>
                <div className={styles.form}>

               <label>Password</label>
                    <div className={styles.passContainer}><input type="password" className={styles.inputField }
                    value={password}
                    onInput={(e)=>{
                        setPassword(e.target.value)
                    }}/><span><i className="fa-regular fa-eye"></i></span></div>
                <label>Password</label>
                    <input type="password" className={styles.inputField} value={confPassword} 
                    
                    onInput={(e)=>{
                        setConfPassword(e.target.value);
                    }}
                    />
                 

                    <button ref={subBTN}
                    className={styles.subBtn} onClick={(e) => {
                        console.log(e);
                    }}>Do</button>
                </div>
            </div>
        </div>
    )

    return (<MainLayout children={node} title={"Verify"} />);
}