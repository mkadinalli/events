import styles from "./page.module.css"
import MainLayout from "../layouts/mainLayout"

export default function Home(){
    const node =  <h1>Home page</h1>

    return < MainLayout children={node} title={"Home"} />

}