"use client"

import Image from 'next/image'
import styles from './page.module.css'
import { useEffect, useRef, useState } from 'react'
import MainLayout from './layouts/mainLayout'
import { useRouter } from 'next/navigation'



export default function Home() {

  const router = useRouter();

  const goLogin = ()=>{
    router.push("/login");
  }

  const goSignin = ()=>{
    router.push("/signup");
  }

  const node = (
    <div className={styles.container}>
      <div className={styles.header}>

        <div><h3>EV</h3></div>

        <div className={styles.buttonContainer}>
        <button className={styles.btn} onClick={(e)=>{
          goLogin();
        }}>Sign in</button>
        <button className={styles.btn} onClick={(e)=>{
          goSignin();
        }}>Sign up</button>
        </div>
      </div>
    </div>
  )
  

  return (
   <MainLayout children={node} title={"Home"}/>
  )
}
