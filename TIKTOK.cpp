#include "TIKTOK.h"
#include <string>

Tiktok::AuthorMeta::AuthorMeta(std::string& aId, std::string& aNameA, std::string& aNickName, std::string& aVerified, std::string& aSignature, 
			std::string& abioLink, std::string& aAvatar, std::string& aPrivateAccount, std::string& aRegion, std::string& aFollowing, 
			std::string& aFans, std::string& aHeart, std::string& aVideo, std::string& aDigg){
	
	id = aId;
	name = aNameA;
	nickName = aNickName;
	if(aVerified == "true"){
		verified = true;
	}else{
		verified = false;
	}
	signature = aSignature; 
	bioLink = abioLink;
	avatar = aAvatar;
	if(aPrivateAccount == "true"){
		privateAccount = true;
	}else{
		privateAccount = false;
	}
	region = aRegion; 
	following = aFollowing;
	fans = aFans;
	heart = aHeart;
	video = aVideo;
	digg = aDigg;

}

Tiktok::MusicMeta::MusicMeta(std::string& aMusicName, std::string& aMusicAuthor, std::string& aMusicOriginal, std::string& aPlayUrl, 
			std::string& aCoverMediumUrl, std::string& aMusicId){
	musicName = aMusicName;
	musicAuthor = aMusicAuthor;
	musicOriginal = aMusicOriginal;
	playUrl = aPlayUrl;
	coverMediumUrl = aCoverMediumUrl;
	musicId = aMusicId;

}

Tiktok::VideoMeta::VideoMeta(std::string& aHeight, std::string& aWidth, std::string& aDuration, std::string& aCoverUrl, 
			std::string& aDefinition, std::string& aFormat,std::string& aDownloadAddr){
	height = aHeight;
	width = aWidth;
	duration = aDuration;
	coverUrl = aCoverUrl;
	definition = aDefinition;
	format = aFormat;
	downloadAddr = aDownloadAddr;
}

Tiktok::Tiktok(std::string& aID, std::string& aText, std::string& aCreateTime, std::string& aCreateTimeIso, AuthorMeta*& aAuthorMeta, 
	MusicMeta*& aMusicMeta, std::string& aWebVideoUrl, VideoMeta*& aVideoMeta, std::string& aDiggCount, std::string& aShareCount, 
	std::string& aPlayCount, std::string& aCommentCount, std::string& aMentions){
	id = aID;
	text = aText;
	createTime = aCreateTime; 
	createTimeIso = aCreateTimeIso;
	authorMeta = aAuthorMeta;
	musicMeta = aMusicMeta;
	webVideoUrl = aWebVideoUrl;
	videoMeta = aVideoMeta;
	diggCount = aDiggCount;
	shareCount = aShareCount;
	playCount = aPlayCount;
	commentCount = aCommentCount;
	mentions = aMentions;
}