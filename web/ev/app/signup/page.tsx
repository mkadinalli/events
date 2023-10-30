"use client"

import { useRouter } from "next/navigation";
import MainLayout from "../layouts/mainLayout"
import styles from "./page.module.css"
import { useState } from "react";
import User from "../models/user"

export default function SignUp()
{
    const router = useRouter();

    const [fullName, setFullName] = useState("");
    const [username, setUserName] = useState("");
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [passwordRep, setPasswordRep] = useState("");

    const goLogin = ()=>{
        router.push("/login");
    }

    const subForm = async ()=>{
        var user = new User(fullName,username,email,password);

        console.log(JSON.stringify(user));

        //const res = await user.postUser();

        if(await User.checkUser(username))
            console.log("user exists");
        else
            console.log("user not exist");

        //console.log(res);
            
    }

    const node = (
        <div className={styles.container}>
          <div className={styles.header}>
    
            <div><h3>EV</h3></div>
    
            <div className={styles.buttonContainer}>
            <div><span className={styles.label}>Already have an account?</span></div>
            <button className={styles.btn} onClick={(e)=>{
              goLogin();
            }}>Login</button>
            </div>
          </div>
            
        <div className={styles.formContainer}>
            <div className={styles.form}>
                <label>Name</label>
                    <input type="text" className={styles.inputField} value={fullName} onInput={(e)=>{setFullName(e.target.value)}}/>
                <label>Username</label>
                    <div className={styles.userContainer}><input type="text" className={styles.inputField} value={username} onInput={(e)=>{setUserName(e.target.value)}}/><span>Y</span></div>
                <label>Email</label>
                    <input type="text" className={styles.inputField} value={email} onInput={(e)=>{setEmail(e.target.value)}}/>
                <label>Password</label>
                    <div className={styles.passContainer}><input type="password" className={styles.inputField} value={password} onInput={(e)=>{setPassword(e.target.value)}}/><span>Y</span></div>
                <label>Password</label>
                    <input type="password" className={styles.inputField} value={passwordRep} onInput={(e)=>{setPasswordRep(e.target.value)}}/>
                    <button className={styles.subBtn} onClick={(e)=>{
                            subForm();
                    }}>Do</button>
            </div>
        </div>
        </div>
      )

    return(
        <MainLayout children={node} title={"Sign In"}/>
    )
}