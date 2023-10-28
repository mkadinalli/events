interface follower{
    userID: string;
    followID: string;
}

export default class Follower implements follower{
    constructor(
        public userID: string,
        public followID: string
    ){}

    public toString(this: Follower): string{
        return JSON.stringify(this);
    }
}