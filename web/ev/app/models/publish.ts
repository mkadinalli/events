interface publish{
    title: string;
    price: number;
    description: string;
    venue: string;
    date: string;
    deadlineDate: string;
    publisherID: string;
}

export default class Publish implements publish{
    constructor(
        public title: string,
        public price: number,
        public description: string,
        public venue: string,
        public date: string,
        public deadlineDate: string,
        public publisherID: string
    ){}

    public toString(this: Publish): string{
        return JSON.stringify(this);
    }
}