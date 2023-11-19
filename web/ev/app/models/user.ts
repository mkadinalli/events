import { json } from "stream/consumers";
import Global from "./global";

interface user {
    fullName: string;
    username: string;
    email: string;
}

export default class User implements user{
    constructor(
        public fullName: string,
        public username: string,
        public email: string
    ){}

    public toString(this: User): string{
        return JSON.stringify(this);
    }

    public static async checkUser(username : string){
        const res = await fetch(`https://localhost:2000/api/checkuser/?userName=${username}`,{
            method : "GET"
        })

        if(!res.ok) return false;

        const data = await res.json();


        if(data.results[0].number === "0")
            return false;
        return true;
    }

    public async postUser(){
        const res = await fetch("https://localhost:2000/api/signup/",{
            method : "POST",
            headers : {
                "content-type": "application/json" 
            },
            body : this.toString()
        })

        const data = await res.json();

        return data;
    }

    static async update(name: string,bio: string,about: string){
        const res = await fetch("https://localhost:2000/api/user",{
            method: "PUT",
            headers:{
                "content-type": "application/json" 
            },
            body: JSON.stringify({name: name,bio: bio, about: about, id: Global.user_id})
        })

        const data = await res.json();

        console.log(data);
    }
}