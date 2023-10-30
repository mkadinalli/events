interface user {
    fullName: string;
    username: string;
    email: string;
    password: string;
}

export default class User implements user{
    constructor(
        public fullName: string,
        public username: string,
        public email: string,
        public password: string
    ){}

    public toString(this: User): string{
        return JSON.stringify(this);
    }

    public static async checkUser(username : string){
        const res = await fetch(`https://localhost:2000/api/checkuser/?userName=${username}`,{
            method : "GET"
        })

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
}