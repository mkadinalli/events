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
            </head>
            <body className={Styles.cont}>
                {children}
            </body>
        </html>
    )
}