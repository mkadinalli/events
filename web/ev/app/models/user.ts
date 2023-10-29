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
}