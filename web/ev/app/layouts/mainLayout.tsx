import React from "react";
import Styles from "./page.module.css"

export default function MainLayout({children, title} : { children : React.ReactNode,title : string})
{
    return(
        <html>
            <head>
                <title>
                    {title}
                </title>
                <script src="https://kit.fontawesome.com/fd6d60db34.js" crossOrigin="anonymous"></script>
            </head>
            <body className={Styles.cont}>
                {children}
            </body>
        </html>
    )
}