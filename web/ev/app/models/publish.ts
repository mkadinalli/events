interface publish{
    title: string;
    price: number;
    description: string;
    venue: string;
    date: string;
    deadlineDate: string;
    publisherID: string;

    initialized: boolean
}

export default class Publish implements publish{
    initialized = false;

    constructor(
        public title: string,
        public price: number,
        public description: string,
        public venue: string,
        public date: string,
        public deadlineDate: string,
        public publisherID: string
    ){
        this.initialized = true;
    }

    public toString(this: Publish): string{
        return JSON.stringify(this);
    }

    public async send(): Promise<boolean>{
        if(!this.initialized)
            return false;

        const res = await fetch("http://localhost:2000/api/events/",{
            method: "POST",
            headers:{
                "Content-Type": "application/json"
            },

            body: this.toString()
        });

        const data = await res.json();

        console.log(data);

        return true;
    }
}