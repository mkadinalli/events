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
        const res = await fetch(`https://localhost:2000/api/login/?id=${this.emailOrUsername}&password=${this.password}`,{
            method: "GET"
        });

        const data = await res.json();

        if(!data.success) return false;
        
        if(data.results[0].matched === "1")
            return true;
        
        return false;
    }
}