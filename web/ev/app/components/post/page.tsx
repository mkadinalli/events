import Link from "next/link"
import styles from "./post.module.css"

export default function Post({props} : any){
    return(
        <div className={styles.cont}>
            
            <h1>{props.title}</h1>
            <p>{props.event_date}</p>
            <p>{props.subscriptions}</p>
            <Link href={`/pages/event/${props.id}`}>Expand</Link>
        </div>
    )
}