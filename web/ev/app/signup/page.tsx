"use client"

import { useRouter } from "next/navigation";
import MainLayout from "../layouts/mainLayout"
import styles from "./page.module.css"
import { useEffect, useRef, useState } from "react";
import User from "../models/user"

export default function SignUp() {
    const router = useRouter();

    const [fullName, setFullName] = useState("");
    const [username, setUserName] = useState("");
    const [email, setEmail] = useState("");
    const [isValid,setValid] = useState(false);

    const unameSpinner = useRef<HTMLSpanElement>(null);

    const goLogin = () => {
        router.push("/login");
    }

    const subForm = async () => {
        var user = new User(fullName, username, email);

        console.log(JSON.stringify(user));

        const res = await user.postUser();


        console.log(res);

        /**
         *                 <label>Password</label>
                    <div className={styles.passContainer}><input type="password" className={styles.inputField} value={password} onInput={(e)=>{setPassword(e.target.value)}}/><span><i className="fa-regular fa-eye"></i></span></div>
                <label>Password</label>
                    <input type="password" className={styles.inputField} value={passwordRep} onInput={(e)=>{setPasswordRep(e.target.value)}}/>
                   
         */

    }

    const checkUserNameValidity = async (uname: string)=>{

        setUserName(uname)
        if(unameSpinner.current === null) return;

        unameSpinner.current.innerHTML = `<i class="fa-solid fa-spinner fa-spin"></i>`;

        if(await User.checkUser(uname))
        {
            unameSpinner.current.innerHTML = `<i class="fa-regular fa-circle-xmark"></i>`;
            return;    
        }
        
        unameSpinner.current.innerHTML = `<i class="fa-regular fa-circle-check"></i>`;
    }

    const node = (
        <div className={styles.container}>
            <div className={styles.header}>

                <div><h3>EV</h3></div>

                <div className={styles.buttonContainer}>
                    <div><span className={styles.label}>Already have an account?</span></div>
                    <button className={styles.btn} onClick={(e) => {
                        goLogin();
                    }}>Login</button>
                </div>
            </div>

            <div className={styles.formContainer}>
                <div className={styles.form}>
                    <label>Name</label>
                    <input type="text" className={styles.inputField} value={fullName} onInput={(e) => { setFullName(e.target.value) }} />
                    <label>Username</label>


                    <div className={styles.userContainer}><input type="text" className={styles.inputField} value={username} onChange={(e) => { checkUserNameValidity(e.target.value) }} />
                    <span ref={unameSpinner}></span></div>
                    <label>Email</label>
                    <input type="text" className={styles.inputField} value={email} onInput={(e) => { setEmail(e.target.value) }} />
                    <button className={styles.subBtn} onClick={(e) => {
                        subForm();
                    }}>Do</button>
                </div>
            </div>
        </div>
    )

    return (
        <MainLayout children={node} title={"Sign In"} />
    )
}