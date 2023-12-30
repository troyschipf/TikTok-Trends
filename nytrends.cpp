#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <regex>
#include "TIKTOK.h"

std::string getSubString(std::string input, std::string first, std::string last){
    std::size_t startPos = input.find(first);
    if(startPos == std::string::npos){
        return "";
    }
    startPos += first.length();

    std::size_t endPos = input.find(last);
    if(endPos == std::string::npos){
        return "";
    }
    return input.substr(startPos, endPos-startPos);
}


// functor that sorts my priority queue for sounds
class sortSoundQUEUE{
public:
	bool operator()(std::pair<long long int,std::string> a, std::pair<long long int,std::string> b){
		if(a.first == b.first){
			return a.second > b.second;
		}
		else{
			return a.first < b.first;
		}
	}

};

// create a function that builds a map of each sound that is used using the MusicMeta musicId map the musicId to the amount of occurences
// based off these occurences create a priority queue from the unordered map
// take the top 10 sounds in the priority queue
void top10sounds(std::vector<Tiktok*> tiktoks, std::unordered_map<std::string,long long int>& soundCount, std::ofstream& output){
	// creates the map of all the music ids and their views
	for(unsigned int i=0; i<tiktoks.size(); i++){
		soundCount[tiktoks[i]->getMusicMeta()->getMusicId()] += std::stoll(tiktoks[i]->getPlayCount());

	}
	// sorts the info from the map in a queue
	std::priority_queue<std::pair<long long int ,std::string>, std::vector<std::pair<long long int,std::string> >, sortSoundQUEUE> orderedCount;
	for(std::unordered_map<std::string,long long int>::iterator itr = soundCount.begin(); itr != soundCount.end();itr++){
		orderedCount.push({itr->second,itr->first});
	}
	std::vector<std::pair<long long int, std::string> > top10;
	int count = 0;
	// adds top ten from queue to vector
	while(count < 10){
		top10.push_back(orderedCount.top());
		orderedCount.pop();
		count++;
	}
	output << "trending sounds:" << "\n" << std::endl;
	for(unsigned int i=0; i<top10.size(); i++){
		// .second == MusicId	.first == views
		std::priority_queue<std::pair<long long int, std::pair<std::string,std::string>>> topSounds;
		std::string musicName;
		std::string musicAuthor;
		// creates a vector for top videos using this sound
		for(unsigned int j=0; j<tiktoks.size(); j++){
			if(top10[i].second == tiktoks[j]->getMusicMeta()->getMusicId()){
				topSounds.push({std::stoi(tiktoks[j]->getPlayCount()),{tiktoks[j]->getVideoMeta()->getCoverUrl(), tiktoks[j]->getWebVideoUrl()}});
				musicName = tiktoks[j]->getMusicMeta()->getMusicName();
				musicAuthor = tiktoks[j]->getMusicMeta()->getMusicAuthor();
			}
		}
		output << "========================" << "\n" << musicName << "\n" << top10[i].first << " views" << "\n" << musicAuthor <<
		"\n" << "music id: " << top10[i].second << "\n" << std::endl;
		int count = 1;
		// prints the top videos depending on how big the queue is 
		if(topSounds.size() == 1){
			std::pair<long long int, std::pair<std::string, std::string> > top = topSounds.top();
    		topSounds.pop();
    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
		}
		if(topSounds.size() == 2){
			while(count <=2){
				std::pair<long long int, std::pair<std::string, std::string> > top = topSounds.top();
	    		topSounds.pop();
	    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
	    		count++;
	    	}
		}
		if(topSounds.size() >=3){
	    	while(count <= 3){
	    		std::pair<long long int, std::pair<std::string, std::string> > top = topSounds.top();
	    		topSounds.pop();
	    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
	    		count++;
	    	}
	    }
    	output << "========================" << std::endl;

	}
}

// functions to extract hashtags from body
std::vector<std::string> extractHashtags(std::string text){
	// define a regular expression to match hashtags
	std::vector<std::string> hashtags;
	std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)");

	// create an iterator for matching
	std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
	std::sregex_iterator endIterator;

	// iterate over the matches and extract the hashtags
	while (hashtagIterator != endIterator) {
	    std::smatch match = *hashtagIterator;
	    std::string hashtag = match.str(1);  // extract the first capturing group
	    hashtags.push_back("#" + hashtag);
	    ++hashtagIterator;
	}
	return hashtags;
}

// functor to sort the vector
class sortPQUEUE{
public:
	bool operator()(std::pair<int,std::pair<long long int, std::string> > a, std::pair<int,std::pair<long long int, std::string> > b){
		if(a.first == b.first){
			if(a.second.first == b.second.first){
				return a.second.second > b.second.second;
			}
			else{
				return a.second.first < b.second.first;
			}
		}
		else{
			return a.first < b.first;
		}
	}

};

// Definition of the top 10 trending hashtags: this should be the based on the usage of the hashtag - how many times in total each hashtag is used.
// When two hashtags are both used for the same amount of times, break the tie by the total view count of the videos associated with each hashtag. 
// And if still a tie, break the tie by the hashtag's name, and the smaller name is the winner.
// extract the hashtags for each files then map the occurence of each tag to the value 
void top10Hashtags(std::vector<Tiktok*> tiktoks, std::unordered_map<std::string, std::pair<int,long long int> >& tagCount, std::ofstream& output){
	for(unsigned int i=0;i<tiktoks.size();i++){
		std::vector<std::string> hashtags = extractHashtags(tiktoks[i]->getText());
		int count = 1;
		for(unsigned int j=0; j<hashtags.size();j++){
			// Assuming tagCount is a std::unordered_map<std::string, std::pair<int, int>>
			std::pair<int, long long int>& entry = tagCount[hashtags[j]];
			entry.first += count;
			entry.second += std::stoll(tiktoks[i]->getPlayCount());
		}
	}
	// first int is total hashtag usage, second int is the total view count, string is hashtag name
	std::priority_queue<std::pair<int,std::pair<long long int, std::string> >,std::vector<std::pair<int,std::pair<long long int, std::string> > >, sortPQUEUE > sortedTags;
	// iterates through all the hashtags mapped to their amount of uses creates a priority queue to sort them
	for(std::unordered_map<std::string, std::pair<int,long long int> >::iterator itr = tagCount.begin(); itr!=tagCount.end();itr++){
		sortedTags.push({itr->second.first,{itr->second.second, itr->first}});
	}
	
	// std::vector<std::pair<int,std::pair<long long int, std::string> > > comparison;
	std::vector<std::pair<int,std::pair<long long int, std::string> > > top10;
	int count = 0;
	while(count < 10){
		top10.push_back(sortedTags.top());
		// std::cout << sortedTags.top().second.first << " " << sortedTags.top().second.second << std::endl;
		sortedTags.pop();
		count++;
	}

	output << "trending hashtags:" << "\n" << std::endl;
	for(unsigned int i=0;i<top10.size(); i++){
		output << "========================" << "\n" << top10[i].second.second << "\n" << "used " << top10[i].first << " times" << "\n" 
		<< top10[i].second.first << " views" << "\n" << std::endl;
		// creates priority queue for top viewed videos using that hashtag
		std::priority_queue<std::pair<int, std::pair<std::string, std::string> > > topTagVideos;
		for(unsigned int j=0; j<tiktoks.size(); j++){
			// if this tiktok uses the current hashtag add it to the stack of most viewed
			std::vector<std::string> hashtags = extractHashtags(tiktoks[j]->getText());
			for(unsigned int k=0; k<hashtags.size();k++){
				if(hashtags[k] == top10[i].second.second){
					topTagVideos.push({std::stoi(tiktoks[j]->getPlayCount()),{tiktoks[j]->getVideoMeta()->getCoverUrl(), tiktoks[j]->getWebVideoUrl()}});
				}
			}
		}
		int count = 1;
		// prints the top videos based on the amount of videos in the queue
		if(topTagVideos.size() == 1){
			std::pair<int, std::pair<std::string, std::string> > top = topTagVideos.top();
    		topTagVideos.pop();
    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
		}
		if(topTagVideos.size() == 2){
			while(count <=2){
				std::pair<int, std::pair<std::string, std::string> > top = topTagVideos.top();
	    		topTagVideos.pop();
	    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
	    		count++;
	    	}
		}
		if(topTagVideos.size() >=3){
	    	while(count <= 3){
	    		std::pair<int, std::pair<std::string, std::string> > top = topTagVideos.top();
	    		topTagVideos.pop();
	    		output << "cover url: " << top.second.first << "\n" << "web video url: " << top.second.second << std::endl;
	    		count++;
	    	}
	    }	
	    output << "========================" << std::endl;
	}
}

int main(int argc, char * argv[]){
	std::ifstream jsonFile(argv[1]);
	std::ofstream output(argv[2]);
	std::string trendType = argv[3];

	// checks for proper amount of command line arguments 
	if(argc != 4){
		std::cerr << "Improper amount of command line arguements." << std::endl;
		return 1;
	}

	// checks if input file is open
	try{
		if (!jsonFile.is_open()) {
            throw std::string("Failed to open the input file.");
	    }
	}
	catch(std::string& error){
		std::cerr << argv[1] << error << std::endl;
		exit(1);
	}


    // checks if output file is open
    try{
		if(!output.is_open()){
			throw std::string("Failed to open the output file.");
		}
	}
	catch(std::string& error){
		std::cerr << argv[2] << error << std::endl;
		exit(1);
	}

    // variables used to read in the file
    std::string json_content;
    std::string line;
    std::string aID;
    std::string aText;
    std::string aCreateTime;
    std::string aCreateTimeIso;
    std::string aAuthorMetaStr;
	std::string aMusicMetaStr;
	std::string aWebVideoUrl;
	std::string aVideoMetaStr;
	std::string aDiggCount;
	std::string aShareCount;
	std::string aPlayCount;
	std::string aCommentCount;
	std::string aMentions;
	std::vector<Tiktok*> tiktoks;
	// reads in and constructs all the information from the input file
    while (std::getline(jsonFile, line)) {
    	aID = getSubString(line, "\"id\": \"", "\", \"text\":");
    	aText = getSubString(line, "\"text\": \"", "\", \"createTime\":");
    	aCreateTime = getSubString(line, "\"createTime\": ", ", \"createTimeISO\":");
    	aCreateTimeIso = getSubString(line, "\"createTimeISO\": \"", "\", \"authorMeta\":");
    	aAuthorMetaStr = getSubString(line, "\"authorMeta\": {", "\"musicMeta\":");
    	// obtains all the information for the AuthorMeta object
    	std::string aId = getSubString(aAuthorMetaStr, "\"id\": \"", "\", \"name\":"); 
    	std::string aNameA = getSubString(aAuthorMetaStr, "\"name\": \"", "\", \"nickName\":"); 
    	std::string aNickName = getSubString(aAuthorMetaStr, "\"nickName\": \"", "\", \"verified\":");
    	std::string aVerified = getSubString(aAuthorMetaStr, "\"verified\": ", ", \"signature\":"); 
    	std::string aSignature = getSubString(aAuthorMetaStr, "\"signature\": \"", "\", \"bioLink\":");
    	std::string abioLink = getSubString(aAuthorMetaStr, "\"bioLink\": ", ", \"avatar\":"); 
    	std::string aAvatar = getSubString(aAuthorMetaStr, "\"avatar\": \"", "\", \"privateAccount\":");
    	std::string aPrivateAccount = getSubString(aAuthorMetaStr, "\"privateAccount\":", "},");
    	std::string aRegion = getSubString(aPrivateAccount, "\"region\": \"", "\", \"following\":");
    	std::string aFollowing = getSubString(aPrivateAccount, "\"following\":", ", \"fans\":");
    	std::string aFans = getSubString(aPrivateAccount, "\"fans\":", ", \"heart\":");
		std::string aHeart = getSubString(aPrivateAccount, "\"heart\":" , ", \"video\":"); 
		std::string aVideo = getSubString(aPrivateAccount, "\"video\":", ", \"digg\":"); 
		std::string aDigg = getSubString(aPrivateAccount, "\"digg\":", "");
		std::string::size_type a = aPrivateAccount.find("false");
		if(a !=  std::string::npos){
			aPrivateAccount.erase((a+5), std::string::npos);
		}
		std::string::size_type b = aPrivateAccount.find("true");
		if(b !=  std::string::npos){
			aPrivateAccount.erase((b+4), std::string::npos);
		}
    	Tiktok::AuthorMeta* aAuthorMeta = new Tiktok::AuthorMeta(aId, aNameA, aNickName, aVerified, aSignature, abioLink, aAvatar, aPrivateAccount,
    	 aRegion, aFollowing, aFans, aHeart, aVideo, aDigg);
    	// obtains all the information for the MusicMeta object
    	aMusicMetaStr = getSubString(line, "\"musicMeta\": {", "\"webVideoUrl\":");
    	std::string aMusicName = getSubString(aMusicMetaStr, "\"musicName\": \"", "\", \"musicAuthor\":");
    	std::string aMusicAuthor = getSubString(aMusicMetaStr, "\"musicAuthor\": \"", "\", \"musicOriginal\":");
    	std::string aMusicOriginal = getSubString(aMusicMetaStr, "\"musicOriginal\": ", ", \"playUrl\":");
    	std::string aPlayUrl = getSubString(aMusicMetaStr, "\"playUrl\": \"", "\", \"coverMediumUrl\":");
		std::string aCoverMediumUrl = getSubString(aMusicMetaStr, "\"coverMediumUrl\": \"", "\", \"musicId\":");
		std::string aMusicId = getSubString(aMusicMetaStr, "\"musicId\": \"", "\"},");
    	aWebVideoUrl = getSubString(line, "\"webVideoUrl\": \"", "\", \"videoMeta\":");
    	Tiktok::MusicMeta* aMusicMeta = new Tiktok::MusicMeta(aMusicName, aMusicAuthor, aMusicOriginal, aPlayUrl, aCoverMediumUrl, aMusicId);
    	// obtains all the information for the VideoMeta object
    	aVideoMetaStr = getSubString(line, "\"videoMeta\": {", "\"diggCount\":");
    	std::string aHeight = getSubString(aVideoMetaStr, "\"height\": ", ", \"width\": ");
    	std::string aWidth = getSubString(aVideoMetaStr, ", \"width\": ", ", \"duration\": ");
    	std::string aDuration = getSubString(aVideoMetaStr, ", \"duration\": ", ", \"coverUrl\": \"");
    	std::string aCoverUrl = getSubString(aVideoMetaStr, ", \"coverUrl\": \"", "\", \"definition\": \"");
		std::string aDefinition = getSubString(aVideoMetaStr, "\", \"definition\": \"", ", \"format\": ");
		std::string aFormat = getSubString(aVideoMetaStr, "\"format\": \"", "\", \"downloadAddr\": \"");
		std::string aDownloadAddr = getSubString(aVideoMetaStr, "\", \"downloadAddr\": \"", "\"},");
    	Tiktok::VideoMeta* aVideoMeta = new Tiktok::VideoMeta(aHeight, aWidth, aDuration, aCoverUrl, aDefinition, aFormat, aDownloadAddr);

    	aDiggCount = getSubString(line, "\"diggCount\": ", ", \"shareCount\":");
    	aShareCount = getSubString(line, "\"shareCount\": ", ", \"playCount\":");
    	aPlayCount = getSubString(line, "\"playCount\": ", ", \"commentCount\":");
    	aCommentCount = getSubString(line, "\"commentCount\": ", ", \"mentions\":");
    	aMentions = getSubString(line, "\"mentions\": [", "]}");

        tiktoks.push_back(new Tiktok(aID, aText, aCreateTime, aCreateTimeIso, aAuthorMeta, aMusicMeta, aWebVideoUrl, aVideoMeta, aDiggCount,
        	aShareCount, aPlayCount, aCommentCount, aMentions));
    }
    // don't need this json file anymore, as the content is read into json_content.
    jsonFile.close();
    // std::cout << tiktoks[189]->get() << std::endl;
    if(trendType == "sound"){
    	std::unordered_map<std::string,long long int> soundCount;
    	top10sounds(tiktoks,soundCount, output);
    }
    if(trendType == "hashtag"){
    	std::unordered_map<std::string, std::pair<int,long long int> > tagCount;
    	top10Hashtags(tiktoks, tagCount, output);
    }
    // any where where i go through everything
	return 0;
}