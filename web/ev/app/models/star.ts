interface star{
    userID: string;
    publishID: string;
}

export default class Star implements star{
    constructor(
        public userID: string,
        public publishID: string
    ){}

    public toString(this: Star): string{
        return JSON.stringify(this);
    }
}