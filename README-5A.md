Project 5A - Concurrent Store
====================

<!-- TODO: Fill this out. -->

## Design Overview:

## Collaborators:

## Extra Credit Attempted:

## Tweeter Reflection Questions:

### Q1.

Primary (use for most of assignment):   Stakeholder Pair #3: Angel Yoline and Film Enthusiasts
Secondary (use for Question 3B):  Stakeholder Pair #1: Congressperson Kirby and The Freedom House Advocacy Group

### Q2.

The delete I implemented was to delete all of the user's posts and replies to that post. Angel wished for her post, Brad's post, and public discourse about the controversy to be deleted. Angel wished for her posts that "characterize working class people in a negative light" to be deleted. However, without some machine learning algorithm of human moderation, it would be tough to understand exactly which posts of hers fit that description. However, deleting ALL her posts would make sure that these negative posts would also be taken down. Furthermore, Angel most likely wishes for the controversy to be buried more than keeping some of her other posts active. We again face the same issue with Brad's posts. It is impossible to rigourously determine which of Brad's posts are negative to Angel and as he is a separate user we can't just delete all of his posts. The same holds true with general public discourse. We cannot delete all internet posts and without human moderation we can't tell which posts are about the controversy. Even screening posts mentioning 'Angel' would be ineffective as there could be multiple Angel's. Even if posts included '@user_10', we can't just delete all of those because that would be limiting user's free speech if the posts were not anything negative about Angel. It is a similar argument to how accused criminals are innocent until proven guilty. However, we can delete all posts replying to Angel's posts. All posts in such threads are most likely related to the controversy. So by deleting these posts we are satisfying Angel's wishes while not infinging on the expressive abilities of other users. There are of couse many limitations to this approach. First is that as a public figure many may view this entire deletion request null as people are free to comment on public figures. However, Angel is free to remove her own content and replies to said content are branching off of Angel's IP in a sense so she should have control over those posts as well. Furthermore, our deletion approach cannot remove all posts pertaining to the controversy as there is no way to classify these posts into such buckets. Unfortunately, once a rumor or controversy spreads across the internet, without manually reading every post and moderating it, there is no way to censor public discourse. 

### Q3b.

Some shortcomings of my implementation are in the safety and legal areas of data privacy. For example, if someone spreads death threats or proven false rumors that endanger a user's safety, all of these posts should be taken down, even if they did not originate from that user's posts or threads. However, the current impelementation would not be rigorous enough to do so. Furthermore, if other user's were using the complaining user's IP in their posts, their posts should be subject to deletion by the complaining user. However, again the current implementation does not account for these. Therefore, it would be prudent to have constant human moderation of posts before they are posted to make sure there are no safety or legal problems. 

Both of my stakeholders have similar issues. They have made controversial Tweeter comments and wish to remove these comments as well as the controversy surrounding them. I believe my implementation works well for both these stakeholders. Deleting Congressperson Kirby's tweets and replies to those tweets would ensure that his personal data/tweets are removed and forgotten. However, again it is unreasonable and unfeasible to remove other public discourse about said controversy from other users. Congressperson Kirby has the right to be forgotten but he cannot force others to forget him. Consider if they had made the same controversial comments at a rally in front of many people. They themselves could choose to forget about the comments, but other people would still remember it and discuss it. The same should apply with the internet. 

Therfore, I think my general implementation of gdprdelete would remain mostly the same. I would delete all the user's posts and replies to those posts. 

## Approximately how long did it take to complete the Concurrent Store portion of KVStore?

5 hours

## Where would you rank the difficulty of the Concurrent Store portion of KVStore, compared to Snake, DMalloc, Caching I/O, and WeensyOS?

---Most difficult---
1. <br Caching IO/>
2. <br DMalloc/>
3. <br Snake/>
4. <br WeensyOS/>
5. <br Concurrent Store/>
---Least difficult---
