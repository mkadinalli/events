"use client"

import { useRouter } from "next/navigation";
import MainLayout from "../layouts/mainLayout"
import styles from "./page.module.css"
import { useEffect, useRef, useState } from "react";
import LoginDetails from "../models/log"

export default function LogIn() {
    const router = useRouter();

    const [Password, setPassword] = useState("");
    const [email, setEmail] = useState("");


    const subBTN  = useRef<HTMLButtonElement | null>(null);

    const hideBtn = useRef<HTMLSpanElement>(null);

    const goSignin = () => {
        router.push("/signup");
    }

    const subForm = async () => {
        var logins = new LoginDetails(email,Password);

        if(await logins.login())
            router.push("/home");
        else
            console.log("Login failed");

    }

    const node = (
        <div className={styles.container}>
            <div className={styles.header}>

                <div><h3>EV</h3></div>

                <div className={styles.buttonContainer}>
                    <div><span className={styles.label}>Don't have an account?</span></div>
                    <button className={styles.btn} onClick={(e) => {
                        goSignin();
                    }}>Sign In</button>
                </div>
            </div>

            <div className={styles.formContainer}>
                <div className={styles.form}>
                    <label>Username</label>
                    <input type="text" className={styles.inputField} value={email} onInput={(e) => { setEmail(e.target.value) }} />
                    <label>Password</label>


                    <div className={styles.userContainer}><input type="text" className={styles.inputField} value={Password} onChange={(e) => { setPassword(e.target.value) }} />
                    <span ref={hideBtn}></span></div>
                    <button ref={subBTN}
                    className={styles.subBtn} onClick={(e) => {
                        subForm();
                    }}>Do</button>
                </div>
            </div>
        </div>
    )

    return (
        <MainLayout children={node} title={"Log In"} />
    )
}