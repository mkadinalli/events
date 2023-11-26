interface password{
    password :string;
    id: string;
    token: string;
}


export default class Password implements password{
    constructor(
        public password : string,
        public token: string,
        public id: string
    ){};

    public async setPassword(){
        const payLoad = {
            token : this.token,
            id : this.id,
            password: this.password
        };

        console.log(payLoad);

        const res = await fetch("http://localhost:2000/api/verify",{
            method: "POST",
            headers: {
                "content-type" : "application/json"
            },
            body: JSON.stringify(this)
        });

        const data = await res.json();

        if(data.results[0].success === "1") return true;

        return false;
    }
}