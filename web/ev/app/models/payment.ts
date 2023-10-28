interface payment {
    phone: string;
    amount: number;
    userID: string;
    publishID: string;
}

export default class Payment implements payment{
    constructor(
        public phone: string,
        public amount: number,
        public userID: string,
        public publishID: string
    ){}

    public toString(this: Payment): string{
        return JSON.stringify(this);
    }
}