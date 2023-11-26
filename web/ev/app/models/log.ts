import Global from "./global"

interface loginDetails{
    emailOrUsername: string;
    password: string;
}


export default class LoginDetails implements loginDetails{
    constructor(
        public emailOrUsername: string,
        public password: string
    ){}

    public async login(){
        const res = await fetch(`http://localhost:2000/api/login/?id=${this.emailOrUsername}&password=${this.password}`,{
            method: "GET"
        });

        const data = await res.json();

        if(!data.success) return false;

        let user_id = data.results[0].matched
        
        if(user_id === "0")
            return false;
        

        Global.user_id = user_id;
        return true;
    }
}